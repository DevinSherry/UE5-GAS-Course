// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tasks/TargetingSortTask_Base.h"
#include "GASCourse_TargetSortBase.generated.h"

/** Lifecycle function called when the task first begins */

UCLASS(Blueprintable)
class GASCOURSE_API UGASCourse_TargetSortBase : public UTargetingSortTask_Base
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
	 * @brief Mutable APawn pointer representing the source pawn for sorting targets.
	 *
	 * The SourcePawn variable is a mutable pointer to an APawn object that is initialized to nullptr.
	 * It is used to store the source pawn for sorting targets and can be modified.
	 */
	UPROPERTY()
	mutable APawn* SourcePawn = nullptr;

	/**
	 * @brief Mutable pointer to a Player Controller representing the source player controller.
	 *
	 * The SourcePlayerController variable is a mutable pointer to an APlayerController object that is initialized to nullptr.
	 * It is used to store the source player controller for sorting targets and can be modified.
	 */
	UPROPERTY()
	mutable APlayerController* SourcePlayerController = nullptr;

	/**
	 * Member variable to store the flag indicating whether the source actor is currently moving.
	 */
	UPROPERTY()
	mutable bool bIsSourceActorMoving;

	/**
	 * Mutable variable to store the final score multiplier used in sorting targets
	 * in GASCourse_TargetSortBase class.
	 */
	UPROPERTY()
	mutable float FinalScoreMultiplier = 1.0f;

	/**
	 * Mutable variable to store the score for sorting targets in the GASCourse_TargetSortBase class.
	 */
	UPROPERTY()
	mutable float Score = 0.0f;

public:
	
	/**
	 * Boolean flag indicating whether the target is affected by the source actor movement input.
	 * If true, the target sorting may consider the source actor's movement when calculating scores.
	 * By default, it is set to false.
	 */
	UPROPERTY(EditDefaultsOnly, Category = "GASCourse|Targeting|MovementInput")
	bool bAffectedBySourceActorMovement = true;

	/**
	 * @brief Score multiplier applied when source movement input is NOT detected
	 *
	 * Member variable representing the score multiplier applied when the source movement input is not detected.
	 * This value is used in sorting targets in the GASCourse_TargetSortBase class.
	 */
	UPROPERTY(EditDefaultsOnly, Category="GASCourse|Targeting|Score", meta=(ToolTip="Score multiplier applied when source movement input is NOT detected"))
	float DefaultScoreMultiplier = 1.0f;

	/**
	 * Score multiplier applied when source movement input is detected.
	 *
	 * This variable determines the multiplier to be applied to the score when movement input is detected
	 * for the source actor. It affects the sorting of targets in GASCourse. The score can be multiplied
	 * by this value when the source actor is moving.
	 */
	UPROPERTY(EditDefaultsOnly, Category="GASCourse|Targeting|Score", meta=(ToolTip="Score multiplier applied when source movement input is detected"))
	float ScoreMultiplierWhenMoving = 1.0f;

	/**
	 * Member variable to store a curve mapping float values to custom scores in GASCourse.
	 * If null, score calculations will be handled by class functionality.
	 */
	UPROPERTY(EditDefaultsOnly, Category="GASCourse|Targeting|Score", meta=(ToolTip="Custom score mapping based on a float curve value. If null, score calculations will be handled by class functionality."))
	UCurveFloat* ScoreCurve = nullptr;

	/** Called on every target to get a Score for sorting. This score will be added to the Score float in FTargetingDefaultResultData */
	UFUNCTION(BlueprintImplementableEvent, DisplayName=GetScoreForTarget, Category=Targeting)
	float BP_GetScoreForTarget(const FTargetingRequestHandle& TargetingHandle, const FTargetingDefaultResultData& TargetData) const;
	
};
