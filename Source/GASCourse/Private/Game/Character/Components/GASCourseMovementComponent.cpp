// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Character/Components/GASCourseMovementComponent.h"
#include "Game/GameplayAbilitySystem/GASCourseNativeGameplayTags.h"
#include "GASCourse/GASCourseCharacter.h"

void UGASCourseMovementComponent::SetMovementMode(EMovementMode NewMovementMode, uint8 NewCustomMode)
{
	const AGASCourseCharacter* Owner = Cast<AGASCourseCharacter>(GetOwner());
	if(!Owner)
	{
		return;
	}

	UGASCourseAbilitySystemComponent* GASCourseASC = Owner->GetAbilitySystemComponent();
	if(!GASCourseASC)
	{
		return;
	}
	if(NewMovementMode == EMovementMode::MOVE_Falling)
	{
		GASCourseASC->SetLooseGameplayTagCount(FGASCourseNativeGameplayTags::Get().Status_Falling, 1);	
	}
	else
	{
		GASCourseASC->SetLooseGameplayTagCount(FGASCourseNativeGameplayTags::Get().Status_Falling, 0);
	}

	Super::SetMovementMode(NewMovementMode, NewCustomMode);
}

float UGASCourseMovementComponent::GetMaxSpeed() const
{
	const AGASCourseCharacter* Owner = Cast<AGASCourseCharacter>(GetOwner());
	if (!Owner)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() No Owner"), *FString(__FUNCTION__));
		return Super::GetMaxSpeed();
	}
	
	switch(MovementMode)
	{
	case MOVE_Walking:
	case MOVE_NavWalking:
	return IsCrouching() ? Owner->GetCrouchSpeed() : Owner->GetMovementSpeed();
	case MOVE_Falling:
	return MaxWalkSpeed;
	case MOVE_Swimming:
	return MaxSwimSpeed;
	case MOVE_Flying:
	return MaxFlySpeed;
	case MOVE_Custom:
	return MaxCustomMovementSpeed;
	case MOVE_None:
	default:
	return 0.f;
	}

}

float UGASCourseMovementComponent::GetMaxJumpHeight() const
{
	const AGASCourseCharacter* Owner = Cast<AGASCourseCharacter>(GetOwner());
	if (!Owner)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() No Owner"), *FString(__FUNCTION__));
		return Super::GetMaxJumpHeight();
	}
	
	const float Gravity = GetGravityZ();
	if (FMath::Abs(Gravity) > UE_KINDA_SMALL_NUMBER)
	{
		return FMath::Square(Owner->GetJumpZVelocityOverride()) / (-2.f * Gravity);
	}
	else
	{
		return 0.f;
	}
}

float UGASCourseMovementComponent::GetMaxJumpHeightWithJumpTime() const
{
	const AGASCourseCharacter* Owner = Cast<AGASCourseCharacter>(GetOwner());
	if (!Owner)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() No Owner"), *FString(__FUNCTION__));
		return Super::GetMaxJumpHeightWithJumpTime();
	}
	
	const float MaxJumpHeight = GetMaxJumpHeight();
	if (CharacterOwner)
	{
		// When bApplyGravityWhileJumping is true, the actual max height will be lower than this.
		// However, it will also be dependent on framerate (and substep iterations) so just return this
		// to avoid expensive calculations.

		// This can be imagined as the character being displaced to some height, then jumping from that height.
		return (CharacterOwner->JumpMaxHoldTime * Owner->GetJumpZVelocityOverride()) + MaxJumpHeight;
	}

	return MaxJumpHeight;
}

bool UGASCourseMovementComponent::DoJump(bool bReplayingMoves)
{
	const AGASCourseCharacter* Owner = Cast<AGASCourseCharacter>(GetOwner());
	if (!Owner)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() No Owner"), *FString(__FUNCTION__));
		return Super::DoJump(bReplayingMoves);
	}
	if ( CharacterOwner && CharacterOwner->CanJump() )
	{
		// Don't jump if we can't move up/down.
		if (!bConstrainToPlane || FMath::Abs(PlaneConstraintNormal.Z) != 1.f)
		{
			Velocity.Z = FMath::Max<FVector::FReal>(Velocity.Z, Owner->GetJumpZVelocityOverride());
			SetMovementMode(MOVE_Falling);
			return true;
		}
	}
	return false;
}
