// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "AbilityTask_WaitDurationChange.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnDurationChanged, FGameplayTag, DurationTag, float, TimeRemaining, float, Duration);

/**
 * 
 */

UCLASS(BlueprintType, meta = (ExposedAsyncProxy = AsyncTask))
class GASCOURSE_API UAbilityTask_WaitDurationChange : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable)
	void EndTask();
	
public:

	UPROPERTY(BlueprintAssignable)
	FOnDurationChanged OnDurationBegin;

	UPROPERTY(BlueprintAssignable)
	FOnDurationChanged OnDurationEnd;

	UPROPERTY(BlueprintAssignable)
	FOnDurationChanged OnDurationTimeUpdated;

	UFUNCTION(BlueprintCallable, Category="GASCourse|Ability|Tasks", meta=(BlueprintInternalUseOnly = "true"))
	static UAbilityTask_WaitDurationChange* WaitForDurationChange(UAbilitySystemComponent* InAbilitySystemComponent,FGameplayTagContainer InDurationTags, float InDurationInterval, bool bInUseServerCooldown);
	
protected:

	UPROPERTY()
	UAbilitySystemComponent* ASC;

	FGameplayTagContainer DurationTags;
	float DurationInterval = 0.1f;
	bool bUseServerCooldown;

	void OnActiveGameplayEffectAddedCallback(UAbilitySystemComponent* InTargetASC, const FGameplayEffectSpec& InSpecApplied, FActiveGameplayEffectHandle ActiveHandle);
	void DurationTagChanged(const FGameplayTag InDurationTag, int32 InNewCount);
	bool GetCooldownRemainingForTag(const FGameplayTagContainer& InDurationTags, float& TimeRemaining, float& InDuration) const;
	
	UFUNCTION()
	void OnDurationUpdate();

private:

	const UObject* WorldContext;
	FTimerHandle DurationTimeUpdateTimerHandle;
};
