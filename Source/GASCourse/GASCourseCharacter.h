// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "MotionWarpingComponent.h"
#include "MVVMViewModelBase.h"
#include "Game/GameplayAbilitySystem/GASCourseAbilitySystemComponent.h"
#include "Game/Character/Components/Health/GASC_HealthComponent.h"
#include "Game/GameplayAbilitySystem/GASAbilityTagRelationshipMapping.h"
#include "Game/GameplayAbilitySystem/AttributeSets/GASCourseCharBaseAttributeSet.h"
#include "Game/GameplayAbilitySystem/GameplayTagResponseTable/GASCourseStatusEffectTable.h"
//#include "CogCommonDebugFilteredActorInterface.h"

#include "GASCourseCharacter.generated.h"

class UGASCourseGameplayAbilitySet;
class UInputAction;

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

//--------------------------------------------------------------------------------------------------------------------------
USTRUCT(BlueprintType)
struct FActiveAbilityInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UGameplayAbility> Ability;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputAction* InputAction = nullptr;
};

//--------------------------------------------------------------------------------------------------------------------------
USTRUCT(BlueprintType)
struct FPassiveAbilityInfo
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UGameplayAbility> Ability;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool ActivateWhenGiven = false;
};

//--------------------------------------------------------------------------------------------------------------------------
USTRUCT(BlueprintType)
struct FCogSampleMontageTableRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	FCogSampleMontageTableRow() {}

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimMontage* Montage = nullptr;
};


UCLASS(config=Game)
class AGASCourseCharacter : public ACharacter, public IAbilitySystemInterface, public IGCAbilitySystemReplicationProxyInterface
//public ICogCommonDebugFilteredActorInterface
{
	GENERATED_BODY()

public:
	
	AGASCourseCharacter(const class FObjectInitializer& ObjectInitializer);
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	FReplicationProxyVarList& Call_GetReplicationProxyVarList_Mutable();

	void PossessedBy(AController* NewController) override;

	virtual void PostInitializeComponents() override;

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void UnPossessed() override;

	virtual void AcknowledgePossession(AController* NewController);

	virtual void AcknowledgeUnpossession();

	
	UFUNCTION()
	virtual bool SimulateInputActionFromBuffer(FGameplayTag InputTag);
	
protected:

	friend class AGASCoursePlayerController;

	UPROPERTY()
	AController* InitialController = nullptr;

	/** Called for movement input */
	virtual void Move(const FInputActionValue& Value);
	void StopMove(const FInputActionValue& Value);
	
	/** Called for looking input */
	virtual void Look(const FInputActionValue& Value);

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

	/**
	 * Sets the rotation of the character for the client.
	 *
	 * @param InRotation The new rotation of the character.
	 */
	UFUNCTION(BlueprintCallable, Reliable, Client)
	void SetCharacterRotation_Client(FRotator InRotation);

	/**
	 * Sets the rotation of the character on the server.
	 *
	 * @param InRotation The new rotation of the character.
	 */
	UFUNCTION(Reliable, Server)
	void SetCharacterRotation_Server(FRotator InRotation);

	/**
	 * Sets the rotation of the character for all clients.
	 *
	 * @param InRotation - The new rotation of the character.
	 */
	UFUNCTION(Reliable, NetMulticast)
	void SetCharacterRotation_Multicast(FRotator InRotation);

	/**
	 * @brief FRotator variable that represents the rotation to be applied to an object in order to face a given direction.
	 *
	 * This variable is decorated with UPROPERTY to ensure replication and provide read-only access. It is also marked as Transient to prevent saving its value in the editor.
	 * Use this variable to rotate an object based on a specified direction.
	 */
	UPROPERTY(Replicated, BlueprintReadOnly, Transient)
	FRotator RotateToDirection;

	
protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Getter=GetAbilitySystemComponent)
	UGASCourseAbilitySystemComponent* AbilitySystemComponent = nullptr;

	/** The component responsible for listening to status effect events.
	 *
	 * This component is used to listen to status effect events, such as when a status effect is applied or removed from the character.
	 * It is visible anywhere and has read-only access, and falls under the StatusEffects category.
	 * The meta flag AllowPrivateAccess is set to true, allowing private access to this component.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = StatusEffects, meta = (AllowPrivateAccess = "true"))
	class UGASCStatusEffectListenerComp* StatusEffectListenerComp;;

	/**
	 * @brief The CharacterHealthComponent variable represents the component responsible for handling the health functionality of the character.
	 *
	 * This variable is decorated with UPROPERTY to ensure replication and provide read-only access. It is also marked as EditAnywhere and BlueprintReadOnly, allowing it to be edited in the editor and accessed from blueprints. It falls under the StatusEffects category.
	 * The meta flag AllowPrivateAccess is set to true, allowing private access to this component.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = HealthComponent, meta = (AllowPrivateAccess = "true"))
	UGASC_HealthComponent* CharacterHealthComponent;

	/** The component responsible for handling the camera targeting functionality.
	 *
	 * This component is used for camera targeting in the game. It is visible anywhere and has read-only access, and falls under the Camera category.
	 * The meta flag AllowPrivateAccess is set to true, allowing private access to this component.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* TargetingSceneComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = HealthComponent, meta = (AllowPrivateAccess = "true"))
	UMotionWarpingComponent* MotionWarpComponent;

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

	UFUNCTION(BlueprintNativeEvent)
	void PostBeginPlayHealthComponentRegistration();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "View Models|Health")
	TSubclassOf<UMVVMViewModelBase> CharacterHealthViewModelContextClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "View Models|Health")
	FName CharacterHealthContextName;

public:

	/**
 * @brief Get the targeting scene component.
 *
 * This method returns the targeting scene component, which is responsible for handling the camera targeting functionality.
 * The component is visible anywhere and has read-only access.
 * It falls under the Camera category and is marked with the meta flag AllowPrivateAccess set to true.
 *
 * @return A pointer to the targeting scene component.
 */
	FORCEINLINE class USceneComponent* GetTargetingSceneComponent() const { return TargetingSceneComponent; }
	
	void CharacterDeathGameplayEventCallback(FGameplayTag MatchingTag, const FGameplayEventData* Payload);

	UFUNCTION()
	void IgnorePawnCollisionGameplayTagEventCallback(FGameplayTag MatchingTag, int32 NewCount);
	
	virtual UGASCourseAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	UFUNCTION(BlueprintCallable, Category = "GASCourse|Character|Attributes")
	float GetCrouchSpeed() const;

	UFUNCTION(BlueprintCallable, Category = "GASCiyrse|Character|Attributes")
	float GetJumpZVelocityOverride() const;

	UFUNCTION(BlueprintCallable, Category = "GASCiyrse|Character|Attributes")
	float GetAirControlOverride() const;

	UFUNCTION(BlueprintCallable, Category = "GASCourse|Character|Attributes")
	float GetCurrentHealth() const;
	
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

	UFUNCTION(Blueprintable)
	FORCEINLINE void SetCharacterDead(bool bInNewDeath)
	{
		bIsAlive = !bInNewDeath;
	}

	UFUNCTION(Blueprintable)
	FORCEINLINE bool IsCharacterAlive() const
	{
		return bIsAlive;
	}
	
protected:
	
	
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

	UFUNCTION()
	virtual void Call_OnRep_ReplicatedAnimMontage() override;

private:

	ECollisionResponse DefaultCollisionResponseToPawn;

	bool bIsAlive = true;
	
};


