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
	
	AimCast,
	
	Duration,
	
	Instant
};

/**
 * Represents the different slot types for a gameplay ability.
 *
 * The slot types are used to categorize abilities based on their usage.
 */
UENUM(BlueprintType)
enum class EGASCourseAbilitySlotType : uint8
{
	
	PrimarySlot,
	
	SecondarySlot,
	
	UltimateSlot,

	EmptySlot,
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

	/**
	 * @brief Get the granted by effect duration.
	 *
	 * This method returns the duration of the effect granted by the ability.
	 *
	 * @return The duration of the effect granted by the ability.
	 */
	UFUNCTION(BlueprintCallable, Category = Ability)
	float GetGrantedbyEffectDuration() const;
	
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

	/**
	 * @brief Get the type of ability slot.
	 *
	 * This method returns the type of ability slot that the ability belongs to.
	 *
	 * @return The ability slot type (EGASCourseAbilitySlotType) of the ability.
	 */
	EGASCourseAbilitySlotType GetAbilitySlotType() const {return AbilitySlotType; }

	void TryActivateAbilityOnSpawn(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) const;

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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GASCourse|Ability|Activation")
	EGASCourseAbilityType AbilityType;

	// Defines how this ability is meant to activate.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GASCourse|Ability|Activation")
	EGASCourseAbilitySlotType AbilitySlotType;

	/**
	 * @brief Should the ability automatically commit when activated? If false, blueprint or child classes must call CommitAbility() manually.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GASCourse|Ability")
	bool bAutoCommitAbilityOnActivate;
};
