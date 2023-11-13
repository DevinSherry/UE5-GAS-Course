// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GameplayAbilitySystem/GASCourseNativeGameplayTags.h"
#include "NativeGameplayTags.h"

UE_DEFINE_GAMEPLAY_TAG(InputTag_Move, "Input.NativeAction.Move")
UE_DEFINE_GAMEPLAY_TAG(InputTag_PointClickMovement, "Input.NativeAction.PointClickMovement")
UE_DEFINE_GAMEPLAY_TAG(InputTag_Look_Stick, "Input.NativeAction.GamepadLook")
UE_DEFINE_GAMEPLAY_TAG(InputTag_Jump, "Input.NativeAction.Jump")
UE_DEFINE_GAMEPLAY_TAG(InputTag_WeaponPrimaryFire, "Input.NativeAction.PrimaryWeaponFire")
UE_DEFINE_GAMEPLAY_TAG(InputTag_Crouch, "Input.NativeAction.Crouch")
UE_DEFINE_GAMEPLAY_TAG(InputTag_CameraZoom, "Input.NativeAction.CameraZoom")
UE_DEFINE_GAMEPLAY_TAG(InputTag_AbilityOne, "Input.NativeAction.Ability.One")
UE_DEFINE_GAMEPLAY_TAG(InputTag_AbilityTwo, "Input.NativeAction.Ability.Two")
UE_DEFINE_GAMEPLAY_TAG(InputTag_AbilityThree, "Input.NativeAction.Ability.Three")
UE_DEFINE_GAMEPLAY_TAG(InputTag_EquipmentAbilityOne, "Input.NativeAction.Ability.Equipment.One")
UE_DEFINE_GAMEPLAY_TAG(InputTag_EquipmentAbilityTwo, "Input.NativeAction.Ability.Equipment.Two")
UE_DEFINE_GAMEPLAY_TAG(InputTag_ConfirmTargetData, "Input.NativeAction.ConfirmTargeting")
UE_DEFINE_GAMEPLAY_TAG(InputTag_CancelTargetData, "Input.NativeAction.CancelTargeting")

UE_DEFINE_GAMEPLAY_TAG(InputTag_MoveCamera, "Input.NativeAction.MoveCamera")
UE_DEFINE_GAMEPLAY_TAG(InputTag_RecenterCamera, "Input.NativeAction.RecenterCamera")
UE_DEFINE_GAMEPLAY_TAG(InputTag_RotateCamera, "Input.NativeAction.RotateCamera")
UE_DEFINE_GAMEPLAY_TAG(InputTag_RotateCameraAxis, "Input.NativeAction.RotateCamera.Axis")

UE_DEFINE_GAMEPLAY_TAG(Status_Crouching, "Status.Crouching")
UE_DEFINE_GAMEPLAY_TAG(Status_Falling, "Status.Falling")
UE_DEFINE_GAMEPLAY_TAG(Status_IsMoving, "Status.IsMoving")
UE_DEFINE_GAMEPLAY_TAG(Status_Block_PointClickMovementInput, "Status.Block.Input.PointClickMovement")
UE_DEFINE_GAMEPLAY_TAG(Status_Gameplay_Targeting, "Status.Gameplay.Targeting")
UE_DEFINE_GAMEPLAY_TAG(Status_Block_MovementInput, "Status.Block.Input.Movement")
UE_DEFINE_GAMEPLAY_TAG(Status_Block_AbilityInput, "Status.Block.Input.AbilityActivation")
UE_DEFINE_GAMEPLAY_TAG(Status_Death, "Status.Death")

FGASCourseNativeGameplayTags FGASCourseNativeGameplayTags::GameplayTags;
