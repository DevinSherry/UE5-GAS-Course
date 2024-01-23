// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Game/GameplayAbilitySystem/GASCourseGameplayAbility.h"
#include "GASCourseDurationGameplayAbility.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGASCourseAbilityDurationRemoved);

/**
 * @class UGASCourseDurationGameplayAbility
 * @brief This class represents a duration-based gameplay ability. It inherits from UGASCourseGameplayAbility.
 *
 * A duration-based gameplay ability is an ability that has a specific duration during which it remains active.
 * This class provides functions for applying and handling the duration effect, as well as managing cooldown and
 * ability activation. 
 */
UCLASS()
class GASCOURSE_API UGASCourseDurationGameplayAbility : public UGASCourseGameplayAbility
{
	GENERATED_BODY()

public:
	
	UGASCourseDurationGameplayAbility(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//Callback for when applied duration effect is removed. Ability must be of type EGASCourseAbilityType::Duration
	UFUNCTION()
	void DurationEffectRemoved(const FGameplayEffectRemovalInfo& GameplayEffectRemovalInfo);

	/**
	 * @brief Helper function to get a reference to the Duration Effect class.
	 * @return 
	 */
	UFUNCTION(BlueprintCallable)
	UGameplayEffect* GetDurationGameplayEffect() const;

	UFUNCTION(BlueprintPure, Category = "GASCourse|Ability|Tags")
	void GetAbilityDurationTags(FGameplayTagContainer& DurationTags) const;
	
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
	//~End of UGameplayAbility interface

	virtual void OnPawnAvatarSet();
	
	//The duration effect to be applied, dictates how long the ability will last for.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GASCourse|Ability|Duration", meta=(EditCondition="AbilityType==EGASCourseAbilityType::Duration", EditConditionHides))
	TSubclassOf<UGameplayEffect> DurationEffect;

	/*Auto apply the duration effect on ability activation, otherwise use function Apply Duration Effect**/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GASCourse|Ability|Duration", meta=(EditCondition="AbilityType==EGASCourseAbilityType::Duration", EditConditionHides))
	bool bAutoApplyDurationEffect;

	/*Manually apply either class duration effect, or custom duration effect**/
	UFUNCTION(BlueprintCallable, Category = "GASCourse|Ability|Duration")
	UPARAM(DisplayName= "bDurationEffectApplied")
	bool ApplyDurationEffect();

	UFUNCTION()
	void OnAbilityInputPressed(float InTimeWaited);

	/**
 * @brief Should the ability automatically commit cooldown when the duration effect ends?
 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GASCourse|Ability|Duration", meta=(EditCondition="AbilityType==EGASCourseAbilityType::Duration", EditConditionHides))
	bool bAutoCommitCooldownOnDurationEnd;

	/**
 * @brief Should the ability automatically end when the duration effect ends?
 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GASCourse|Ability|Duration", meta=(EditCondition="AbilityType==EGASCourseAbilityType::Duration", EditConditionHides))
	bool bAutoEndAbilityOnDurationEnd;

	/**
	 * @brief Indicates whether the ability should be canceled upon reactivation.
	 *
	 * If this flag is set to true, the ability will be canceled if it is reactivated while it is still active. By default, this flag is true, meaning the ability will continue to run even
	 * if it is reactivated.
	 *
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GASCourse|Ability|Duration")
	bool bCancelAbilityOnReactivation;

private:
	
	FActiveGameplayEffectHandle DurationEffectHandle;
	
};
