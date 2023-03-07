// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GASCourseMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class GASCOURSE_API UGASCourseMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:

	/*Can this character perform a jump action while in crouch state?*/
	UPROPERTY(Category="Character Movement (General Settings)", EditAnywhere, BlueprintReadWrite)
	bool bAllowJumpFromCrouch;

protected:

	virtual float GetMaxSpeed() const override;
	
};
