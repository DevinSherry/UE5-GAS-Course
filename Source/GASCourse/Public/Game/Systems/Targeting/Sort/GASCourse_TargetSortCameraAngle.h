// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Game/Systems/Targeting/Sort/GASCourse_TargetSortBase.h"
#include "GASCourse_TargetSortCameraAngle.generated.h"

/**
 * Class representing a camera angle-based target sorting task within the GASCourse module.
 *
 * This class inherits from UGASCourse_TargetSortBase.
 */
UCLASS(Blueprintable)
class GASCOURSE_API UGASCourse_TargetSortCameraAngle : public UGASCourse_TargetSortBase
{
	GENERATED_BODY()

public:
	/**
	 * Represents the maximum angle that the camera can be tilted when targeting in the GASCourse module.
	 *
	 * This variable is used to limit the angle of the camera when calculating scores based on camera angles.
	 */
	UPROPERTY(EditDefaultsOnly, Category = "GASCourse|Targeting|Camera")
	float MaxCameraAngle = 30.0f;
	
	/** Lifecycle function called when the task first begins */
	virtual void Init(const FTargetingRequestHandle& TargetingHandle) const override;
	
	/** Evaluation function called by derived classes to process the targeting request */
	virtual void Execute(const FTargetingRequestHandle& TargetingHandle) const override;
	
	/** Called on every target to get a Score for sorting. This score will be added to the Score float in FTargetingDefaultResultData */
	virtual float GetScoreForTarget(const FTargetingRequestHandle& TargetingHandle, const FTargetingDefaultResultData& TargetData) const;

protected:
	/**
	 * Represents a mutable pointer to an instance of APlayerCameraManager.
	 *
	 * This variable is used to manage the player camera and retrieve camera-related information
	 * needed for targeting and scoring calculations within the GASCourse module.
	 */
	UPROPERTY()
	mutable APlayerCameraManager* CameraManager = nullptr;
	
};
