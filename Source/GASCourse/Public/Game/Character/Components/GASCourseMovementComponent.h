// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GASCourseMovementComponent.generated.h"

/**
 * FLyraCharacterGroundInfo
 *
 *	Information about the ground under the character.  It only gets updated as needed.
 */
USTRUCT(BlueprintType)
struct FGASCharacterGroundInfo
{
	GENERATED_BODY()

	FGASCharacterGroundInfo()
		: LastUpdateFrame(0)
		, GroundDistance(0.0f)
	{}

	uint64 LastUpdateFrame;

	UPROPERTY(BlueprintReadOnly)
	FHitResult GroundHitResult;

	UPROPERTY(BlueprintReadOnly)
	float GroundDistance;
};

/**
 * 
 */
UCLASS()
class GASCOURSE_API UGASCourseMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:

	virtual void SetMovementMode(EMovementMode NewMovementMode, uint8 NewCustomMode = 0) override;

	/*Can this character perform a jump action while in crouch state?*/
	UPROPERTY(Category="Character Movement (General Settings)", EditAnywhere, BlueprintReadWrite)
	bool bAllowJumpFromCrouch;

	// Returns the current ground info.  Calling this will update the ground info if it's out of date.
	UFUNCTION(BlueprintCallable, Category = "Lyra|CharacterMovement")
	const FGASCharacterGroundInfo& GetGroundInfo();

protected:
	
	virtual float GetMaxSpeed() const override;

	//TODO: Consider removing in favor of only DoJump()
	virtual float GetMaxJumpHeight() const override;
	//TODO: Consider removing in favor of only DoJump()
	virtual float GetMaxJumpHeightWithJumpTime() const override;
	
	virtual bool DoJump(bool bReplayingMoves) override;

protected:
	
	// Cached ground info for the character.  Do not access this directly!  It's only updated when accessed via GetGroundInfo().
	FGASCharacterGroundInfo CachedGroundInfo;
	
};
