// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GASCourseTargetActor_Trace.h"
#include "GASCourseTargetActor_CameraTrace.generated.h"

class UGameplayAbility;

/**
 * 
 */

UCLASS(Blueprintable)
class GASCOURSE_API AGASCourseTargetActor_CameraTrace : public AGASCourseTargetActor_Trace
{
	GENERATED_UCLASS_BODY()

public:	

	virtual void StartTargeting(UGameplayAbility* InAbility) override;
	virtual void ConfirmTargetingAndContinue() override;
	virtual void CancelTargeting();

	/** Radius for a sphere or capsule. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = Targeting)
	float CollisionRadius;

	/** Height for a capsule. Implicitly indicates a capsule is desired if this is greater than zero. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = Targeting)
	float CollisionHeight;
	
	/** Trace Channel to check for*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = Targeting)
	TArray<TEnumAsByte<ECollisionChannel>> QueryChannels;

protected:
	virtual FHitResult PerformTrace(AActor* InSourceActor) override;

	virtual bool IsConfirmTargetingAllowed() override;

	TArray<TWeakObjectPtr<AActor> >	PerformOverlap(const FVector& Origin);
	
	bool OverlapMultiByObjectTypes(TArray<TWeakObjectPtr<AActor>>& OutHitActors, const FVector& Pos, const FQuat& Rot, const FCollisionShape& OverlapCollisionShape,
		const FCollisionQueryParams& Params = FCollisionQueryParams::DefaultQueryParam) const;
	
	FGameplayAbilityTargetDataHandle MakeTargetData(const TArray<TWeakObjectPtr<AActor>>& Actors, const FVector& Origin) const;
	
protected:	
	bool bLastTraceWasGood;
};
