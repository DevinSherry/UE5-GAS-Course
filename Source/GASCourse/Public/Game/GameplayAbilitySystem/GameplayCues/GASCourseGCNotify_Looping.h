// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayCueNotify_Looping.h"
#include "AbilityAsync_WaitGameplayEvent.h"
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

	/** Called when the GC is finished. It may be about to go back to the recycle pool, or it may be about to be destroyed. */
	virtual void GameplayCueFinishedCallback() override;

protected:

	virtual bool OnActive_Implementation(AActor* Target, const FGameplayCueParameters& Parameters) override;
	virtual bool OnRemove_Implementation(AActor* Target, const FGameplayCueParameters& Parameters) override;

	virtual void BeginPlay() override;
	
};
