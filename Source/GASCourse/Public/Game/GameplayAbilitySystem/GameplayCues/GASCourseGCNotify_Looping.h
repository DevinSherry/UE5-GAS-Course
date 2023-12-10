// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayCueNotify_Looping.h"
#include "GASCourseGCNotify_Looping.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class GASCOURSE_API AGASCourseGCNotify_Looping : public AGameplayCueNotify_Looping
{
	GENERATED_BODY()

public:

	AGASCourseGCNotify_Looping();

	virtual void HandleGameplayCue(AActor* MyTarget, EGameplayCueEvent::Type EventType, const FGameplayCueParameters& Parameters) override;

	/**
	 * @brief Callback function called when a gameplay cue is finished.
	 *
	 * This function is called when a gameplay cue is finished for the actor that owns this component.
	 * It handles clearing the finish timer, calling the OnRemove event if necessary, and notifying the
	 * gameplay cue manager that the actor has finished the gameplay cue.
	 */
	virtual void GameplayCueFinishedCallback() override;

protected:
	
	/**
	 * Activates the gameplay cue for the given actor.
	 *
	 * @param Target The actor for which the gameplay cue is being activated.
	 * @param Parameters The gameplay cue parameters.
	 * @return True if the gameplay cue was successfully activated, false otherwise.
	 */
	virtual bool OnActive_Implementation(AActor* Target, const FGameplayCueParameters& Parameters) override;

	/**
	 * Called when the gameplay cue is removed from the target actor.
	 *
	 * @param Target The actor from which the gameplay cue is being removed.
	 * @param Parameters Additional parameters associated with the gameplay cue.
	 *
	 * @return Whether the gameplay cue was successfully removed.
	 */
	virtual bool OnRemove_Implementation(AActor* Target, const FGameplayCueParameters& Parameters) override;

	virtual void BeginPlay() override;
	
};
