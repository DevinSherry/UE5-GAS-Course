// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NativeGameplayTags.h"

class UGameplayTagsManager;

/**
 * Singleton containing native gameplay tags.
 */

UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Move);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_PointClickMovement)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Look_Mouse);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Look_Stick);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Jump);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_WeaponPrimaryFire);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Crouch);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_AbilityOne);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_AbilityTwo);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_AbilityThree);

UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Crouching);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Falling);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_IsMoving);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_BlockMovementInput);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_AbilityInputBlocked);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Death);

struct FGASCourseNativeGameplayTags
{
	
	static const FGASCourseNativeGameplayTags& Get() { return GameplayTags; }

private:

	static FGASCourseNativeGameplayTags GameplayTags;
};