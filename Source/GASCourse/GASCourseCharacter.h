// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Game/GameplayAbilitySystem/GASCourseAbilitySystemComponent.h"
#include "Game/GameplayAbilitySystem/GASAbilityTagRelationshipMapping.h"
#include "Game/GameplayAbilitySystem/AttributeSets/GASCourseCharBaseAttributeSet.h"
#include "Game/GameplayAbilitySystem/GameplayTagResponseTable/GASCourseStatusEffectTable.h"
#include "GASCourseCharacter.generated.h"

class UGASCourseGameplayAbilitySet;

USTRUCT(BlueprintType)
struct GASCOURSE_API FReplicationProxyVarList
{
	GENERATED_BODY()

public:

	FReplicationProxyVarList() :
		GameplayTagsBitMask(0),
		AttributeOne(0.0f),
		AttributeTwo(0.0f)
	{
		
	}

	void Copy(uint8 inGameplayTagsBitMask, float inAttributeOne, float inAttributeTwo)
	{
		GameplayTagsBitMask = inGameplayTagsBitMask;
		AttributeOne = inAttributeOne;
		AttributeTwo = inAttributeTwo;
	}
	
public:

	UPROPERTY()
	uint8 GameplayTagsBitMask;
	
	UPROPERTY()
	float AttributeOne;
	
	UPROPERTY()
	float AttributeTwo;
};


UCLASS(config=Game)
class AGASCourseCharacter : public ACharacter, public IAbilitySystemInterface, public IGCAbilitySystemReplicationProxyInterface
{
	GENERATED_BODY()


public:
	
	AGASCourseCharacter(const class FObjectInitializer& ObjectInitializer);
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	FReplicationProxyVarList& Call_GetReplicationProxyVarList_Mutable();

protected:

	/** Called for movement input */
	virtual void Move(const FInputActionValue& Value);
	void StopMove(const FInputActionValue& Value);
	
	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Called for crouch input */
	void Input_Crouch(const FInputActionValue& Value);

	//Override these functions to add loose gameplay tag for status.crouching
	virtual void OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;
	virtual void OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;

	//Override these functions in order to jump while crouched, if movement component allows for it.
	virtual bool CanJumpInternal_Implementation() const override;
	virtual void Jump() override;
	
	UFUNCTION()
	void OnRep_ReplicationVarList();
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ReplicationVarList)
	FReplicationProxyVarList ReplicationVarList;

	
protected:
	
	// To add mapping context
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Getter=GetAbilitySystemComponent)
	UGASCourseAbilitySystemComponent* AbilitySystemComponent = nullptr;

	void InitializeAbilitySystem(UGASCourseAbilitySystemComponent* InASC);
		
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Abilities")
	TObjectPtr<UGASCourseGameplayAbilitySet> DefaultAbilitySet;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	TObjectPtr<UGASAbilityTagRelationshipMapping> AbilityTagRelationshipMapping;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	TObjectPtr<UGASCourseStatusEffectTable> GameplayStatusEffectTable;

	/** Remove gameplay effects with these Asset Tags on Character Death */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GASCourse|GameplayTags", DisplayName = "GameplayEffectsToRemoveOnDeath")
	FGameplayTagContainer GameplayEffectAssetTagsToRemove;

public:
	
	void CharacterDeathGameplayEventCallback(FGameplayTag MatchingTag, const FGameplayEventData* Payload);
	
	virtual UGASCourseAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	UFUNCTION(BlueprintCallable, Category = "GASCourse|Character|Attributes")
	float GetCrouchSpeed() const;

	UFUNCTION(BlueprintCallable, Category = "GASCiyrse|Character|Attributes")
	float GetJumpZVelocityOverride() const;
	
	UFUNCTION()
	FORCEINLINE UGASCourseGameplayAbilitySet* GetDefaultAbilitySet() const
	{
		return (DefaultAbilitySet) ? DefaultAbilitySet : nullptr;
	}
	
	UFUNCTION()
	FORCEINLINE UGASAbilityTagRelationshipMapping* GetAbilityTagRelationshipMapping() const
	{
		return (AbilityTagRelationshipMapping) ? AbilityTagRelationshipMapping : nullptr;
	}

	UFUNCTION()
	FORCEINLINE UGASCourseStatusEffectTable* GetGameplayStatusEffectTable() const
	{
		return (GameplayStatusEffectTable) ? GameplayStatusEffectTable : nullptr;
	}
	
protected:

	UFUNCTION()
	virtual void Call_OnRep_ReplicatedAnimMontage() override;
	
	UPROPERTY(ReplicatedUsing = Call_OnRep_ReplicatedAnimMontage)
	FGameplayAbilityRepAnimMontage RepAnimMontageInfo;
	
	virtual void ForceReplication() override;

	UFUNCTION(NetMulticast, unreliable)
	virtual void NetMulticast_InvokeGameplayCueExecuted_FromSpec(const FGameplayEffectSpecForRPC Spec, FPredictionKey PredictionKey) override;

	UFUNCTION(NetMulticast, unreliable)
	virtual void NetMulticast_InvokeGameplayCueExecuted(const FGameplayTag GameplayCueTag, FPredictionKey PredictionKey, FGameplayEffectContextHandle EffectContext) override;

	UFUNCTION(NetMulticast, unreliable)
	virtual void NetMulticast_InvokeGameplayCuesExecuted(const FGameplayTagContainer GameplayCueTags, FPredictionKey PredictionKey, FGameplayEffectContextHandle EffectContext) override;

	UFUNCTION(NetMulticast, unreliable)
	virtual void NetMulticast_InvokeGameplayCueExecuted_WithParams(const FGameplayTag GameplayCueTag, FPredictionKey PredictionKey, FGameplayCueParameters GameplayCueParameters) override;

	UFUNCTION(NetMulticast, unreliable)
	virtual void NetMulticast_InvokeGameplayCuesExecuted_WithParams(const FGameplayTagContainer GameplayCueTags, FPredictionKey PredictionKey, FGameplayCueParameters GameplayCueParameters) override;

	UFUNCTION(NetMulticast, unreliable)
	virtual void NetMulticast_InvokeGameplayCueAdded(const FGameplayTag GameplayCueTag, FPredictionKey PredictionKey, FGameplayEffectContextHandle EffectContext) override;

	UFUNCTION(NetMulticast, unreliable)
	virtual void NetMulticast_InvokeGameplayCueAdded_WithParams(const FGameplayTag GameplayCueTag, FPredictionKey PredictionKey, FGameplayCueParameters Parameters) override;

	UFUNCTION(NetMulticast, unreliable)
	virtual void NetMulticast_InvokeGameplayCueAddedAndWhileActive_FromSpec(const FGameplayEffectSpecForRPC& Spec, FPredictionKey PredictionKey) override;

	UFUNCTION(NetMulticast, unreliable)
	virtual void NetMulticast_InvokeGameplayCueAddedAndWhileActive_WithParams(const FGameplayTag GameplayCueTag, FPredictionKey PredictionKey, FGameplayCueParameters GameplayCueParameters) override;

	UFUNCTION(NetMulticast, unreliable)
	virtual void NetMulticast_InvokeGameplayCuesAddedAndWhileActive_WithParams(const FGameplayTagContainer GameplayCueTags, FPredictionKey PredictionKey, FGameplayCueParameters GameplayCueParameters) override;
	  
	virtual FGameplayAbilityRepAnimMontage& Call_GetRepAnimMontageInfo_Mutable() override;
	
};


