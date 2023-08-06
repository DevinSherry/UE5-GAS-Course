// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor_Trace.h"
#include "GASCourseTargetActor_CameraTrace.generated.h"


class UGameplayAbility;

/**
 * 
 */
UCLASS(Blueprintable)
class GASCOURSE_API AGASCourseTargetActor_CameraTrace : public AGameplayAbilityTargetActor_Trace
{
	GENERATED_UCLASS_BODY()

	virtual void StartTargeting(UGameplayAbility* InAbility) override;

	/** Radius for a sphere or capsule. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = Targeting)
	float CollisionRadius;

	/** Height for a capsule. Implicitly indicates a capsule is desired if this is greater than zero. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = Targeting)
	float CollisionHeight;

protected:
	virtual FHitResult PerformTrace(AActor* InSourceActor) override;

	virtual bool IsConfirmTargetingAllowed() override;

	bool AdjustCollisionResultForShape(const FVector OriginalStartPoint, const FVector OriginalEndPoint, const FCollisionQueryParams Params, FHitResult& OutHitResult) const;

	FCollisionShape CollisionShape;
	float CollisionHeightOffset;		//When tracing, give this much extra height to avoid start-in-ground problems. Dealing with thick placement actors while standing near walls may be trickier.
	bool bLastTraceWasGood;
	
};
