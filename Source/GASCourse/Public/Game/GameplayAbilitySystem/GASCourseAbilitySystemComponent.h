// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Game/GameplayAbilitySystem/GASAbilityTagRelationshipMapping.h"
#include "GASCourseAbilitySystemComponent.generated.h"

/**
 * 
 */

UINTERFACE(meta = (CannotImplementInterfaceInBlueprint))
class GASCOURSE_API UGCAbilitySystemReplicationProxyInterface : public UAbilitySystemReplicationProxyInterface
{
	GENERATED_BODY()
};

struct FGameplayAbilityRepAnimMontage;

class GASCOURSE_API IGCAbilitySystemReplicationProxyInterface : public IAbilitySystemReplicationProxyInterface
{
	GENERATED_BODY()

public:

	virtual FGameplayAbilityRepAnimMontage& Call_GetRepAnimMontageInfo_Mutable() = 0;

	virtual void Call_OnRep_ReplicatedAnimMontage() = 0;
};

UCLASS(ClassGroup=AbilitySystem, hidecategories=(Object,LOD,Lighting,Transform,Sockets,TextureStreaming), editinlinenew, meta=(BlueprintSpawnableComponent))
class GASCOURSE_API UGASCourseAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:

	UGASCourseAbilitySystemComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	virtual void InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor) override;
	
	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);

	virtual void AbilitySpecInputPressed(FGameplayAbilitySpec& Spec) override;
	virtual void AbilitySpecInputReleased(FGameplayAbilitySpec& Spec) override;

	void ProcessAbilityInput(float DeltaTime, bool bGamePaused);
	void ClearAbilityInput();

	/** Gets the ability target data associated with the given ability handle and activation info */
	void GetAbilityTargetData(const FGameplayAbilitySpecHandle AbilityHandle, FGameplayAbilityActivationInfo ActivationInfo,
		FGameplayAbilityTargetDataHandle& OutTargetDataHandle) const;

	virtual void OnRegister() override;
	
	/** Sets the current tag relationship mapping, if null it will clear it out */
	void SetTagRelationshipMapping(UGASAbilityTagRelationshipMapping* NewMapping);
	
	/** Looks at ability tags and gathers additional required and blocking tags */
	void GetAdditionalActivationTagRequirements(const FGameplayTagContainer& AbilityTags, FGameplayTagContainer& OutActivationRequired, FGameplayTagContainer& OutActivationBlocked) const;

	// Replication proxy helpers and accesors - CRITICAL TO KEEP UPDATED ON MAJOR REVISIONS
	IGCAbilitySystemReplicationProxyInterface* GetExtendedReplicationInterface();

	void ReplicatedAnimMontageOnRepAccesor();

	void SetRepAnimMontageInfoAccessor(const FGameplayAbilityRepAnimMontage& NewRepAnimMontageInfo);

	virtual float PlayMontage(UGameplayAbility* AnimatingAbility, FGameplayAbilityActivationInfo ActivationInfo, UAnimMontage* Montage, float InPlayRate, FName StartSectionName = NAME_None, float StartTimeSeconds = 0.0f) override;

	virtual void CurrentMontageStop(float OverrideBlendOutTime /*= -1.0f*/) override;

	void WaitForAbilityCooldownEnd(UGameplayAbility* InAbility, const FActiveGameplayEffectHandle InCooldownActiveGEHandle);

protected:

	// Handles to abilities that had their input pressed this frame.
	TArray<FGameplayAbilitySpecHandle> InputPressedSpecHandles;

	// Handles to abilities that had their input released this frame.
	TArray<FGameplayAbilitySpecHandle> InputReleasedSpecHandles;

	// Handles to abilities that have their input held.
	TArray<FGameplayAbilitySpecHandle> InputHeldSpecHandles;
	
	// If set, this table is used to look up tag relationships for activate and cancel
	UPROPERTY()
	TObjectPtr<UGASAbilityTagRelationshipMapping> AbilityTagRelationshipMapping;
	
	void TryActivateAbilitiesOnSpawn();
	
	virtual void ApplyAbilityBlockAndCancelTags(const FGameplayTagContainer& AbilityTags, UGameplayAbility* RequestingAbility, bool bEnableBlockTags, const FGameplayTagContainer& BlockTags, bool bExecuteCancelTags, const FGameplayTagContainer& CancelTags) override;
	virtual void HandleChangeAbilityCanBeCanceled(const FGameplayTagContainer& AbilityTags, UGameplayAbility* RequestingAbility, bool bCanBeCanceled) override;
	
};
