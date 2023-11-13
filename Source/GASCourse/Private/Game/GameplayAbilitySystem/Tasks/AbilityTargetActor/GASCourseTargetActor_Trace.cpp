// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GameplayAbilitySystem/Tasks/AbilityTargetActor/GASCourseTargetActor_Trace.h"
#include "AbilitySystemComponent.h"
#include "Game/Character/NPC/GASCourseNPC_Base.h"
#include "Game/Character/Player/GASCoursePlayerController.h"
#include "Abilities/GameplayAbility.h"
#include "Game/GameplayAbilitySystem/GASCourseNativeGameplayTags.h"

AGASCourseTargetActor_Trace::AGASCourseTargetActor_Trace(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PostUpdateWork;

	//Initialize these variables to our needs for tracing.
	MaxRange = 999999.0f;
}

void AGASCourseTargetActor_Trace::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (ReticleActor.IsValid())
	{
		ReticleActor.Get()->Destroy();
	}

	Super::EndPlay(EndPlayReason);
}

void AGASCourseTargetActor_Trace::LineTraceWithFilter(FHitResult& OutHitResult, const UWorld* World,
	const FGameplayTargetDataFilterHandle FilterHandle, const FVector& Start, const FVector& End, ECollisionChannel CollisionChannel,
	const FCollisionQueryParams Params)
{
	check(World);

	TArray<FHitResult> HitResults;
	//World->LineTraceMultiByProfile(HitResults, Start, End, ProfileName, Params);
	World->LineTraceMultiByChannel(HitResults, Start, End, CollisionChannel, Params);

	OutHitResult.TraceStart = Start;
	OutHitResult.TraceEnd = End;

	for (int32 HitIdx = 0; HitIdx < HitResults.Num(); ++HitIdx)
	{
		const FHitResult& Hit = HitResults[HitIdx];

		if (!Hit.HitObjectHandle.IsValid() || FilterHandle.FilterPassesForActor(Hit.HitObjectHandle.FetchActor()))
		{
			OutHitResult = Hit;
			OutHitResult.bBlockingHit = true; // treat it as a blocking hit
			return;
		}
	}
}

void AGASCourseTargetActor_Trace::SweepWithFilter(FHitResult& OutHitResult, const UWorld* World,
	const FGameplayTargetDataFilterHandle FilterHandle, const FVector& Start, const FVector& End, const FQuat& Rotation,
	const FCollisionShape CollisionShape, FName ProfileName, const FCollisionQueryParams Params)
{
	check(World);

	TArray<FHitResult> HitResults;
	World->SweepMultiByProfile(HitResults, Start, End, Rotation, ProfileName, CollisionShape, Params);

	OutHitResult.TraceStart = Start;
	OutHitResult.TraceEnd = End;

	for (int32 HitIdx = 0; HitIdx < HitResults.Num(); ++HitIdx)
	{
		const FHitResult& Hit = HitResults[HitIdx];

		if (!Hit.HitObjectHandle.IsValid() || FilterHandle.FilterPassesForActor(Hit.HitObjectHandle.FetchActor()))
		{
			OutHitResult = Hit;
			OutHitResult.bBlockingHit = true; // treat it as a blocking hit
			return;
		}
	}
}

void AGASCourseTargetActor_Trace::AimWithPlayerController(const AActor* InSourceActor, FCollisionQueryParams Params,
	const FVector& TraceStart, FVector& OutTraceEnd, bool bIgnorePitch) const
{
	if (!OwningAbility) // Server and launching client only
		{
		return;
		}

	APlayerController* PC = OwningAbility->GetCurrentActorInfo()->PlayerController.Get();
	check(PC);

	FVector ViewStart;
	FRotator ViewRot;
	PC->GetPlayerViewPoint(ViewStart, ViewRot);

	const FVector ViewDir = ViewRot.Vector();
	FVector ViewEnd = ViewStart + (ViewDir * MaxRange);

	ClipCameraRayToAbilityRange(ViewStart, ViewDir, TraceStart, MaxRange, ViewEnd);

	FHitResult HitResult;
	LineTraceWithFilter(HitResult, InSourceActor->GetWorld(), Filter, ViewStart, ViewEnd, TraceChannel, Params);

	const bool bUseTraceResult = HitResult.bBlockingHit && (FVector::DistSquared(TraceStart, HitResult.Location) <= (MaxRange * MaxRange));

	const FVector AdjustedEnd = (bUseTraceResult) ? HitResult.Location : ViewEnd;

	FVector AdjustedAimDir = (AdjustedEnd - TraceStart).GetSafeNormal();
	if (AdjustedAimDir.IsZero())
	{
		AdjustedAimDir = ViewDir;
	}

	if (bUseTraceResult)
	{
		FVector OriginalAimDir = (ViewEnd - TraceStart).GetSafeNormal();

		if (!OriginalAimDir.IsZero())
		{
			// Convert to angles and use original pitch
			const FRotator OriginalAimRot = OriginalAimDir.Rotation();

			FRotator AdjustedAimRot = AdjustedAimDir.Rotation();
			AdjustedAimRot.Pitch = OriginalAimRot.Pitch;

			AdjustedAimDir = AdjustedAimRot.Vector();
		}
	}

	OutTraceEnd = TraceStart + (AdjustedAimDir * MaxRange);
}

bool AGASCourseTargetActor_Trace::ClipCameraRayToAbilityRange(FVector CameraLocation, FVector CameraDirection,
	FVector AbilityCenter, float AbilityRange, FVector& ClippedPosition)
{
	FVector CameraToCenter = AbilityCenter - CameraLocation;
	float DotToCenter = FVector::DotProduct(CameraToCenter, CameraDirection);
	if (DotToCenter >= 0)		//If this fails, we're pointed away from the center, but we might be inside the sphere and able to find a good exit point.
		{
		float DistanceSquared = CameraToCenter.SizeSquared() - (DotToCenter * DotToCenter);
		float RadiusSquared = (AbilityRange * AbilityRange);
		if (DistanceSquared <= RadiusSquared)
		{
			float DistanceFromCamera = FMath::Sqrt(RadiusSquared - DistanceSquared);
			float DistanceAlongRay = DotToCenter + DistanceFromCamera;						//Subtracting instead of adding will get the other intersection point
			ClippedPosition = CameraLocation + (DistanceAlongRay * CameraDirection);		//Cam aim point clipped to range sphere
			return true;
		}
		}
	return false;
}

void AGASCourseTargetActor_Trace::Tick(float DeltaSeconds)
{
	// very temp - do a mostly hardcoded trace from the source actor
	if (SourceActor && SourceActor->GetLocalRole() != ENetRole::ROLE_SimulatedProxy)
	{
		FHitResult HitResult = PerformTrace(SourceActor);
		FVector EndPoint = HitResult.Component.IsValid() ? HitResult.ImpactPoint : HitResult.TraceEnd;

#if ENABLE_DRAW_DEBUG
		if (bDebug)
		{
			DrawDebugLine(GetWorld(), SourceActor->GetActorLocation(), EndPoint, FColor::Green, false);
			DrawDebugSphere(GetWorld(), EndPoint, 16, 10, FColor::Green, false);
		}
#endif // ENABLE_DRAW_DEBUG

		SetActorLocationAndRotation(EndPoint, SourceActor->GetActorRotation());
	}
}

void AGASCourseTargetActor_Trace::ShowMouseCursor(bool bShowCursor)
{
	if(AGASCoursePlayerController* SourcePC = Cast<AGASCoursePlayerController>(OwningAbility->GetCurrentActorInfo()->PlayerController.Get()))
	{
		SourcePC->bShowMouseCursor = bShowCursor;
	}
}

FGameplayAbilityTargetDataHandle AGASCourseTargetActor_Trace::MakeTargetData(const FHitResult& HitResult) const
{
	/** Note: This will be cleaned up by the FGameplayAbilityTargetDataHandle (via an internal TSharedPtr) */
	return StartLocation.MakeTargetDataHandleFromHitResult(OwningAbility, HitResult);
}

void AGASCourseTargetActor_Trace::DrawTargetOutline(TArray<TWeakObjectPtr<AActor>> InHitActors, TArray<TWeakObjectPtr<AActor>> InLatestHitActors)
{

}

void AGASCourseTargetActor_Trace::ClearTargetOutline(TArray<TWeakObjectPtr<AActor>> InHitActors)
{

}

void AGASCourseTargetActor_Trace::StartTargeting(UGameplayAbility* InAbility)
{
	Super::StartTargeting(InAbility);
	SourceActor = InAbility->GetCurrentActorInfo()->AvatarActor.Get();
	UpdateLooseGameplayTagsDuringTargeting(Status_Block_PointClickMovementInput, 1);
	UpdateLooseGameplayTagsDuringTargeting(Status_Gameplay_Targeting, 1);
	ShowMouseCursor(false);
	
	if (ReticleClass)
	{
		AGameplayAbilityWorldReticle* SpawnedReticleActor = GetWorld()->SpawnActor<AGameplayAbilityWorldReticle>(ReticleClass, GetActorLocation(), GetActorRotation());
		if (SpawnedReticleActor)
		{
			SpawnedReticleActor->InitializeReticle(this, PrimaryPC, ReticleParams);
			ReticleActor = SpawnedReticleActor;

			// This is to catch cases of playing on a listen server where we are using a replicated reticle actor.
			// (In a client controlled player, this would only run on the client and therefor never replicate. If it runs
			// on a listen server, the reticle actor may replicate. We want consistancy between client/listen server players.
			// Just saying 'make the reticle actor non replicated' isnt a good answer, since we want to mix and match reticle
			// actors and there may be other targeting types that want to replicate the same reticle actor class).
			if (!ShouldProduceTargetDataOnServer)
			{
				SpawnedReticleActor->SetReplicates(false);
			}
		}
	}

}

void AGASCourseTargetActor_Trace::ConfirmTargetingAndContinue()
{
	Super::ConfirmTargetingAndContinue();
	UpdateLooseGameplayTagsDuringTargeting(Status_Block_PointClickMovementInput, 0);
	UpdateLooseGameplayTagsDuringTargeting(Status_Gameplay_Targeting, 0);
	ShowMouseCursor(true);
	ClearTargetOutline(ActorsToOutline);
}

void AGASCourseTargetActor_Trace::CancelTargeting()
{
	Super::CancelTargeting();
	UpdateLooseGameplayTagsDuringTargeting(Status_Block_PointClickMovementInput, 0);
	UpdateLooseGameplayTagsDuringTargeting(Status_Gameplay_Targeting, 0);
	ShowMouseCursor(true);
	ClearTargetOutline(ActorsToOutline);
}

void AGASCourseTargetActor_Trace::ConfirmTargeting()
{
	Super::ConfirmTargeting();
	Super::ConfirmTargetingAndContinue();
	UpdateLooseGameplayTagsDuringTargeting(Status_Block_PointClickMovementInput, 0);
	UpdateLooseGameplayTagsDuringTargeting(Status_Gameplay_Targeting, 0);
	ShowMouseCursor(true);
	ClearTargetOutline(ActorsToOutline);
}

void AGASCourseTargetActor_Trace::UpdateLooseGameplayTagsDuringTargeting(FGameplayTag InGameplayTag, int32 InCount)
{
	if(UAbilitySystemComponent* ASC = OwningAbility->GetCurrentActorInfo()->AbilitySystemComponent.Get())
	{
		ASC->SetLooseGameplayTagCount(InGameplayTag, InCount);
	}
}
