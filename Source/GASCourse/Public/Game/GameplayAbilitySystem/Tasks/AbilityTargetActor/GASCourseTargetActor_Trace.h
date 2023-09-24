// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor_Trace.h"
#include "GASCourseTargetActor_Trace.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class GASCOURSE_API AGASCourseTargetActor_Trace : public AGameplayAbilityTargetActor_Trace
{
	GENERATED_UCLASS_BODY()

public:
	
	virtual void StartTargeting(UGameplayAbility* InAbility) override;
	virtual void ConfirmTargetingAndContinue() override;
	virtual void CancelTargeting();

	virtual void UpdateLooseGameplayTagsDuringTargeting(FGameplayTag InGameplayTag, int32 InCount);

protected:

	FGameplayTagContainer DefaultTargetingTagContainer;
	
};
