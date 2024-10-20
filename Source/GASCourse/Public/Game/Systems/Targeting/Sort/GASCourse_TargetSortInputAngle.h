// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Game/Systems/Targeting/Sort/GASCourse_TargetSortBase.h"
#include "GASCourse_TargetSortInputAngle.generated.h"

/**
 * UGASCourse_TargetSortInputAngle class is a subclass of UGASCourse_TargetSortBase
 * that provides functionality for sorting targets based on input angle.
 */
UCLASS()
class GASCOURSE_API UGASCourse_TargetSortInputAngle : public UGASCourse_TargetSortBase
{
	GENERATED_BODY()
	
protected:
	
	/** Lifecycle function called when the task first begins */
	virtual void Init(const FTargetingRequestHandle& TargetingHandle) const override;
	
	/** Evaluation function called by derived classes to process the targeting request */
	virtual void Execute(const FTargetingRequestHandle& TargetingHandle) const override;
	
	/** Called on every target to get a Score for sorting. This score will be added to the Score float in FTargetingDefaultResultData */
	virtual float GetScoreForTarget(const FTargetingRequestHandle& TargetingHandle, const FTargetingDefaultResultData& TargetData) const;

	/**
	 * @brief Mutable FVector representing the input direction for sorting targets.
	 */
	UPROPERTY()
	mutable FVector InputDirection;
	
	
};
