// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Systems/Targeting/AreaofEffect/GASC_AreaOfEffectData.h"
#include "Game/Systems/Targeting/AreaofEffect/GASC_TargetingSelectionTask_AOE.h"
#include "Game/BlueprintLibraries/GameplayAbilitySystem/GASCourseASCBlueprintLibrary.h"
#include "Game/BlueprintLibraries/TargetingSystem/GASCourseTargetingLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Misc/DataValidation.h"
#include "TargetingSystem/TargetingSubsystem.h"
#include "Tasks/TargetingTask.h"
#include "Types/TargetingSystemTypes.h"

bool UGASC_AreaOfEffectData::ProcessAreaOfEffect(AActor* Instigator, const FVector& Center)
{
	if(UTargetingSubsystem* TargetingSubsystem = UTargetingSubsystem::Get(Instigator->GetWorld()))
	{
		AreaOfEffectInstigatorActor = Instigator;
		FTargetingSourceContext TargetingSourceContext;
		TargetingSourceContext.InstigatorActor = Instigator;
		TargetingSourceContext.SourceLocation = Center;
		FTargetingRequestDelegate OnCompletedDelegate;
		OnCompletedDelegate.BindUFunction(this, FName("OnTargetRequestCompleted"));
		CurrentTargetHandle = UTargetingSubsystem::MakeTargetRequestHandle(TargetingPreset, TargetingSourceContext);
		FTargetingAsyncTaskData& AsyncTaskData = FTargetingAsyncTaskData::FindOrAdd(CurrentTargetHandle);
		AsyncTaskData.bReleaseOnCompletion = true;
		TargetingSubsystem->StartAsyncTargetingRequestWithHandle(CurrentTargetHandle, OnCompletedDelegate);
		return true;
	}

	return false;
}
#if WITH_EDITOR
EDataValidationResult UGASC_AreaOfEffectData::IsDataValid(class FDataValidationContext& Context) const
{
	EDataValidationResult Result = EDataValidationResult::Valid;
	if (!TargetingPreset)
	{
		Result = EDataValidationResult::Invalid;
		Context.AddError(FText::FromString("Targeting Preset is not set"));
	}

	return Result;
}
#endif // WITH_EDITOR

void UGASC_AreaOfEffectData::OnTargetRequestCompleted(FTargetingRequestHandle TargetingRequestHandle)
{
	if (AreaOfEffectInstigatorActor)
	{
		if(UTargetingSubsystem* TargetingSubsystem = UTargetingSubsystem::Get(AreaOfEffectInstigatorActor->GetWorld()))
		{
			TArray<AActor*> FoundTargets;
			TargetingSubsystem->GetTargetingResultsActors(TargetingRequestHandle, FoundTargets);
			
			if (bDebugDrawAreaOfEffect)
			{
				if (const UGASC_TargetingSelectionTask_AOE* AoETask = Cast<UGASC_TargetingSelectionTask_AOE>(GetAOESelectionTask(TargetingPreset)))
				{
					DrawAoEDebugShape(AoETask, TargetingRequestHandle, AoETask->GetCollisionShapeFromTask());
				}
			}
			
			if(FoundTargets.IsEmpty())
			{
				return;
			}
			
			ApplyAreaOfEffectOnActors(AreaOfEffectInstigatorActor, FoundTargets, TargetingRequestHandle);
			TargetingSubsystem->RemoveAsyncTargetingRequestWithHandle(TargetingRequestHandle);
		}
	}
}

void UGASC_AreaOfEffectData::DrawAoEDebugShape(const UTargetingTask* InAoETask, const FTargetingRequestHandle& TargetingHandle, FCollisionShape InAoEShape)
{
	const UGASC_TargetingSelectionTask_AOE* AoeTask = Cast<UGASC_TargetingSelectionTask_AOE>(InAoETask);
	if (!AoeTask)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid AoE Task"));
	}
	const UWorld* World = GetWorldFromTargetHandle(TargetingHandle);
	const FVector SourceLocation = GetSourceAoELocation(TargetingHandle) + AoeTask->GetAoESourceOffset(TargetingHandle);
	const FQuat SourceRotation = GetSourceAoERotation(TargetingHandle);

	constexpr bool bPersistentLines = false;
	const float LifeTime = AoEDebugShapeDuration;
	constexpr uint8 DepthPriority = 0;
	constexpr float Thickness = 1.0f;
	
	switch (ETargetingAOEShape Shape = AoeTask->GetTargetingAOEShape())
	{
	case ETargetingAOEShape::Box:
		DrawDebugBox(World, SourceLocation, InAoEShape.GetExtent(), SourceRotation,
			AoEDebugShapeColor, bPersistentLines, LifeTime, DepthPriority, Thickness);
		break;
	case ETargetingAOEShape::Sphere:
		DrawDebugSphere(World, SourceLocation, InAoEShape.GetSphereRadius(),
16, FColor::Green, false, 3.0f, 0, 2.0f);
		break;
	case ETargetingAOEShape::Capsule:
		DrawDebugCapsule(World, SourceLocation, InAoEShape.GetCapsuleHalfHeight(), InAoEShape.GetCapsuleRadius(), SourceRotation,
			AoEDebugShapeColor, bPersistentLines, LifeTime, DepthPriority, Thickness);
		break;
	case ETargetingAOEShape::Cylinder:
		{
			const FVector RotatedExtent = SourceRotation * InAoEShape.GetExtent();
			DrawDebugCylinder(World, SourceLocation - RotatedExtent, SourceLocation + RotatedExtent, InAoEShape.GetExtent().X, 32,
				AoEDebugShapeColor, bPersistentLines, LifeTime, DepthPriority, Thickness);
			break;
		}
	default:
		UE_LOG(LogTemp, Warning, TEXT("Unknown AoE Shape"));
		break;
	}
}

const UWorld* UGASC_AreaOfEffectData::GetWorldFromTargetHandle(const FTargetingRequestHandle& TargetingHandle)
{
	if (const FTargetingSourceContext* SourceContext = FTargetingSourceContext::Find(TargetingHandle))
	{
		if (SourceContext->SourceActor)
		{
			return SourceContext->SourceActor->GetWorld();
		}

		if (SourceContext->InstigatorActor)
		{
			return SourceContext->InstigatorActor->GetWorld();
		}
	}

	return GetWorld();
}


FVector UGASC_AreaOfEffectData::GetSourceAoELocation(const FTargetingRequestHandle& TargetingHandle)
{
	if (const FTargetingSourceContext* SourceContext = FTargetingSourceContext::Find(TargetingHandle))
	{
		return SourceContext->SourceLocation;
	}

	return FVector::ZeroVector;
}

FQuat UGASC_AreaOfEffectData::GetSourceAoERotation(const FTargetingRequestHandle& TargetingHandle) const
{
	if (const FTargetingSourceContext* SourceContext = FTargetingSourceContext::Find(TargetingHandle))
	{
		if (SourceContext->SourceActor)
		{
			return SourceContext->SourceActor->GetActorQuat();
		}
	}

	return FQuat::Identity;
}

bool UGASC_AreaOfEffectData::ApplyAreaOfEffectOnActors(AActor* InInstigator, TArray<AActor*> TargetActors, FTargetingRequestHandle TargetingRequestHandle)
{
	float AreaOfEffectValue = 0.0f;
	for (AActor* TargetActor : TargetActors)
	{
		if (AreaOfEffectType == EGASC_AreaOfEffectType::Damage)
		{
			if (UGASCourseTargetingLibrary::AreActorsOnSameTeam(InInstigator, TargetActor) && !bCanDamageAllies)
			{
				break;
			}
			AreaOfEffectValue = ProcessAreaOfEffectValueFallOffOnTarget(TargetingRequestHandle, TargetActor);
			UGASCourseASCBlueprintLibrary::ApplyDamageToTarget(TargetActor, InInstigator, AreaOfEffectValue, AreaOfEffectDamageContext);
		}
		else if (AreaOfEffectType == EGASC_AreaOfEffectType::Healing)
		{
			if (!UGASCourseTargetingLibrary::AreActorsOnSameTeam(InInstigator, TargetActor) && !bCanHealEnemies)
			{
				break;
			}
			AreaOfEffectValue = ProcessAreaOfEffectValueFallOffOnTarget(TargetingRequestHandle, TargetActor);
			UGASCourseASCBlueprintLibrary::ApplyHealingToTarget(TargetActor, InInstigator, AreaOfEffectValue);
		}
	}

	return true;
}

float UGASC_AreaOfEffectData::ProcessAreaOfEffectValueFallOffOnTarget(FTargetingRequestHandle TargetingRequestHandle, const AActor* InTargetActor)
{
	FVector AreaOfEffectCenter = GetSourceAoELocation(TargetingRequestHandle);
	FVector TargetActorLocation = InTargetActor->GetActorLocation();
	
	float Distance = UKismetMathLibrary::Vector_Distance2D(AreaOfEffectCenter, TargetActorLocation);
	float MaxDistance = GetMaxDistanceFromCenter();
	float NormalizedDistance = UKismetMathLibrary::SafeDivide(Distance, MaxDistance);
	float ProcessedAreaOfEffectValue = AreaOfEffectFallOffCurve.EditorCurveData.Eval(NormalizedDistance);

	float FinalAreaOfEffectValue = FMath::Clamp((ProcessedAreaOfEffectValue * AreaOfEffectMaxValue.GetValue()),
		AreaOfEffectMinValue.GetValue(), AreaOfEffectMaxValue.GetValue());

	if (bDrawNumericalValueOfEffectOnActor)
	{
		const FString NumericalValueToString = FString::SanitizeFloat(FinalAreaOfEffectValue);
		const FColor NumericalValueTextColor = (AreaOfEffectType == EGASC_AreaOfEffectType::Damage) ? FColor::Red : FColor::Green;
		DrawDebugString(AreaOfEffectInstigatorActor->GetWorld(), FVector(0.0f, 0.0f, 100.0f), NumericalValueToString, const_cast<AActor*>(InTargetActor),
			NumericalValueTextColor, 3.0f, false, 2.0f);
	}

	if (bDrawAffectedActors)
	{
		FVector ActorOrigin(0.0f);
		FVector ActorExtents(0.0f);
		InTargetActor->GetActorBounds(true, ActorOrigin, ActorExtents, false);
		DrawDebugBox(InTargetActor->GetWorld(), InTargetActor->GetActorLocation(), ActorExtents, FColor::Red, false, 5.0f, 0, 2.0f);
	}

	return FinalAreaOfEffectValue;
}

const UGASC_TargetingSelectionTask_AOE* UGASC_AreaOfEffectData::GetAOESelectionTask(const UTargetingPreset* InTargetingPreset)
{
	if (const FTargetingTaskSet* TargetingTaskSet = InTargetingPreset->GetTargetingTaskSet())
	{
		for (UTargetingTask* Task : TargetingTaskSet->Tasks)
		{
			if (const UGASC_TargetingSelectionTask_AOE* AoETask = Cast<UGASC_TargetingSelectionTask_AOE>(Task))
			{
				return AoETask;
			}
		}
	}

	return nullptr;
}

float UGASC_AreaOfEffectData::GetMaxDistanceFromCenter()
{
	if (const UGASC_TargetingSelectionTask_AOE* AoETask = Cast<UGASC_TargetingSelectionTask_AOE>(GetAOESelectionTask(TargetingPreset)))
	{
		FCollisionShape InAoEShape = AoETask->GetCollisionShapeFromTask();
		
		switch (ETargetingAOEShape Shape = AoETask->GetTargetingAOEShape())
		{
		case ETargetingAOEShape::Box:
			return InAoEShape.GetExtent().Length();
		case ETargetingAOEShape::Sphere:
			return InAoEShape.GetSphereRadius();
		case ETargetingAOEShape::Capsule:
			return InAoEShape.GetCapsuleRadius();
		case ETargetingAOEShape::Cylinder:
			{
				return InAoEShape.GetExtent().X;
			}
		default:
			UE_LOG(LogTemp, Warning, TEXT("Unknown AoE Shape"));
			break;
		}
	}
	
	return 0.0f;
}
