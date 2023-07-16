// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Async/AbilityAsync.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_WaitDurationChange.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FWaitDurationChangeDelegate, float, CurrentDuration, float, CurrentDurationAsPercentage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWaitDurationFinishedDelegate);

/**
 * 
 */

UCLASS()
class GASCOURSE_API UAbilityTask_WaitDurationChange : public UAbilityAsync
{
	GENERATED_UCLASS_BODY()
	
public:

	virtual void EndAction() override;
	virtual void Activate() override;
	
public:
	
	
	UPROPERTY(BlueprintAssignable)
	FWaitDurationChangeDelegate OnDurationChange;

	UPROPERTY(BlueprintAssignable)
	FWaitDurationFinishedDelegate OnDurationDone;


	void OnDurationChanged() const;
	
	UFUNCTION()
	void OnDurationFinished();

	UFUNCTION(BlueprintCallable, Category="GASCourse|Ability|Tasks", meta=(DefaultToSelf = "InOwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAbilityTask_WaitDurationChange* WaitForDurationChange(UGameplayAbility* InOwningAbility, float InIntervalCheck);

	FDelegateHandle OnDurationChangedDelegateHandle;
	FDelegateHandle OnDurationFinishedDelegateHandle;
	
	TSoftObjectPtr<UGameplayAbility> OwningAbility;
	float IntervalCheck = 0.1f;

private:
	
	FTimerHandle DurationIntervalTimerHandle;
};
