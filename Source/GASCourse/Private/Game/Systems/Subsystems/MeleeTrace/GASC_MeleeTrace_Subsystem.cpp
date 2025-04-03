// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Systems/Subsystems/MeleeTrace/GASC_MeleeTrace_Subsystem.h"

#include "Game/Interfaces/GASC_CharacterWeapon_Interface.h"
#include "Kismet/GameplayStatics.h"

//TODO

/**
 * Add console command to show debug draw
 * 
 * 
 */

void UGASC_MeleeTrace_Subsystem::Tick(float DeltaTime)
{
	if (!GEngine || !GEngine->IsInitialized())
	{
		return;
	}
	UWorld* World = GetWorld();
	if (!World || !World->IsValidLowLevel())
	{
		UE_LOG(LogTemp, Warning, TEXT("World is invalid! Skipping tick."));
		return;
	}
	
	if (!IsInitialized())
	{
		return;
	}

	Super::Tick(DeltaTime);

	ProcessMeleeTraces(DeltaTime);
	
}

void UGASC_MeleeTrace_Subsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UGASC_MeleeTrace_Subsystem::Deinitialize()
{
	Super::Deinitialize();
	if (!MeleeTraceRequests.IsEmpty())
	{
		MeleeTraceRequests.Empty();
	}
}

bool UGASC_MeleeTrace_Subsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	UWorld* OuterWorld = Cast<UWorld>(Outer);

	// Allow the subsystem to be created in the editor world
	return OuterWorld && OuterWorld->WorldType == EWorldType::Editor || Super::ShouldCreateSubsystem(Outer);
}

void UGASC_MeleeTrace_Subsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
}
#if WITH_EDITOR
void UGASC_MeleeTrace_Subsystem::InEditor_DrawSphereMeleeTrace(USkeletalMeshComponent* SkeletalMeshComponent, float inRadius,
	FName inTraceSocketStart)
{

	if (GEditor && !GIsPlayInEditorWorld)
	{
		FVector StartLocation = FVector::ZeroVector;
		FVector EndLocation = FVector::ZeroVector;
		if (SkeletalMeshComponent)
		{
			if (SkeletalMeshComponent->DoesSocketExist(inTraceSocketStart))
			{
				StartLocation = SkeletalMeshComponent->GetSocketLocation(inTraceSocketStart);
				EndLocation = StartLocation;
			}
		}
		else
		{
			StartLocation = SkeletalMeshComponent->GetOwner()->GetActorLocation();
			EndLocation = StartLocation;
		}
			
		DrawDebugSphere(SkeletalMeshComponent->GetWorld(), StartLocation, inRadius, 8, FColor::Red, false, 1.0f, 0, 1.0f);
	}
}
#endif

void UGASC_MeleeTrace_Subsystem::RequestSphereMeleeTrace(AActor* Instigator, float inRadius, FName inTraceSocketStart,
                                                         TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes, bool bUseWeapon, FGuid TraceId)
{
	FGASC_MeleeTrace_Subsystem_Data NewMeleeTraceRequest;
	
	NewMeleeTraceRequest.InstigatorActor = Instigator;
	NewMeleeTraceRequest.TraceShape = ETraceShape::Sphere;
	NewMeleeTraceRequest.TraceRadius = inRadius;
	NewMeleeTraceRequest.TraceSocketStart = inTraceSocketStart;
	NewMeleeTraceRequest.ObjectTypes = ObjectTypes;
	NewMeleeTraceRequest.TraceId = TraceId;

	if (bUseWeapon)
	{
		GetSocketTraceLocationFromMesh(Instigator, inTraceSocketStart, FName(""));
	}

	if (USkeletalMeshComponent* SkeletalMeshComponent = Instigator->FindComponentByClass<USkeletalMeshComponent>())
	{
		if (SkeletalMeshComponent->DoesSocketExist(inTraceSocketStart))
		{
			NewMeleeTraceRequest.CurrentPosition = SkeletalMeshComponent->GetSocketLocation(inTraceSocketStart);
			NewMeleeTraceRequest.PreviousPosition = NewMeleeTraceRequest.CurrentPosition;
		}
	}
	else
	{
		NewMeleeTraceRequest.CurrentPosition  = Instigator->GetActorLocation();
		NewMeleeTraceRequest.PreviousPosition = NewMeleeTraceRequest.CurrentPosition;
	}
	
	MeleeTraceRequests.Add(NewMeleeTraceRequest);
}

bool UGASC_MeleeTrace_Subsystem::IsMeleeTraceInProgress(FGuid TraceId)
{
	for (const FGASC_MeleeTrace_Subsystem_Data& MeleeTraceRequest : MeleeTraceRequests)
	{
		if (MeleeTraceRequest.TraceId == TraceId)
		{
			return true;
		}
	}
	
	return false;
}

bool UGASC_MeleeTrace_Subsystem::CancelMeleeTrace(FGuid TraceId)
{
	bool bCanceledByUser = false;

	for (int32 i = MeleeTraceRequests.Num() - 1; i >= 0; --i)
	{
		if (MeleeTraceRequests[i].TraceId == TraceId)
		{
			MeleeTraceRequests.RemoveAt(i);
			bCanceledByUser = true;
		}
	}
	return bCanceledByUser;
}

void UGASC_MeleeTrace_Subsystem::ProcessMeleeTraces(float DeltaTime)
{
	if (MeleeTraceRequests.Num() == 0)
	{
		return;
	}

	for (FGASC_MeleeTrace_Subsystem_Data& MeleeTraceRequest : MeleeTraceRequests)
	{
		UWorld* World = GetWorld();
		if (World && MeleeTraceRequest.InstigatorActor)
		{
			float DistanceDelta = FVector::Distance(MeleeTraceRequest.CurrentPosition, MeleeTraceRequest.PreviousPosition);
			//int32 NumSteps = FMath::CeilToInt(DeltaTime * 60.0f);
			int32 NumSteps = FMath::Max(1,FMath::CeilToInt(DistanceDelta / MeleeTraceRequest.TraceRadius));
			UE_LOG(LogTemp, Warning, TEXT("NumSteps: %d"), NumSteps);

			FVector StartPosition = MeleeTraceRequest.PreviousPosition;
			FVector EndPosition; 
			FVector StepVector = (MeleeTraceRequest.CurrentPosition - MeleeTraceRequest.PreviousPosition) / NumSteps;
			DrawDebugSphere(World, StartPosition, MeleeTraceRequest.TraceRadius, 8, FColor::Purple, false, 1.0f, 0, 1.0f);
			for (int32 StepIndex = 0; StepIndex < NumSteps; ++StepIndex)
			{
				EndPosition = StartPosition + StepVector;
				EDrawDebugTrace::Type DrawDebugType = EDrawDebugTrace::ForDuration;
				TArray<FHitResult> StepHits;
				FCollisionQueryParams QueryParams;
				QueryParams.AddIgnoredActor(MeleeTraceRequest.InstigatorActor);
				
				bool bHit = World->SweepMultiByObjectType(StepHits, StartPosition, EndPosition, FQuat::Identity, FCollisionObjectQueryParams(MeleeTraceRequest.ObjectTypes), FCollisionShape::MakeSphere(MeleeTraceRequest.TraceRadius), QueryParams);
				if (bHit)
				{
					UE_LOG(LogTemp, Warning, TEXT("Hit!"));
				}

				FColor TraceColor = bHit ? FColor::Green : FColor::Red;
				DrawDebugSphere(World, EndPosition, MeleeTraceRequest.TraceRadius, 8, TraceColor, false, 1.0f, 0, 1.0f);

				StartPosition = EndPosition;
			}

			MeleeTraceRequest.PreviousPosition = MeleeTraceRequest.CurrentPosition;
			if (USkeletalMeshComponent* SkeletalMeshComponent = MeleeTraceRequest.InstigatorActor->FindComponentByClass<USkeletalMeshComponent>())
			{
				if (SkeletalMeshComponent->DoesSocketExist(MeleeTraceRequest.TraceSocketStart))
				{
					MeleeTraceRequest.CurrentPosition = SkeletalMeshComponent->GetSocketLocation(MeleeTraceRequest.TraceSocketStart);
				}
			}
			else
			{
				MeleeTraceRequest.CurrentPosition  = MeleeTraceRequest.InstigatorActor->GetActorLocation();
			}
		}
	}
}

FVector UGASC_MeleeTrace_Subsystem::GetSocketTraceLocationFromMesh(AActor* Instigator, FName TraceStartSocketName, FName TraceEndSocketName) const
{
	//bIsImplemented = OriginalObject->GetClass()->ImplementsInterface(UReactToTriggerInterface::StaticClass());
	if (Instigator && Instigator->GetClass()->ImplementsInterface(UGASC_CharacterWeapon_Interface::StaticClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Implements INTERFACE!"));
		TScriptInterface<IGASC_CharacterWeapon_Interface> MyScriptInterface = TScriptInterface<IGASC_CharacterWeapon_Interface>(Instigator);
		UStaticMeshComponent* WeaponMesh = MyScriptInterface->Execute_GetWeaponMeshComponent(Instigator);
		if (WeaponMesh)
		{
			UE_LOG(LogTemp, Warning, TEXT("Weapon Mesh: %s"), *WeaponMesh->GetFullName());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("NO WEAPON MESH FOUND"));
		}
		
	}
	return FVector::ZeroVector;
}