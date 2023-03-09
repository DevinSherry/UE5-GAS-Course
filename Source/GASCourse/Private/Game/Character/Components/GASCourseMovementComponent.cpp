// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Character/Components/GASCourseMovementComponent.h"

#include "GASCourse/GASCourseCharacter.h"

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
