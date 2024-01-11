// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Game/GameplayAbilitySystem/GASCourseGameplayAbility.h"
#include "GASCourseAimcastGameplayAbility.generated.h"

/**
 *
 * \brief This class represents a gameplay ability that allows the player to aim and cast a specific action.
 *
 * The UGASCourseAimcastGameplayAbility class is a derived class of UGASCourseGameplayAbility and implements various methods from the UGameplayAbility interface.
 * It provides functionality to activate and execute the aimcast ability, apply costs, handle cooldowns, and more.
 *
 * \note This class should be used as a base class for specific aimcast gameplay abilities.
 */
UCLASS()
class GASCOURSE_API UGASCourseAimcastGameplayAbility : public UGASCourseGameplayAbility
{
	GENERATED_BODY()

public:
	
	UGASCourseAimcastGameplayAbility(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/**
	 * @brief Callback function invoked when the aim cast target data is ready to be processed.
	 *
	 * This function is called when the aim cast target data is ready to be processed. It takes in the aim cast target data handle and the application tag.
	 *
	 * @param InData The target data handle containing the aim cast target data.
	 * @param ApplicationTag The tag of the ability application that caused the target data to be ready.
	 */
	void OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& InData, FGameplayTag ApplicationTag);

	/**
	 * @brief Callback function invoked when the aim cast target data is cancelled.
	 *
	 * This function is called when the aim cast target data is cancelled. It takes in the target data handle as the parameter.
	 *
	 * @param Data The handle containing the aim cast target data that was cancelled.
	 */
	void OnTargetDataCancelledCallback(const FGameplayAbilityTargetDataHandle& Data);


protected:

	//~UGameplayAbility interface
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual void SetCanBeCanceled(bool bCanBeCanceled) override;
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual bool CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;
	virtual FGameplayEffectContextHandle MakeEffectContext(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo) const override;
	virtual void ApplyAbilityTagsToGameplayEffectSpec(FGameplayEffectSpec& Spec, FGameplayAbilitySpec* AbilitySpec) const override;
	virtual bool DoesAbilitySatisfyTagRequirements(const UAbilitySystemComponent& AbilitySystemComponent, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual bool CommitAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) override;
	virtual void CommitExecute(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	virtual void ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;
	virtual void K2_EndAbility() override;
	//~End of UGameplayAbility interface

	virtual void OnPawnAvatarSet();

	/**
	 * @brief Event called when the aim cast target data is ready.
	 *
	 * This event is blueprint implementable, allowing developers to define their own functionality in blueprint.
	 * It is called when the aim cast target data is ready to be processed.
	 *
	 * @param TargetData The target data handle containing the aim cast target data.
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void OnAimCastTargetDataReady(const FGameplayAbilityTargetDataHandle& TargetData);

	/**
	 * @brief Event called when the aim cast target data is cancelled.
	 *
	 * This event is blueprint implementable, allowing developers to define their own functionality in blueprint.
	 * It is called when the aim cast target data is cancelled.
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void OnAimCastTargetDataCancelled();

protected:
	
	/**
	 * Whether the ability should be ended if the target data is cancelled.
	 * If false, make sure to manually handle End Ability in your ability.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="GASCourse|Ability|AimCast")
	bool bEndAbilityOnTargetDataCancelled;

	/**
	 * Whether the ability should send target data to the server when it is cancelled.
	 * If true, target data will be sent to the server. If false, target data will not be sent.
	 * If the ability is interrupted and bEndAbilityOnTargetDataCancelled is true, the ability will be automatically ended.
	 * If the ability is interrupted and bEndAbilityOnTargetDataCancelled is false, the ability must be manually ended.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="GASCourse|Ability|AimCast")
	bool bSendTargetDataToServerOnCancelled;

private:
	
	FActiveGameplayEffectHandle DurationEffectHandle;
	FDelegateHandle OnTargetDataReadyCallbackDelegateHandle;
	
};
