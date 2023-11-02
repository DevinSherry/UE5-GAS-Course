// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Character/Components/GASCourseMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Game/Character/Player/GASCoursePlayerState.h"
#include "Game/GameplayAbilitySystem/GASCourseNativeGameplayTags.h"
#include "GASCourse/GASCourseCharacter.h"

namespace GASCharacter
{
	static float GroundTraceDistance = 100000.0f;
	FAutoConsoleVariableRef CVar_GroundTraceDistance(TEXT("LyraCharacter.GroundTraceDistance"), GroundTraceDistance, TEXT("Distance to trace down when generating ground information."), ECVF_Cheat);
}

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
		GASCourseASC->SetLooseGameplayTagCount(Status_Falling, 1);	
	}
	else
	{
		GASCourseASC->SetLooseGameplayTagCount(Status_Falling, 0);
	}

	Super::SetMovementMode(NewMovementMode, NewCustomMode);
}

const FGASCharacterGroundInfo& UGASCourseMovementComponent::GetGroundInfo()
{
	if (!CharacterOwner || (GFrameCounter == CachedGroundInfo.LastUpdateFrame))
	{
		return CachedGroundInfo;
	}

	if (MovementMode == MOVE_Walking)
	{
		CachedGroundInfo.GroundHitResult = CurrentFloor.HitResult;
		CachedGroundInfo.GroundDistance = 0.0f;
	}
	else
	{
		const UCapsuleComponent* CapsuleComp = CharacterOwner->GetCapsuleComponent();
		check(CapsuleComp);

		const float CapsuleHalfHeight = CapsuleComp->GetUnscaledCapsuleHalfHeight();
		const ECollisionChannel CollisionChannel = (UpdatedComponent ? UpdatedComponent->GetCollisionObjectType() : ECC_Pawn);
		const FVector TraceStart(GetActorLocation());
		const FVector TraceEnd(TraceStart.X, TraceStart.Y, (TraceStart.Z - GASCharacter::GroundTraceDistance - CapsuleHalfHeight));

		FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(GASCourseMovementComponent_GetGroundInfo), false, CharacterOwner);
		FCollisionResponseParams ResponseParam;
		InitCollisionParams(QueryParams, ResponseParam);

		FHitResult HitResult;
		GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, CollisionChannel, QueryParams, ResponseParam);

		CachedGroundInfo.GroundHitResult = HitResult;
		CachedGroundInfo.GroundDistance = GASCharacter::GroundTraceDistance;

		if (MovementMode == MOVE_NavWalking)
		{
			CachedGroundInfo.GroundDistance = 0.0f;
		}
		else if (HitResult.bBlockingHit)
		{
			CachedGroundInfo.GroundDistance = FMath::Max((HitResult.Distance - CapsuleHalfHeight), 0.0f);
		}
	}

	CachedGroundInfo.LastUpdateFrame = GFrameCounter;

	return CachedGroundInfo;
}

float UGASCourseMovementComponent::GetMaxSpeed() const
{
	const AGASCourseCharacter* Owner = Cast<AGASCourseCharacter>(GetOwner());
	if(Owner->IsPlayerControlled())
	{
		const AGASCoursePlayerState* PS = Cast<AGASCoursePlayerState>(Owner->GetPlayerState());
		if (!Owner || !PS)
		{
			UE_LOG(LogTemp, Error, TEXT("%s() No Owner"), *FString(__FUNCTION__));
			return Super::GetMaxSpeed();
		}
	}
	
	switch(MovementMode)
	{
	case MOVE_Walking:
	case MOVE_NavWalking:
	return IsCrouching() ? Owner->GetCrouchSpeed() : MaxWalkSpeed;
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
	if(Owner->IsPlayerControlled())
	{
		const AGASCoursePlayerState* PS = Cast<AGASCoursePlayerState>(Owner->GetPlayerState());
		if (!Owner || !PS)
		{
			UE_LOG(LogTemp, Error, TEXT("%s() No Owner"), *FString(__FUNCTION__));
			return Super::GetMaxJumpHeight();
		}
	}
	
	const float Gravity = GetGravityZ();
	if (FMath::Abs(Gravity) > UE_KINDA_SMALL_NUMBER)
	{
		return FMath::Square(Owner->GetJumpZVelocityOverride()) / (-2.f * Gravity);
	}

	
	return 0.f;
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
	if(Owner->IsPlayerControlled())
	{
		const AGASCoursePlayerState* PS = Cast<AGASCoursePlayerState>(Owner->GetPlayerState());
		if (!Owner || !PS)
		{
			UE_LOG(LogTemp, Error, TEXT("%s() No Owner"), *FString(__FUNCTION__));
			return Super::DoJump(bReplayingMoves);
		}
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
