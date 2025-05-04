// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Systems/Subsystems/MeleeTrace/GASC_MeleeTrace_Subsystem.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Game/Systems/Subsystems/MeleeTrace/Settings/GASC_MeleeSubsystem_Settings.h" 
#include "KismetTraceUtils.h"
#include "Game/Systems/Subsystems/MeleeTrace/Shapes/GASC_MeleeShape_Base.h"
#include "DrawDebugHelpers.h"
#include "CollisionQueryParams.h"
#include "Kismet/KismetMathLibrary.h"
#include "AbilitySystemComponent.h"
#include "GASCourse/GASCourseCharacter.h"
#include "NativeGameplayTags.h"
#include "Game/GameplayAbilitySystem/GASCourseNativeGameplayTags.h"

static const float KISMET_TRACE_DEBUG_IMPACTPOINT_SIZE = 8.f;
DEFINE_LOG_CATEGORY(LOG_GASC_MeleeTraceSubsystem);


namespace GASCourse_MeleeSubSystemCVars
{
	static TAutoConsoleVariable<bool> CvarEnableMeleeTracesDebug(TEXT("GASCourseDebug.MeleeTrace.CollisionShapes"),
		false,
		TEXT("Enable on-screen debug collision shape preview for melee traces.(Enabled: true, Disabled: false)"));
	
}

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

void UGASC_MeleeTrace_Subsystem::DrawDebugMeleeTrace(const UObject* WorldContextObject,
	const FCollisionShape& MeleeTraceShape, const FTransform& Start, const FTransform& End, bool bHit,
	const TArray<FHitResult>& HitResults)
{
	const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!World)
	{
		return;
	}
	
	const UGASC_MeleeSubsystem_Settings* MeleeTraceSettings = GetDefault<UGASC_MeleeSubsystem_Settings>();
	check(MeleeTraceSettings);

	const FColor TraceColor = MeleeTraceSettings->MeleeTraceDebugColor;
	const FColor TraceColor_Hit = MeleeTraceSettings->MeleeTraceHitDebugColor;
	const float DebugLifeTime = MeleeTraceSettings->DebugDrawTime;

	if (MeleeTraceShape.IsSphere())
	{
		DrawDebugSphereTraceMulti(World,
			Start.GetLocation(),
			End.GetLocation(),
			MeleeTraceShape.GetSphereRadius(),
			EDrawDebugTrace::ForDuration,
			bHit,
			HitResults,
			TraceColor,
			TraceColor_Hit,
			DebugLifeTime);
	}
	else if (MeleeTraceShape.IsCapsule())
	{
		DrawDebugCapsuleTraceMulti(World,
			Start.GetLocation(),
			End.GetLocation(),
			Start.GetRotation(),
			MeleeTraceShape.GetCapsuleRadius(),
			MeleeTraceShape.GetCapsuleHalfHeight(),
			EDrawDebugTrace::ForDuration,
			bHit,
			HitResults,
			TraceColor,
			TraceColor_Hit,
			DebugLifeTime);
	}
	else if (MeleeTraceShape.IsBox())
	{
		DrawDebugBoxTraceMulti(World,
			Start.GetLocation(),
			End.GetLocation(),
			End.GetRotation().Rotator(),
			MeleeTraceShape.GetExtent(),
			EDrawDebugTrace::ForDuration,
			bHit,
			HitResults,
			TraceColor,
			TraceColor_Hit,
			DebugLifeTime);
	}
}

void UGASC_MeleeTrace_Subsystem::DrawDebugSphereTraceMulti(const UWorld* World, const FVector& Start,
	const FVector& End, float Radius, EDrawDebugTrace::Type DrawDebugType, bool bHit, const TArray<FHitResult>& Hits,
	const FLinearColor& TraceColor, const FLinearColor& TraceHitColor, float DrawTime)
{
	if (DrawDebugType != EDrawDebugTrace::None)
	{
		const bool bPersistent = DrawDebugType == EDrawDebugTrace::Persistent;
		const float LifeTime = (DrawDebugType == EDrawDebugTrace::ForDuration) ? DrawTime : 0.f;

		if (bHit && Hits.Last().bBlockingHit)
		{
			// Red up to the blocking hit, green thereafter
			FVector const BlockingHitPoint = Hits.Last().Location;
			DrawDebugSweptSphere(World,
				Start,
				BlockingHitPoint,
				Radius,
				TraceColor.ToFColor(true),
				bPersistent,
				LifeTime);
			DrawDebugSweptSphere(World,
				BlockingHitPoint,
				End,
				Radius,
				TraceHitColor.ToFColor(true),
				bPersistent,
				LifeTime);
		}
		else
		{
			// no hit means all red
			DrawDebugSweptSphere(World, Start, End, Radius, TraceColor.ToFColor(true), bPersistent, LifeTime);
		}

		// draw hits
		for (int32 HitIdx = 0; HitIdx < Hits.Num(); ++HitIdx)
		{
			FHitResult const& Hit = Hits[HitIdx];
			::DrawDebugPoint(World,
				Hit.ImpactPoint,
				KISMET_TRACE_DEBUG_IMPACTPOINT_SIZE,
				(Hit.bBlockingHit ? TraceColor.ToFColor(true) : TraceHitColor.ToFColor(true)),
				bPersistent,
				LifeTime);
		}
	}
}

void UGASC_MeleeTrace_Subsystem::DrawDebugSweptSphere(const UWorld* InWorld, FVector const& Start, FVector const& End,
	float Radius, FColor const& Color, bool bPersistentLines, float LifeTime, uint8 DepthPriority)
{
	FVector const TraceVec = End - Start;
	float const Dist = TraceVec.Size();

	FVector const Center = Start + TraceVec * 0.5f;
	float const HalfHeight = (Dist * 0.5f) + Radius;

	FQuat const CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	::DrawDebugCapsule(InWorld,
		Center,
		HalfHeight,
		Radius,
		CapsuleRot,
		Color,
		bPersistentLines,
		LifeTime,
		DepthPriority);
}

void UGASC_MeleeTrace_Subsystem::DrawDebugCapsuleTraceMulti(const UWorld* World, const FVector& Start,
	const FVector& End, const FQuat& Orientation, float Radius, float HalfHeight, EDrawDebugTrace::Type DrawDebugType,
	bool bHit, const TArray<FHitResult>& HitResults, const FLinearColor& TraceColor, const FLinearColor& TraceHitColor,
	float DrawTime)
{
	if (DrawDebugType != EDrawDebugTrace::None)
	{
		const bool bPersistent = DrawDebugType == EDrawDebugTrace::Persistent;
		const float LifeTime = (DrawDebugType == EDrawDebugTrace::ForDuration) ? DrawTime : 0.f;

		if (bHit && HitResults.Last().bBlockingHit)
		{
			// Red up to the blocking hit, green thereafter
			FVector const BlockingHitPoint = HitResults.Last().Location;
			FVector MidPosition = (Start + End) * 0.5f;
			::DrawDebugCapsule(World,
				MidPosition,
				HalfHeight,
				Radius,
				Orientation,
				TraceColor.ToFColor(true),
				bPersistent,
				LifeTime);
			::DrawDebugCapsule(World,
				BlockingHitPoint,
				HalfHeight,
				Radius,
				Orientation,
				TraceColor.ToFColor(true),
				bPersistent,
				LifeTime);
			::DrawDebugLine(World, Start, BlockingHitPoint, TraceColor.ToFColor(true), bPersistent, LifeTime);
			
			::DrawDebugCapsule(World,
				End,
				HalfHeight,
				Radius,
				Orientation,
				TraceHitColor.ToFColor(true),
				bPersistent,
				LifeTime);
			::DrawDebugLine(World, BlockingHitPoint, End, TraceHitColor.ToFColor(true), bPersistent, LifeTime);
		}
		else
		{
			FVector MidPosition = (Start + End) * 0.5f;
			::DrawDebugCapsule(World,
				Start,
				HalfHeight,
				Radius,
				Orientation,
				TraceColor.ToFColor(true),
				bPersistent,
				LifeTime);
			::DrawDebugPoint(World,
				Start,
				KISMET_TRACE_DEBUG_IMPACTPOINT_SIZE,
				TraceColor.ToFColor(true),
				bPersistent,
				LifeTime);
			::DrawDebugPoint(World,
				Start,
				KISMET_TRACE_DEBUG_IMPACTPOINT_SIZE,
				TraceHitColor.ToFColor(true),
				bPersistent,
				LifeTime);
			::DrawDebugPoint(World,
				End,
				KISMET_TRACE_DEBUG_IMPACTPOINT_SIZE,
				TraceHitColor.ToFColor(true),
				bPersistent,
				LifeTime);
			/*
			// no hit means all red
			::DrawDebugCapsule(World,
				Start,
				HalfHeight,
				Radius,
				Orientation,
				TraceColor.ToFColor(true),
				bPersistent,
				LifeTime);
			::DrawDebugCapsule(World,
				End,
				HalfHeight,
				Radius,
				Orientation,
				TraceColor.ToFColor(true),
				bPersistent,
				LifeTime);
				*/
			::DrawDebugLine(World, Start, End, TraceColor.ToFColor(true), bPersistent, LifeTime);
		}

		// draw hits
		for (int32 HitIdx = 0; HitIdx < HitResults.Num(); ++HitIdx)
		{
			FHitResult const& Hit = HitResults[HitIdx];
			::DrawDebugPoint(World,
				Hit.ImpactPoint,
				KISMET_TRACE_DEBUG_IMPACTPOINT_SIZE,
				(Hit.bBlockingHit ? TraceColor.ToFColor(true) : TraceHitColor.ToFColor(true)),
				bPersistent,
				LifeTime);
		}
	}
}

void UGASC_MeleeTrace_Subsystem::DrawDebugBoxTraceMulti(const UWorld* World, const FVector& Start, const FVector& End,
	const FRotator& Orientation, const FVector& BoxExtent, EDrawDebugTrace::Type DrawDebugType, bool bHit,
	const TArray<FHitResult>& HitResults, const FLinearColor& TraceColor, const FLinearColor& TraceHitColor,
	float DrawTime)
{
	if (DrawDebugType != EDrawDebugTrace::None && (World != nullptr))
	{
		const bool bPersistent = DrawDebugType == EDrawDebugTrace::Persistent;
		const float LifeTime = (DrawDebugType == EDrawDebugTrace::ForDuration) ? DrawTime : 0.f;

		if (bHit && HitResults.Last().bBlockingHit)
		{
			// Red up to the blocking hit, green thereafter
			FVector const BlockingHitPoint = HitResults.Last().Location;
			DrawDebugSweptBox(World,
				Start,
				BlockingHitPoint,
				Orientation,
				BoxExtent,
				TraceColor.ToFColor(true),
				bPersistent,
				LifeTime);
			DrawDebugSweptBox(World,
				BlockingHitPoint,
				End,
				Orientation,
				BoxExtent,
				TraceHitColor.ToFColor(true),
				bPersistent,
				LifeTime);
		}
		else
		{
			// no hit means all red
			DrawDebugSweptBox(World,
				Start,
				End,
				Orientation,
				BoxExtent,
				TraceColor.ToFColor(true),
				bPersistent,
				LifeTime);
		}

		// draw hits
		for (int32 HitIdx = 0; HitIdx < HitResults.Num(); ++HitIdx)
		{
			FHitResult const& Hit = HitResults[HitIdx];
			::DrawDebugPoint(World,
				Hit.ImpactPoint,
				KISMET_TRACE_DEBUG_IMPACTPOINT_SIZE,
				(Hit.bBlockingHit ? TraceColor.ToFColor(true) : TraceHitColor.ToFColor(true)),
				bPersistent,
				LifeTime);
		}
	}
}

void UGASC_MeleeTrace_Subsystem::DrawDebugSweptBox(const UWorld* InWorld, FVector const& Start, FVector const& End,
	FRotator const& Orientation, FVector const& HalfSize, FColor const& Color, bool bPersistentLines, float LifeTime,
	uint8 DepthPriority)
{
	FVector const TraceVec = End - Start;
	FQuat const CapsuleRot = Orientation.Quaternion();
	::DrawDebugBox(InWorld, Start, HalfSize, CapsuleRot, Color, bPersistentLines, LifeTime, DepthPriority);

	//now draw lines from vertices
	FVector Vertices[8];
	Vertices[0] = Start + CapsuleRot.RotateVector(FVector(-HalfSize.X, -HalfSize.Y, -HalfSize.Z)); //flt
	Vertices[1] = Start + CapsuleRot.RotateVector(FVector(-HalfSize.X, HalfSize.Y, -HalfSize.Z));  //frt
	Vertices[2] = Start + CapsuleRot.RotateVector(FVector(-HalfSize.X, -HalfSize.Y, HalfSize.Z));  //flb
	Vertices[3] = Start + CapsuleRot.RotateVector(FVector(-HalfSize.X, HalfSize.Y, HalfSize.Z));   //frb
	Vertices[4] = Start + CapsuleRot.RotateVector(FVector(HalfSize.X, -HalfSize.Y, -HalfSize.Z));  //blt
	Vertices[5] = Start + CapsuleRot.RotateVector(FVector(HalfSize.X, HalfSize.Y, -HalfSize.Z));   //brt
	Vertices[6] = Start + CapsuleRot.RotateVector(FVector(HalfSize.X, -HalfSize.Y, HalfSize.Z));   //blb
	Vertices[7] = Start + CapsuleRot.RotateVector(FVector(HalfSize.X, HalfSize.Y, HalfSize.Z));    //brb
	for (int32 VertexIdx = 0; VertexIdx < 8; ++VertexIdx)
	{
		::DrawDebugLine(InWorld,
			Vertices[VertexIdx],
			Vertices[VertexIdx] + TraceVec,
			Color,
			bPersistentLines,
			LifeTime,
			DepthPriority);
	}

	::DrawDebugBox(InWorld, End, HalfSize, CapsuleRot, Color, bPersistentLines, LifeTime, DepthPriority);
}

void UGASC_MeleeTrace_Subsystem::RequestShapeMeleeTrace(AActor* Instigator, FGASC_MeleeTrace_Subsystem_Data TraceData, FGuid TraceId)
{
	FGASC_MeleeTrace_Subsystem_Data NewMeleeTraceRequest;
	NewMeleeTraceRequest = TraceData;
	NewMeleeTraceRequest.TraceId = TraceId;
	NewMeleeTraceRequest.InstigatorActor = Instigator;
	NewMeleeTraceRequest.TraceCollisionShape = NewMeleeTraceRequest.TraceShape->CreateCollisionShape();
	NewMeleeTraceRequest.SourceMeshComponent = GetMeshComponent(Instigator, NewMeleeTraceRequest.TraceSocket_Start,
		NewMeleeTraceRequest.TraceSocket_End);

	GetTraceSamples(NewMeleeTraceRequest.SourceMeshComponent.Get(), TraceData.TraceDensity,
		NewMeleeTraceRequest.TraceSocket_Start,
		NewMeleeTraceRequest.TraceSocket_End,
		NewMeleeTraceRequest.PreviousFrameSamples);
	
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
	const bool bShouldDrawDebug = GASCourse_MeleeSubSystemCVars::CvarEnableMeleeTracesDebug.GetValueOnGameThread();
	const UGASC_MeleeSubsystem_Settings* MeleeTraceSettings = GetDefault<UGASC_MeleeSubsystem_Settings>();
	check(MeleeTraceSettings);

	FCollisionQueryParams QueryParams;
	QueryParams.bReturnPhysicalMaterial = true;
	QueryParams.bReturnFaceIndex = true;
	
	FCollisionObjectQueryParams ObjectParams = ConfigureCollisionObjectParams(MeleeTraceSettings->CollisionObjectTypes);

	for (FGASC_MeleeTrace_Subsystem_Data& ActiveMeleeTraceRequest : MeleeTraceRequests)
	{
		QueryParams.AddIgnoredActor(ActiveMeleeTraceRequest.InstigatorActor);
		TArray<FHitResult> HitResults;
		TArray<FVector> TraceSamples;
		GetTraceSamples(ActiveMeleeTraceRequest.SourceMeshComponent.Get(),
			ActiveMeleeTraceRequest.TraceDensity,
			ActiveMeleeTraceRequest.TraceSocket_Start,
			ActiveMeleeTraceRequest.TraceSocket_End,
			TraceSamples);

		//const FQuat OffsetRotation = ActiveMeleeTraceRequest.SourceMeshComponent->GetSocketRotation(ActiveMeleeTraceRequest.TraceSocket_Start).Quaternion();
		FVector Direction = UKismetMathLibrary::GetDirectionUnitVector(ActiveMeleeTraceRequest.SourceMeshComponent->GetSocketLocation(ActiveMeleeTraceRequest.TraceSocket_Start),
	ActiveMeleeTraceRequest.SourceMeshComponent->GetSocketLocation(ActiveMeleeTraceRequest.TraceSocket_End));
		const FQuat Rotation = FQuat::FindBetweenVectors(FVector::UpVector, Direction);

		for (int32 Index = 0; Index < TraceSamples.Num(); Index++)
		{
			HitResults.Reset();
			FVector PreviousSample = ActiveMeleeTraceRequest.PreviousFrameSamples[Index];
			if (PreviousSample.Equals(TraceSamples[Index]))
			{
				PreviousSample += FVector::UpVector * UE_KINDA_SMALL_NUMBER;
			}
			const bool bHit = GetWorld()->SweepMultiByObjectType(HitResults,
					PreviousSample, TraceSamples[Index],
						Rotation,
						ObjectParams,
						ActiveMeleeTraceRequest.TraceCollisionShape,
						QueryParams);

			if (bShouldDrawDebug)
			{
				DrawDebugMeleeTrace(ActiveMeleeTraceRequest.InstigatorActor,
				ActiveMeleeTraceRequest.TraceCollisionShape,
						FTransform(Rotation, ActiveMeleeTraceRequest.PreviousFrameSamples[Index]),
						FTransform(Rotation, TraceSamples[Index]),
						bHit,
						HitResults);
			}

				if (!bHit)
				{
					continue;
				}

				for (const FHitResult& Hit : HitResults)
				{
					ActiveMeleeTraceRequest.HitResults_PreviousFrames.Add(Hit);
				}
			}
		ActiveMeleeTraceRequest.PreviousFrameSamples = MoveTemp(TraceSamples);

		for (FHitResult Hit : ActiveMeleeTraceRequest.HitResults_PreviousFrames)
		{
			if (ActiveMeleeTraceRequest.HitActors_PreviousFrames.Contains(Hit.GetActor()))
			{
				return;
			}

			ActiveMeleeTraceRequest.HitActors_PreviousFrames.Add(Hit.GetActor());
			if (AGASCourseCharacter* InstigatorCharacter = Cast<AGASCourseCharacter>(ActiveMeleeTraceRequest.InstigatorActor))
			{
				FGameplayEventData OnHitPayload;
				OnHitPayload.Instigator = InstigatorCharacter;
				OnHitPayload.Target = Hit.GetActor();
				OnHitPayload.TargetData = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromHitResult(Hit);
				InstigatorCharacter->GetAbilitySystemComponent()->HandleGameplayEvent(Event_Gameplay_OnHit, &OnHitPayload);
			}
		}
	}
}

FCollisionObjectQueryParams UGASC_MeleeTrace_Subsystem::ConfigureCollisionObjectParams(
	const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes)
{
	TArray<TEnumAsByte<ECollisionChannel>> CollisionObjectTraces;
	CollisionObjectTraces.AddUninitialized(ObjectTypes.Num());

	for (auto Iter = ObjectTypes.CreateConstIterator(); Iter; ++Iter)
	{
		CollisionObjectTraces[Iter.GetIndex()] = UEngineTypes::ConvertToCollisionChannel(*Iter);
	}

	FCollisionObjectQueryParams ObjectParams;
	for (auto Iter = CollisionObjectTraces.CreateConstIterator(); Iter; ++Iter)
	{
		const ECollisionChannel & Channel = (*Iter);
		if (FCollisionObjectQueryParams::IsValidObjectQuery(Channel))
		{
			ObjectParams.AddObjectTypesToQuery(Channel);
		}
		else
		{
			UE_LOG(LogBlueprintUserMessages, Warning, TEXT("%d isn't valid object type"), (int32)Channel);
		}
	}

	return ObjectParams;
}

void UGASC_MeleeTrace_Subsystem::GetTraceSamples(const UMeshComponent* MeshComponent, int32 TraceDensity,
                                                 const FName& StartSocketName, const FName& EndSocketName, TArray<FVector>& OutSamples)
{
	OutSamples.Reset(TraceDensity + 1);
	TraceDensity = FMath::Max(TraceDensity, 1);
	const FVector StartSampleLocation = MeshComponent->GetSocketLocation(StartSocketName);
	const FVector EndSampleLocation = MeshComponent->GetSocketLocation(EndSocketName);
	for (int32 Index = 0; Index <= TraceDensity; Index++)
	{
		const float Alpha = static_cast<float>(Index) / static_cast<float>(TraceDensity);
		const FVector Sample = FMath::Lerp(StartSampleLocation, EndSampleLocation, Alpha);
		UE_LOG(LOG_GASC_MeleeTraceSubsystem, Log, TEXT("Sample: %s"), *Sample.ToString());
		OutSamples.Add(Sample);
	}
}

TWeakObjectPtr<UMeshComponent> UGASC_MeleeTrace_Subsystem::GetMeshComponent(AActor* Actor, const FName& StartSocketName, const FName& EndSocketName)
{
	if (Actor == nullptr)
	{
		UE_LOG(LOG_GASC_MeleeTraceSubsystem, Warning, TEXT("Actor is NULL!"));
		return nullptr;
	}

	if (USkeletalMeshComponent* SkeletalMeshComponent = Actor->FindComponentByClass<USkeletalMeshComponent>())
	{
		if (SkeletalMeshComponent->DoesSocketExist(StartSocketName) && SkeletalMeshComponent->DoesSocketExist(EndSocketName))
		{
			return SkeletalMeshComponent;
		}
		else
		{
			TArray<USceneComponent*> ChildrenComponents;
			SkeletalMeshComponent->GetChildrenComponents(true, ChildrenComponents);
			for (USceneComponent* ChildComponent : ChildrenComponents)
			{
				if (auto* TypedMeshComponent = Cast<UMeshComponent>(ChildComponent))
				{
					if (TypedMeshComponent->DoesSocketExist(StartSocketName)
						&& TypedMeshComponent->DoesSocketExist(EndSocketName))
					{
						return TypedMeshComponent;
					}
				}
			}
		}
	}
	else
	{
		UE_LOG(LOG_GASC_MeleeTraceSubsystem, Warning, TEXT("No Skeletal Mesh Component found!"));
		return nullptr;
	}

	return nullptr;
}

FGASC_MeleeTrace_Subsystem_Data UGASC_MeleeTrace_Subsystem::CreateShapeDataFromRow(
	FGASC_MeleeTrace_TraceShapeData RowData)
{
	FGASC_MeleeTrace_Subsystem_Data TraceData;
	TraceData.TraceDensity = RowData.TraceDensity;
	TraceData.TraceSocket_Start = RowData.StartSocket;
	TraceData.TraceSocket_End = RowData.EndSocket;
	TraceData.HitActors.Reset();
	TraceData.PreviousFrameSamples.Reset();
	TraceData.TraceId = FGuid::NewGuid();
	TraceData.InstigatorActor = nullptr;
	TraceData.SourceMeshComponent = nullptr;
	
	switch (RowData.TraceShape)
	{
	case EGASC_MeleeTrace_TraceShape::Box:
		// Handle Box Trace Shape
			TraceData.TraceShape = NewObject<UGASC_MeleeShape_Box>(GetOuter());
		if (UGASC_MeleeShape_Box* Box = Cast<UGASC_MeleeShape_Box>(TraceData.TraceShape))
		{
			Box->BoxExtent = RowData.BoxExtent;
			break;
		}
		else
		{
			break;
		}

	case EGASC_MeleeTrace_TraceShape::Capsule:
		// Handle Capsule Trace Shape
			TraceData.TraceShape = NewObject<UGASC_MeleeShape_Capsule>(GetOuter());
		if (UGASC_MeleeShape_Capsule* Capsule = Cast<UGASC_MeleeShape_Capsule>(TraceData.TraceShape))
		{
			Capsule->Radius = RowData.CapsuleRadius;
			Capsule->HalfHeight = RowData.CapsuleHeight;
			break;
		}
		else
		{
			break;
		}

	case EGASC_MeleeTrace_TraceShape::Sphere:
		// Handle Sphere Trace Shape
			TraceData.TraceShape = NewObject<UGASC_MeleeShape_Sphere>(GetOuter());
		if (UGASC_MeleeShape_Sphere* Sphere = Cast<UGASC_MeleeShape_Sphere>(TraceData.TraceShape))
		{
			Sphere->Radius = RowData.SphereRadius;
			break;
		}
		else
		{
			break;
		}

	default:
		// Fallback case (optional, depending on your enum design)
			UE_LOG(LOG_GASC_MeleeTraceSubsystem, Warning, TEXT("Unknown Trace Shape!"));
		break;

	}
	return TraceData;
}