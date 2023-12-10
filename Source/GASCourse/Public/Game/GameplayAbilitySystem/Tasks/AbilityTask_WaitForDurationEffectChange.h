// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

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

	/**
	 * This method is invoked when a gameplay effect is added to the target ability system component (ASC).
	 * It retrieves the asset tags and granted tags from the applied gameplay effect specification (SpecApplied)
	 * and checks if any of them match the duration tags (DurationTagArray) specified.
	 * If a match is found, it determines the remaining time and total duration for the duration tag, and broadcasts
	 * the OnDurationBegin event with the duration tag, time remaining, and duration as parameters.
	 *
	 * Here are the possible scenarios for broadcasting the OnDurationBegin event:
	 * - If the owner of the ASC is the server, it broadcasts the event with the duration tag, time remaining, and duration as parameters.
	 * - If bUseServerCooldown is false and the applied effect is not replicated, it broadcasts the event with the duration tag, time remaining, and duration as parameters.
	 * - If bUseServerCooldown is true and the applied effect is not replicated, it broadcasts the event with the duration tag, time remaining, and duration as parameters.
	 * - If bUseServerCooldown is true and the applied effect is replicated, it broadcasts the event with the duration tag, -1.0f, and -1.0f as parameters.
	 *
	 * Additionally, if the WorldContext is not null, it sets a timer that calls the OnDurationUpdate method periodically
	 * with a specified frequency (DurationInterval).
	 *
	 * @param InTargetASC The target ability system component to which the gameplay effect is added.
	 * @param InSpecApplied The gameplay effect specification that was applied.
	 * @param ActiveHandle The handle of the active gameplay effect.
	 */
	void OnActiveGameplayEffectAddedCallback(UAbilitySystemComponent* InTargetASC, const FGameplayEffectSpec& InSpecApplied, FActiveGameplayEffectHandle ActiveHandle);

	/**
	 * \brief Notifies when the duration tag of a duration effect has changed.
	 *
	 * This method is called when the duration tag of a duration effect has changed to a new count.
	 * If the new count is zero, it means that the duration has ended.
	 *
	 * \param InDurationTag The gameplay tag of the duration effect.
	 * \param InNewCount The new count of the duration effect.
	 */
	void DurationTagChanged(const FGameplayTag InDurationTag, int32 InNewCount);

	/**
	 * Retrieves the remaining time and total duration of the longest active cooldown with the specified tags.
	 *
	 * @param InDurationTags The tags used to identify the cooldowns.
	 * @param TimeRemaining (out) The remaining time of the longest active cooldown.
	 * @param InDuration (out) The total duration of the longest active cooldown.
	 *
	 * @return True if a cooldown with the specified tags is found, false otherwise.
	 */
	bool GetCooldownRemainingForTag(const FGameplayTagContainer& InDurationTags, float& TimeRemaining, float& InDuration) const;

	/**
	 * @brief The OnDurationUpdate method is called when the duration of a particular effect is updated.
	 *
	 * This method calculates the time remaining and total duration of the effect and broadcasts these values
	 * using the OnDurationTimeUpdated event.
	 *
	 * @see UAbilityTask_WaitForDurationEffectChange::OnDurationTimeUpdated
	 */
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
	
	/**
	 * WaitOnDurationChange is a static method that creates and returns an instance of UAbilityTask_WaitOnDurationChange.
	 * This task listens for changes in the duration of active gameplay effects with the specified duration tags.
	 *
	 * @param InAbilitySystemComponent The ability system component to listen for duration changes on.
	 * @param InDurationTags The gameplay tag container specifying which duration tags to listen for changes on.
	 * @param InDurationInterval The interval (in seconds) at which to check for duration changes.
	 * @param bInUseServerCooldown Determines if server cooldown should be used for duration changes.
	 * @return A new instance of UAbilityTask_WaitOnDurationChange that is set up to listen for duration changes.
	 */
	UFUNCTION(BlueprintCallable, Category="GASCourse|Ability|Tasks", meta=(BlueprintInternalUseOnly = "true"))
	static UAbilityTask_WaitOnDurationChange* WaitOnDurationChange(UAbilitySystemComponent* InAbilitySystemComponent,FGameplayTagContainer InDurationTags, float InDurationInterval=0.05f, bool bInUseServerCooldown=true);
	
};

UCLASS(BlueprintType, meta = (ExposedAsyncProxy = AsyncTask))
class GASCOURSE_API UAbilityTask_WaitOnCooldownChange : public UAbilityTask_WaitForDurationEffectChange
{
	GENERATED_BODY()
	
public:
	/**
	 * Waits for the cooldown status of ability to change.
	 *
	 * @param InAbilitySystemComponent The ability system component to wait on cooldown change.
	 * @param InCooldownTags The cooldown tags to listen for changes.
	 * @param InDurationInterval The duration interval to check for cooldown changes.
	 * @param bInUseServerCooldown Determines if server cooldown should be used.
	 * @return The instance of UAbilityTask_WaitOnCooldownChange.
	 */
	UFUNCTION(BlueprintCallable, Category="GASCourse|Ability|Tasks", meta=(BlueprintInternalUseOnly = "true"))
	static UAbilityTask_WaitOnCooldownChange* WaitOnCooldownChange(UAbilitySystemComponent* InAbilitySystemComponent,FGameplayTagContainer InCooldownTags, float InDurationInterval = 1.0f, bool bInUseServerCooldown=true);
	
};
