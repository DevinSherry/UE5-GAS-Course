// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Game/GameplayAbilitySystem/GASCourseAbilitySystemComponent.h"
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
	AGASCourseCharacter();

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Called for crouch input */
	void Input_Crouch(const FInputActionValue& Value);
	
	virtual void OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;
	virtual void OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;
	
protected:
	
	// To add mapping context
	virtual void BeginPlay() override;
	
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
	
};

