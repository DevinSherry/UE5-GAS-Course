// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tasks/TargetingSortTask_Base.h"
#include "GASCourse_TargetSortBase.generated.h"

/**
 * 
 */
UCLASS()
class GASCOURSE_API UGASCourse_TargetSortBase : public UTargetingSortTask_Base
{
	GENERATED_BODY()

	/** Lifecycle function called when the task first begins */
	virtual void Init(const FTargetingRequestHandle& TargetingHandle) const override;

	UPROPERTY()
	mutable bool bIsSourceActorMoving;

protected:
	/** Called on every target to get a Score for sorting. This score will be added to the Score float in FTargetingDefaultResultData */
	virtual float GetScoreForTarget(const FTargetingRequestHandle& TargetingHandle, const FTargetingDefaultResultData& TargetData) const;

	/** Evaluation function called by derived classes to process the targeting request */
	virtual void Execute(const FTargetingRequestHandle& TargetingHandle) const override;

public:

	UPROPERTY(EditAnywhere, Category = "GASCourse|Targeting")
	bool bAffectedBySourceActorMovement = false;

	UPROPERTY(EditAnywhere, Category="GASCourse|Targeting", meta=(EditCondition="!bAffectedBySourceActorMovement"))
	float DefaultScoreMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, Category="GASCourse|Targeting", meta=(EditCondition="bAffectedBySourceActorMovement"))
	float ScoreMultiplierWhenMoving = 1.0f;

	/** Called on every target to get a Score for sorting. This score will be added to the Score float in FTargetingDefaultResultData */
	UFUNCTION(BlueprintImplementableEvent, DisplayName=GetScoreForTarget, Category=Targeting)
	float BP_GetScoreForTarget(const FTargetingRequestHandle& TargetingHandle, const FTargetingDefaultResultData& TargetData) const;
	
};
