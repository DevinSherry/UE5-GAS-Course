// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Abilities/GameplayAbility.h"
#include "Game/Character/Player/GASCoursePlayerCharacter.h"
#include "Game/Character/Player/GASCoursePlayerController.h"
#include "GASCourseGameplayAbility.generated.h"

/*
 * Delegate fired when ability is committed, returns whether commit was successful
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGASCourseAbilityCommitSignature, bool, CommitAbility);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGASCourseAbilityCooldownCommitSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGASCourseAbilityDurationRemoved);


/**
 * EGASCourseAbilityActivationPolicy
 *
 *	Defines how an ability is meant to activate.
 */
UENUM(BlueprintType)
enum class EGASCourseAbilityActivationPolicy : uint8
{
	// Try to activate the ability when the input is triggered.
	OnInputTriggered,

	// Continually try to activate the ability while the input is active.
	WhileInputActive,

	// Try to activate the ability when an avatar is assigned.
	OnSpawn
};

UENUM(BlueprintType)
enum class EGASCourseAbilityType : uint8
{
	// Instant ability, immediately goes into cool-down.
	Instant,

	// Must have duration gameplay effect class, ability automatically ends on duration end.
	Duration,

	// Instantiates target data aiming.
	AimCast
};

/**
 * 
 */
UCLASS(Abstract, HideCategories = Input, Meta = (ShortTooltip = "The base gameplay ability class used by this project."))
class GASCOURSE_API UGASCourseGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	friend class UGASCourseAbilitySystemComponent;

public:

	UPROPERTY(BlueprintAssignable)
	FGASCourseAbilityCommitSignature OnAbilityCommitDelegate;

	UFUNCTION(BlueprintPure, Category = "GASCourse|Ability|Tags")
	void GetAbilityCooldownTags(FGameplayTagContainer& CooldownTags) const;
	
	UFUNCTION(BlueprintPure, Category = "GASCourse|Ability|Tags")
	void GetAbilityDurationTags(FGameplayTagContainer& DurationTags) const;
	
public:

	UGASCourseGameplayAbility(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category = "GASCourse|Ability")
	UGASCourseAbilitySystemComponent* GetGASCourseAbilitySystemComponentFromActorInfo() const;

	UFUNCTION(BlueprintCallable, Category = "GASCourse|Ability")
	AGASCoursePlayerController* GetGASCoursePlayerControllerFromActorInfo() const;

	UFUNCTION(BlueprintCallable, Category = "GASCourse|Ability")
	AController* GetControllerFromActorInfo() const;

	UFUNCTION(BlueprintCallable, Category = "GASCourse|Ability")
	AGASCoursePlayerCharacter* GetGASCouresPlayerCharacterFromActorInfo() const;

	EGASCourseAbilityActivationPolicy GetActivationPolicy() const { return ActivationPolicy; }

	void TryActivateAbilityOnSpawn(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec);

	/**Returns the type of ability, see EGASCourseAbilityType*/
	UFUNCTION(BlueprintCallable, Category="GASCourse|Ability")
	FORCEINLINE EGASCourseAbilityType GetAbilityType() const
	{
		return AbilityType;
	}

	//Callback for when applied duration effect is removed. Ability must be of type EGASCourseAbilityType::Duration
	UFUNCTION()
	void DurationEffectRemoved(const FGameplayEffectRemovalInfo& GameplayEffectRemovalInfo);

	/**
	 * @brief Helper function to get a reference to the Duration Effect class.
	 * @return 
	 */
	UFUNCTION(BlueprintCallable)
	UGameplayEffect* GetDurationGameplayEffect() const;

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

	/** Called when this ability is granted to the ability system component. */
	UFUNCTION(BlueprintImplementableEvent, Category = Ability, DisplayName = "OnAbilityAdded")
	void K2_OnAbilityAdded();

	/** Called when this ability is removed from the ability system component. */
	UFUNCTION(BlueprintImplementableEvent, Category = Ability, DisplayName = "OnAbilityRemoved")
	void K2_OnAbilityRemoved();

	/** Called when the ability system is initialized with a pawn avatar. */
	UFUNCTION(BlueprintImplementableEvent, Category = Ability, DisplayName = "OnPawnAvatarSet")
	void K2_OnPawnAvatarSet();


protected:

	// Defines how this ability is meant to activate.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GASCourse|Ability|Activation")
	EGASCourseAbilityActivationPolicy ActivationPolicy;

	// Defines how this ability is meant to activate.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GASCourse|Ability")
	EGASCourseAbilityType AbilityType;

	//The duration effect to be applied, dictates how long the ability will last for.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GASCourse|Ability|Duration", meta=(EditCondition="AbilityType==EGASCourseAbilityType::Duration", EditConditionHides))
	TSubclassOf<UGameplayEffect> DurationEffect;

	/*Auto apply the duration effect on ability activation, otherwise use function Apply Duration Effect**/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GASCourse|Ability|Duration", meta=(EditCondition="AbilityType==EGASCourseAbilityType::Duration", EditConditionHides))
	bool bAutoApplyDurationEffect;

	/*Manually apply either class duration effect, or custom duration effect**/
	UFUNCTION(BlueprintCallable, Category = "GASCourse|Ability|Duration")
	UPARAM(DisplayName= "bDurationEffectApplied") bool ApplyDurationEffect();
	

	/**
	 * @brief Should the ability automatically commit when activated? If false, blueprint or child classes must call CommitAbility() manually.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GASCourse|Ability")
	bool bAutoCommitAbilityOnActivate;

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

private:
	
	FActiveGameplayEffectHandle DurationEffectHandle;
	
};
