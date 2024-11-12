// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Game/Systems/Targeting/Sort/GASCourse_TargetSortBase.h"
#include "GASCourse_TargetSortDistance.generated.h"

/**
 * @class UGASCourse_TargetSortDistance
 * @brief A class that represents a target sorting task based on distance in GASCourse.
 *
 * UGASCourse_TargetSortDistance is derived from UGASCourse_TargetSortBase and provides functionality for sorting
 * targets based on distance.
 */
UCLASS()
class GASCOURSE_API UGASCourse_TargetSortDistance : public UGASCourse_TargetSortBase
{
	GENERATED_BODY()

public:
	/**
	 * @brief Editable default maximum distance for targeting in GASCourse.
	 *
	 * Represents the maximum distance in float units within which targets can be considered for sorting in the GASCourse_TargetSortDistance class.
	 * The default value is set to 600.0f and can be edited in the GASCourse editor. This parameter is applicable when target sorting based on distance is enabled and not disabled.
	 *
	 * @see UGASCourse_TargetSortDistance
	 */
	UPROPERTY(EditDefaultsOnly, Category = "GASCourse|Targeting|Distance")
	float MaxDistance = 600.0f;
	
protected:
	
	/** Lifecycle function called when the task first begins */
	virtual void Init(const FTargetingRequestHandle& TargetingHandle) const override;
	
	/** Evaluation function called by derived classes to process the targeting request */
	virtual void Execute(const FTargetingRequestHandle& TargetingHandle) const override;
	
	/** Called on every target to get a Score for sorting. This score will be added to the Score float in FTargetingDefaultResultData */
	virtual float GetScoreForTarget(const FTargetingRequestHandle& TargetingHandle, const FTargetingDefaultResultData& TargetData) const;
};
