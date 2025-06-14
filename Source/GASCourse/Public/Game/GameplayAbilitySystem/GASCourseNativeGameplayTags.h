// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayTagContainer.h"
#include "NativeGameplayTags.h"

class UGameplayTagsManager;

/**
 * Singleton containing native gameplay tags.
 */

UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Move);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_PointClickMovement);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Look_Stick);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Jump);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Evade);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_WeaponPrimaryFire);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Crouch);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_CameraZoom);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_AbilityOne);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_AbilityTwo);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_AbilityThree);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_EquipmentAbilityOne);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_EquipmentAbilityTwo);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_MovementAbility);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_ConfirmTargetData);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_CancelTargetData);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_ToggleStance);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_BlockStance);

UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_MoveCamera);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_RecenterCamera);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_RotateCameraAxis);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_RotateCamera);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_CameraMovementChordedAction);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_CameraRotationChordedAction);

UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Crouching);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Falling);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_IsMoving);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_CanMoveInterrupt);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Block_MovementInput);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Block_RotationInput);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Block_AbilityInput);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Gameplay_Targeting);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Block_PointClickMovementInput);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Death);

UE_DECLARE_GAMEPLAY_TAG_EXTERN(Data_IncomingDamage);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Data_IncomingHealing);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Data_CachedDamage);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Data_DamageOverTime);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(DamageType_Physical);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(DamageType_Elemental);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(DamageType_Elemental_Fire);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(DamageType_Status);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(DamageType_Status_Burn);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(DamageType_Critical);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(DamageType_Healing);

UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_OnDeath);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_OnStatusDeath);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Gameplay_OnDamageDealt);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Gameplay_OnDamageReceived);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Gameplay_OnHit);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Gameplay_OnHealing);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Gameplay_OnTargetHealed);

UE_DECLARE_GAMEPLAY_TAG_EXTERN(Collision_IgnorePawn);

//Ability Activation fails

UE_DECLARE_GAMEPLAY_TAG_EXTERN(AbilityActivationFail_IsDead);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(AbilityActivationFail_OnCooldown);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(AbilityActivationFail_CantAffordCost);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(AbilityActivationFail_BlockedByTags);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(AbilityActivationFail_MissingTags);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(AbilityActivationFail_Networking);

UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_AbilityActivation_Fail);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_AbilityActivation_CantAffordCost);

struct FGASCourseNativeGameplayTags
{
	
	static const FGASCourseNativeGameplayTags& Get() { return GameplayTags; }

private:

	static FGASCourseNativeGameplayTags GameplayTags;
};