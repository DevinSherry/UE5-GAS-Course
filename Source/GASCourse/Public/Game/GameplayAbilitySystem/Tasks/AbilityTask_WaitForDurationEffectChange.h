// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "AbilityTask_WaitForDurationEffectChange.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnDurationChanged, FGameplayTag, DurationTag, float, TimeRemaining, float, Duration);

/**
 * 
 */

UCLASS(Abstract)
class GASCOURSE_API UAbilityTask_WaitForDurationEffectChange: public UBlueprintAsyncActionBase
{

	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable)
	void EndTask();
	
	UPROPERTY(BlueprintAssignable)
	FOnDurationChanged OnDurationBegin;

	UPROPERTY(BlueprintAssignable)
	FOnDurationChanged OnDurationEnd;

	UPROPERTY(BlueprintAssignable)
	FOnDurationChanged OnDurationTimeUpdated;

protected:

	UPROPERTY()
	UAbilitySystemComponent* ASC;

	FGameplayTagContainer DurationTags;
	float DurationInterval = 0.1f;
	bool bUseServerCooldown;
	const UObject* WorldContext;

	
	void OnActiveGameplayEffectAddedCallback(UAbilitySystemComponent* InTargetASC, const FGameplayEffectSpec& InSpecApplied, FActiveGameplayEffectHandle ActiveHandle);
	void DurationTagChanged(const FGameplayTag InDurationTag, int32 InNewCount);
	bool GetCooldownRemainingForTag(const FGameplayTagContainer& InDurationTags, float& TimeRemaining, float& InDuration) const;

		
	UFUNCTION()
	void OnDurationUpdate();

private:

	
	FTimerHandle DurationTimeUpdateTimerHandle;
};

UCLASS(BlueprintType, meta = (ExposedAsyncProxy = AsyncTask))
class GASCOURSE_API UAbilityTask_WaitOnDurationChange : public UAbilityTask_WaitForDurationEffectChange
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintCallable, Category="GASCourse|Ability|Tasks", meta=(BlueprintInternalUseOnly = "true"))
	static UAbilityTask_WaitOnDurationChange* WaitOnDurationChange(UAbilitySystemComponent* InAbilitySystemComponent,FGameplayTagContainer InDurationTags, float InDurationInterval=0.05f, bool bInUseServerCooldown=true);
	
};

UCLASS(BlueprintType, meta = (ExposedAsyncProxy = AsyncTask))
class GASCOURSE_API UAbilityTask_WaitOnCooldownChange : public UAbilityTask_WaitForDurationEffectChange
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, Category="GASCourse|Ability|Tasks", meta=(BlueprintInternalUseOnly = "true"))
	static UAbilityTask_WaitOnCooldownChange* WaitOnCooldownChange(UAbilitySystemComponent* InAbilitySystemComponent,FGameplayTagContainer InCooldownTags, float InDurationInterval = 1.0f, bool bInUseServerCooldown=true);
	
};
