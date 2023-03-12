// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Game/GameplayAbilitySystem/GASCourseAbilitySystemComponent.h"
#include "Game/GameplayAbilitySystem/AttributeSets/GASCourseCharBaseAttributeSet.h"
#include "GASCourseCharacter.generated.h"

class UGASCourseGameplayAbilitySet;

UCLASS(config=Game)
class AGASCourseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

public:
	AGASCourseCharacter(const class FObjectInitializer& ObjectInitializer);

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);
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
	
protected:
	
	// To add mapping context
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Getter=GetAbilitySystemComponent)
	UGASCourseAbilitySystemComponent* AbilitySystemComponent = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Abilities")
	UGASCourseGameplayAbilitySet* DefaultAbilities;
	
	void GrantDefaultAbilitySet() const;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	virtual UGASCourseAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintCallable, Category = "GASCourse|Character|Attributes")
	float GetMovementSpeed() const;
	
	UFUNCTION(BlueprintCallable, Category = "GASCourse|Character|Attributes")
	float GetCrouchSpeed() const;

	UFUNCTION(BlueprintCallable, Category = "GASCiyrse|Character|Attributes")
	float GetJumpZVelocityOverride() const;
};

