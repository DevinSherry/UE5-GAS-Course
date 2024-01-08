// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GameplayAbilitySystem/Tasks/AbilityTargetActor/GASCourseTargetActor_CameraTrace.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GASCourseGameplayAbility.h"
#include "Abilities/GameplayAbility.h"
#include "GASCourse/GASCourse.h"
#include "Game/Character/Player/GASCoursePlayerController.h"
#include "GameplayAbilities/Aimcast/GASCourseAimcastGameplayAbility.h"

AGASCourseTargetActor_CameraTrace::AGASCourseTargetActor_CameraTrace(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	CollisionRadius = 50.0f;
	CollisionHeight = 50.0f;

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PrePhysics;
	ShouldProduceTargetDataOnServer = false;
	SetReplicates(true);
}

void AGASCourseTargetActor_CameraTrace::StartTargeting(UGameplayAbility* InAbility)
{
	Super::StartTargeting(InAbility);
}

void AGASCourseTargetActor_CameraTrace::ConfirmTargetingAndContinue()
{
	check(ShouldProduceTargetData());
	if (SourceActor)
	{
		const FVector Origin = PerformTrace(SourceActor).Location;
		FGameplayAbilityTargetDataHandle Handle = MakeTargetData(PerformOverlap(Origin), Origin);
		TargetDataReadyDelegate.Broadcast(Handle);
		SendTargetDataBacktoServer(Handle, FGameplayTag());
		Super::ConfirmTargetingAndContinue();
	}
}

void AGASCourseTargetActor_CameraTrace::ConfirmTargeting()
{
	check(ShouldProduceTargetData());
	if (SourceActor)
	{
		const FVector Origin = PerformTrace(SourceActor).Location;
		const FGameplayAbilityTargetDataHandle Handle = MakeTargetData(PerformOverlap(Origin), Origin);
		TargetDataReadyDelegate.Broadcast(Handle);
		SendTargetDataBacktoServer(Handle, FGameplayTag());
		Super::ConfirmTargeting();
	}
}

void AGASCourseTargetActor_CameraTrace::CancelTargeting()
{
	
	if(SourceActor)
	{
		const FVector Origin = PerformTrace(SourceActor).Location;
		const FGameplayAbilityTargetDataHandle Handle = MakeTargetData(PerformOverlap(Origin), Origin);
		SendCancelledTargetDataBackToServer(Handle);
		Super::CancelTargeting();
	}
}

void AGASCourseTargetActor_CameraTrace::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (SourceActor && SourceActor->GetLocalRole() != ENetRole::ROLE_SimulatedProxy)
	{
		const FHitResult HitResult = PerformTrace(SourceActor);
		const FVector EndPoint = HitResult.Component.IsValid() ? HitResult.ImpactPoint : HitResult.TraceEnd;

		if(TargetOutlineData.bEnableTargetingOutline)
		{
			DrawTargetOutline(ActorsToOutline, PerformOverlap(EndPoint));
			ActorsToOutline = PerformOverlap(EndPoint);
		}
	}
}

void AGASCourseTargetActor_CameraTrace::SendTargetDataBacktoServer(const FGameplayAbilityTargetDataHandle& InData,
	FGameplayTag ApplicationTag)
{
	if(bHasDataBeenSentToServer)
	{
		return;
	}
	
	if(UGASCourseAimcastGameplayAbility* LocalAbility = Cast<UGASCourseAimcastGameplayAbility>(OwningAbility))
	{
		LocalAbility->OnTargetDataReadyCallback(InData, ApplicationTag);
		bHasDataBeenSentToServer = true;
	}
	Super::SendTargetDataBacktoServer(InData, ApplicationTag);
}

void AGASCourseTargetActor_CameraTrace::SendCancelledTargetDataBackToServer(
	const FGameplayAbilityTargetDataHandle& InData)
{
	if(bHasDataBeenSentToServer)
	{
		return;
	}
	
	if(UGASCourseAimcastGameplayAbility* LocalAbility = Cast<UGASCourseAimcastGameplayAbility>(OwningAbility))
	{
		LocalAbility->OnTargetDataCancelledCallback(InData);
		bHasDataBeenSentToServer = true;
	}
	Super::SendCancelledTargetDataBackToServer(InData);
}

FHitResult AGASCourseTargetActor_CameraTrace::PerformTrace(AActor* InSourceActor)
{
	
	bool bTraceComplex = false;

	FCollisionQueryParams Params(SCENE_QUERY_STAT(AGASCourseTargetActor_CameraTrace), bTraceComplex);
	Params.bReturnPhysicalMaterial = true;
	Params.AddIgnoredActor(InSourceActor);
	UWorld *ThisWorld = GetWorld();
	FHitResult ReturnHitResult;
	
	AGASCoursePlayerController* PC = Cast<AGASCoursePlayerController>(OwningAbility->GetCurrentActorInfo()->PlayerController.Get());
	check(PC);
	
	FVector TraceStart = PC->MousePositionDeprojectedToWorld;
	FVector TraceEnd = TraceStart + PC->MouseDirectionDeprojectedToWorld * MaxRange;

	bLastTraceWasGood = false;
	
	LineTraceWithFilter(ReturnHitResult, InSourceActor->GetWorld(), Filter, TraceStart, TraceEnd, TraceChannel, Params);
	//Default to end of trace line if we don't hit anything.
	if (ReturnHitResult.bBlockingHit)
	{
		TraceEnd = ReturnHitResult.Location;
		bLastTraceWasGood = true;
	}

#if ENABLE_DRAW_DEBUG
	if (bDebug)
	{
		const FVector CylinderHeight = (ReturnHitResult.Normal * CollisionHeight);
		DrawDebugCylinder(ThisWorld, TraceEnd, TraceEnd + CylinderHeight, CollisionRadius, 10, FColor::Red, false, 1.0f, 0, 2.0f);
		DrawDebugLine(GetWorld(), ReturnHitResult.Location, ReturnHitResult.Location + (ReturnHitResult.Normal * 500.0f), FColor::Blue, true);
	}
#endif	
	
	if (AGameplayAbilityWorldReticle* LocalReticleActor = ReticleActor.Get())
	{
		LocalReticleActor->SetIsTargetValid(bLastTraceWasGood);
		LocalReticleActor->SetActorLocation(ReturnHitResult.Location);
		UE_LOG(LogTemp, Warning, TEXT("%s"), *ReturnHitResult.Location.ToString());
		LocalReticleActor->SetActorScale3D(ReticleParams.AOEScale);
		FRotator LocalReticleRot = ReturnHitResult.Normal.Rotation();
		LocalReticleActor->SetActorRotation(LocalReticleRot);
	}

	return ReturnHitResult;
}

bool AGASCourseTargetActor_CameraTrace::IsConfirmTargetingAllowed()
{
	return bLastTraceWasGood;
}

TArray<TWeakObjectPtr<AActor>> AGASCourseTargetActor_CameraTrace::PerformOverlap(const FVector& Origin)
{
	constexpr bool bTraceComplex = false;
	
	FCollisionQueryParams Params(SCENE_QUERY_STAT(RadiusTargetingOverlap), bTraceComplex);
	Params.bReturnPhysicalMaterial = false;
	
	TArray<TWeakObjectPtr<AActor>>	HitActors;
	if(QueryChannels.Num() == 0)
	{
		return HitActors;
	}
	
	OverlapMultiByObjectTypes(HitActors, Origin, FQuat::Identity, FCollisionShape::MakeSphere(CollisionRadius), Params);
	return HitActors;
}

bool AGASCourseTargetActor_CameraTrace::OverlapMultiByObjectTypes(TArray<TWeakObjectPtr<AActor>>& OutHitActors, const FVector& Pos,
                                                                  const FQuat& Rot, const FCollisionShape& OverlapCollisionShape, const FCollisionQueryParams& Params) const
{
	TArray<FOverlapResult> Overlaps;
	bool bTraceSuccessful = false;
	
	if(QueryChannels.Num() == 0)
	{
		return bTraceSuccessful;
	}
	
	for(const ECollisionChannel QueryChannel : QueryChannels)
	{
		SourceActor->GetWorld()->OverlapMultiByObjectType(Overlaps, Pos, Rot, FCollisionObjectQueryParams(QueryChannel), OverlapCollisionShape, Params);
		for(int32 i = 0; i < Overlaps.Num(); ++i)
		{
			//Should this check to see if these pawns are in the AimTarget list?
			AActor* HitActor = Overlaps[i].OverlapObjectHandle.FetchActor<AActor>();
			if (HitActor && !OutHitActors.Contains(HitActor) && Filter.FilterPassesForActor(HitActor))
			{
				OutHitActors.Add(HitActor);
			}
		}
	}

	return bTraceSuccessful = OutHitActors.Num() > 0 ? true : false;
}

FGameplayAbilityTargetDataHandle AGASCourseTargetActor_CameraTrace::MakeTargetData(
	const TArray<TWeakObjectPtr<AActor>>& Actors, const FVector& Origin) const
{
	if (OwningAbility)
	{
		/** Use the source location instead of the literal origin */
		return StartLocation.MakeTargetDataHandleFromActors(Actors, false);
	}

	return FGameplayAbilityTargetDataHandle();
}

void AGASCourseTargetActor_CameraTrace::DrawTargetOutline(TArray<TWeakObjectPtr<AActor>> InHitActors,
	TArray<TWeakObjectPtr<AActor>> InLatestHitActors)
{
	if(TargetOutlineData.CharacterClassToOutline == nullptr)
	{
		return;
	}

	if(InHitActors.IsEmpty())
	{
		return;
	}
	
	for(const TWeakObjectPtr<AActor>& Actor : InHitActors)
	{
		if(Actor->IsA(TargetOutlineData.CharacterClassToOutline))
		{
			const AGASCourseCharacter* Character = Cast<AGASCourseCharacter>(Actor);
			if(USkeletalMeshComponent* Mesh = Character->GetComponentByClass<USkeletalMeshComponent>())
			{
				Mesh->SetRenderCustomDepth(false);
				Mesh->SetCustomDepthStencilValue(STENCIL_NONE);
			}
		}
	}
	
	for(const TWeakObjectPtr<AActor>& Actor : InLatestHitActors)
	{
		if(Actor->IsA(TargetOutlineData.CharacterClassToOutline))
		{
			const AGASCourseCharacter* Character = Cast<AGASCourseCharacter>(Actor);
			if(USkeletalMeshComponent* Mesh = Character->GetComponentByClass<USkeletalMeshComponent>())
			{
				Mesh->SetRenderCustomDepth(true);
				Mesh->SetCustomDepthStencilValue(STENCIL_ENEMY_OUTLINE);
			}
		}
	}
}

void AGASCourseTargetActor_CameraTrace::ClearTargetOutline(TArray<TWeakObjectPtr<AActor>> InHitActors)
{
	if(TargetOutlineData.CharacterClassToOutline == nullptr)
	{
		return;
	}
	
	for(const TWeakObjectPtr<AActor>& Actor : InHitActors)
	{
		if(Actor->IsA(TargetOutlineData.CharacterClassToOutline))
		{
			const AGASCourseCharacter* Character = Cast<AGASCourseCharacter>(Actor);
			if(USkeletalMeshComponent* Mesh = Character->GetComponentByClass<USkeletalMeshComponent>())
			{
				Mesh->SetRenderCustomDepth(false);
				Mesh->SetCustomDepthStencilValue(STENCIL_NONE);
			}
		}
	}
}
