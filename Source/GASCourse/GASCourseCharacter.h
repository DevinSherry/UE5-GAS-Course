// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once



#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "InputAction.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Game/Character/Player/GASCoursePlayerController.h"
#include "Game/GameplayAbilitySystem/GASCourseAbilitySystemComponent.h"
#include "Game/GameplayAbilitySystem/GASAbilityTagRelationshipMapping.h"
#include "Game/GameplayAbilitySystem/AttributeSets/GASCourseCharBaseAttributeSet.h"
#include "Tasks/Task.h"
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

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	
	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Champion, meta = (AllowPrivateAccess = "true"))
	class UGASCourseChampionComponent* ChampionComponent;

public:
	
	AGASCourseCharacter(const class FObjectInitializer& ObjectInitializer);
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	FReplicationProxyVarList& Call_GetReplicationProxyVarList_Mutable();

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);
	void StopMove(const FInputActionValue& Value);

	/** Called for left-click based movement */
	void PointClickMovement(const FInputActionValue& Value);
	void PointClickMovementStarted(const FInputActionValue& Value);
	void PointClickMovementCompleted(const FInputActionInstance& InputActionInstance);
	
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

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	
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

public:

	UE::Tasks::TTask<FVector> MultithreadTask;
	
	FVector GetWorldDirection(const FVector& CachedDirection) const;
	
};


