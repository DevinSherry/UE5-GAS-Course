// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Systems/Subsystems/MeleeTrace/GASC_MeleeTrace_NotifyState.h"
#include "Game/Systems/Subsystems/MeleeTrace/GASC_MeleeTrace_Subsystem.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"

void UGASC_MeleeTrace_NotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                               float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	if (UWorld* NotifyWorld = MeshComp->GetOwner()->GetWorld())
	{
		UGASC_MeleeTrace_Subsystem* MeleeTrace_Subsystem = NotifyWorld->GetSubsystem<UGASC_MeleeTrace_Subsystem>();
		if (MeleeTrace_Subsystem)
		{
			MeleeTraceNotifyID = FGuid::NewGuid();
			if (MeleeTraceNotifyID.IsValid() && !MeleeTrace_Subsystem->IsMeleeTraceInProgress(MeleeTraceNotifyID))
			{
				if (MeleeTraceRowHandle.DataTable != nullptr)
				{
					if (FGASC_MeleeTrace_TraceShapeData* RowData = MeleeTraceRowHandle.GetRow<FGASC_MeleeTrace_TraceShapeData>("Context"))
					{
						MeleeTraceSubsystemData = MeleeTrace_Subsystem->CreateShapeDataFromRow(*RowData);
						MeleeTrace_Subsystem->RequestShapeMeleeTrace(MeshComp->GetOwner(), MeleeTraceSubsystemData, MeleeTraceNotifyID);
					}
				}
			}
		}
	}
	
#if WITH_EDITOR
	if (GEditor && ShouldFireInEditor())
	{
		if (MeleeTraceRowHandle.DataTable != nullptr)
		{
			if (FGASC_MeleeTrace_TraceShapeData* RowData = MeleeTraceRowHandle.GetRow<FGASC_MeleeTrace_TraceShapeData>("Context"))
			{
				FName StartSocket = RowData->StartSocket;
				FName EndSocket = RowData->EndSocket;
				MeleeTraceSubsystemData = CreateShapeDataFromRow(*RowData);
				if (MeshComp->DoesSocketExist(StartSocket) && MeshComp->DoesSocketExist(EndSocket))
				{
					DebugMeshComponent = MeshComp;
				}
				else
				{
					TArray<USceneComponent*> ChildrenComponents;
					MeshComp->GetChildrenComponents(true, ChildrenComponents);
					for (USceneComponent* ChildComponent : ChildrenComponents)
					{
						if (auto* TypedMeshComponent = Cast<UMeshComponent>(ChildComponent))
						{
							if (TypedMeshComponent->DoesSocketExist(StartSocket)
								&& TypedMeshComponent->DoesSocketExist(EndSocket))
							{
								DebugMeshComponent = TypedMeshComponent;
							}
						}
					}
				}
			}
		}
	}
#endif
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
}

void UGASC_MeleeTrace_NotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
#if WITH_EDITOR
	if (!DebugMeshComponent.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("DebugMeshComponent is not valid, exit."));
		return;
	}
	if (GEditor)
	{
		UWorld* EditorWorld = MeshComp->GetWorld();
		if (EditorWorld && EditorWorld->WorldType != EWorldType::EditorPreview)
		{
			return;
		}
		UGASC_MeleeTrace_Subsystem* MeleeTrace_Subsystem = EditorWorld->GetSubsystem<UGASC_MeleeTrace_Subsystem>();

		FCollisionShape CollisionShape;
		if (MeleeTraceSubsystemData.TraceShape)
		{
			CollisionShape = MeleeTraceSubsystemData.TraceShape->CreateCollisionShape();
			if (UGASC_MeleeShape_Capsule* Capsule = Cast<UGASC_MeleeShape_Capsule>(MeleeTraceSubsystemData.TraceShape))
			{
				Capsule->HalfHeight = UKismetMathLibrary::Vector_Distance(DebugMeshComponent->GetSocketLocation(MeleeTraceSubsystemData.TraceSocket_Start),
				DebugMeshComponent->GetSocketLocation(MeleeTraceSubsystemData.TraceSocket_End)) * 0.5f;
			}
		}
		TArray<FVector> Samples;
		FVector Direction = UKismetMathLibrary::GetDirectionUnitVector(DebugMeshComponent->GetSocketLocation(MeleeTraceSubsystemData.TraceSocket_Start),
			DebugMeshComponent->GetSocketLocation(MeleeTraceSubsystemData.TraceSocket_End));
		const FQuat Rotation = FQuat::FindBetweenVectors(FVector::UpVector, Direction);
		UGASC_MeleeTrace_Subsystem::GetTraceSamples(DebugMeshComponent.Get(),
			MeleeTraceSubsystemData.TraceDensity,
			MeleeTraceSubsystemData.TraceSocket_Start,
			MeleeTraceSubsystemData.TraceSocket_End,
			Samples);

		if (PreviousFrameSamples.Num() == Samples.Num())
		{
			for (int32 i = 0; i < Samples.Num(); i++)
			{
				MeleeTrace_Subsystem->DrawDebugMeleeTrace(MeshComp->GetWorld(),
							CollisionShape,
							FTransform(Rotation, PreviousFrameSamples[i]),
							FTransform(Rotation, Samples[i]),
							false,
							{});
			}
		}
		else
		{
			MeleeTrace_Subsystem->DrawDebugMeleeTrace(MeshComp->GetWorld(),
							CollisionShape,
							FTransform(Rotation, DebugMeshComponent->GetSocketLocation(MeleeTraceSubsystemData.TraceSocket_Start)),
							FTransform(Rotation, DebugMeshComponent->GetSocketLocation(MeleeTraceSubsystemData.TraceSocket_End)),
							false,
							{});
		}
		PreviousFrameSamples = MoveTemp(Samples);
	}
#endif

	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
}

void UGASC_MeleeTrace_NotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	if (UWorld* NotifyWorld = MeshComp->GetOwner()->GetWorld())
	{
		UGASC_MeleeTrace_Subsystem* MeleeTrace_Subsystem = NotifyWorld->GetSubsystem<UGASC_MeleeTrace_Subsystem>();
		if (MeleeTrace_Subsystem)
		{
			if (MeleeTraceNotifyID.IsValid() && MeleeTrace_Subsystem->IsMeleeTraceInProgress(MeleeTraceNotifyID))
			{
				MeleeTrace_Subsystem->CancelMeleeTrace(MeleeTraceNotifyID);
			}
		}
	}

#if WITH_EDITOR
	DebugMeshComponent.Reset();
	PreviousFrameSamples.Reset();
#endif
	Super::NotifyEnd(MeshComp, Animation, EventReference);
}

FGASC_MeleeTrace_Subsystem_Data UGASC_MeleeTrace_NotifyState::CreateShapeDataFromRow(
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
