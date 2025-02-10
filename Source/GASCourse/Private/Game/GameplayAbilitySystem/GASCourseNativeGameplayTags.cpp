// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GameplayAbilitySystem/GASCourseNativeGameplayTags.h"
#include "NativeGameplayTags.h"

UE_DEFINE_GAMEPLAY_TAG(InputTag_Move, "Input.NativeAction.Move")
UE_DEFINE_GAMEPLAY_TAG(InputTag_PointClickMovement, "Input.NativeAction.PointClickMovement")
UE_DEFINE_GAMEPLAY_TAG(InputTag_Look_Stick, "Input.NativeAction.GamepadLook")
UE_DEFINE_GAMEPLAY_TAG(InputTag_Jump, "Input.NativeAction.Jump")
UE_DEFINE_GAMEPLAY_TAG(InputTag_Evade, "Input.NativeAction.Evade")
UE_DEFINE_GAMEPLAY_TAG(InputTag_WeaponPrimaryFire, "Input.NativeAction.PrimaryWeaponFire")
UE_DEFINE_GAMEPLAY_TAG(InputTag_Crouch, "Input.NativeAction.Crouch")
UE_DEFINE_GAMEPLAY_TAG(InputTag_AbilityOne, "Input.NativeAction.Ability.One")
UE_DEFINE_GAMEPLAY_TAG(InputTag_AbilityTwo, "Input.NativeAction.Ability.Two")
UE_DEFINE_GAMEPLAY_TAG(InputTag_AbilityThree, "Input.NativeAction.Ability.Three")
UE_DEFINE_GAMEPLAY_TAG(InputTag_EquipmentAbilityOne, "Input.NativeAction.Ability.Equipment.One")
UE_DEFINE_GAMEPLAY_TAG(InputTag_EquipmentAbilityTwo, "Input.NativeAction.Ability.Equipment.Two")
UE_DEFINE_GAMEPLAY_TAG(InputTag_MovementAbility, "Input.NativeAction.Ability.Movement")
UE_DEFINE_GAMEPLAY_TAG(InputTag_ConfirmTargetData, "Input.NativeAction.ConfirmTargeting")
UE_DEFINE_GAMEPLAY_TAG(InputTag_CancelTargetData, "Input.NativeAction.CancelTargeting")

UE_DEFINE_GAMEPLAY_TAG(InputTag_MoveCamera, "Input.NativeAction.Camera.Movement")
UE_DEFINE_GAMEPLAY_TAG(InputTag_RecenterCamera, "Input.NativeAction.Camera.Recenter")
UE_DEFINE_GAMEPLAY_TAG(InputTag_RotateCamera, "Input.NativeAction.Camera.Rotate")
UE_DEFINE_GAMEPLAY_TAG(InputTag_RotateCameraAxis, "Input.NativeAction.Camera.Rotate.Axis")
UE_DEFINE_GAMEPLAY_TAG(InputTag_CameraZoom, "Input.NativeAction.Camera.Zoom")
UE_DEFINE_GAMEPLAY_TAG(InputTag_CameraMovementChordedAction, "Input.NativeAction.Camera.Enable.Movement.Chorded")
UE_DEFINE_GAMEPLAY_TAG(InputTag_CameraRotationChordedAction, "Input.NativeAction.Camera.Enable.Rotation.Chorded")

UE_DEFINE_GAMEPLAY_TAG(Status_Crouching, "Status.Crouching")
UE_DEFINE_GAMEPLAY_TAG(Status_Falling, "Status.Falling")
UE_DEFINE_GAMEPLAY_TAG(Status_IsMoving, "Status.IsMoving")
UE_DEFINE_GAMEPLAY_TAG(Status_Block_PointClickMovementInput, "Status.Block.Input.PointClickMovement")
UE_DEFINE_GAMEPLAY_TAG(Status_Gameplay_Targeting, "Status.Gameplay.Targeting")
UE_DEFINE_GAMEPLAY_TAG(Status_Block_MovementInput, "Status.Block.Input.Movement")
UE_DEFINE_GAMEPLAY_TAG(Status_Block_RotationInput, "Status.Block.Input.Rotation")
UE_DEFINE_GAMEPLAY_TAG(Status_Block_AbilityInput, "Status.Block.Input.AbilityActivation")
UE_DEFINE_GAMEPLAY_TAG(Status_Death, "Status.Death")
UE_DEFINE_GAMEPLAY_TAG(Status_CanMoveInterrupt, "Status.Movement.CanInterrupt")

UE_DEFINE_GAMEPLAY_TAG(Data_IncomingDamage, "Data.IncomingDamage")
UE_DEFINE_GAMEPLAY_TAG(Data_IncomingHealing, "Data.IncomingHealing")
UE_DEFINE_GAMEPLAY_TAG(Data_CachedDamage, "Data.CachedDamage")
UE_DEFINE_GAMEPLAY_TAG(Data_DamageOverTime, "Data.DamageOverTime")
UE_DEFINE_GAMEPLAY_TAG(DamageType_Physical, "Damage.Type.Physical")
UE_DEFINE_GAMEPLAY_TAG(DamageType_Elemental, "Damage.Type.Elemental")
UE_DEFINE_GAMEPLAY_TAG(DamageType_Elemental_Fire, "Damage.Type.Elemental.Fire")
UE_DEFINE_GAMEPLAY_TAG(DamageType_Status, "Damage.Type.Status")
UE_DEFINE_GAMEPLAY_TAG(DamageType_Status_Burn, "Damage.Type.Status.Burn")
UE_DEFINE_GAMEPLAY_TAG(DamageType_Critical, "Damage.Type.Critical")

UE_DEFINE_GAMEPLAY_TAG(Event_OnDeath, "Event.Gameplay.OnDeath")
UE_DEFINE_GAMEPLAY_TAG(Event_OnStatusDeath, "Event.Gameplay.Status.OnDeath")
UE_DEFINE_GAMEPLAY_TAG(Event_Gameplay_OnDamageDealt, "Event.Gameplay.OnDamageDealt")
UE_DEFINE_GAMEPLAY_TAG(Event_Gameplay_OnDamageReceived, "Event.Gameplay.OnDamageReceived")
UE_DEFINE_GAMEPLAY_TAG(Event_Gameplay_OnHit, "Event.Gameplay.OnHit")
UE_DEFINE_GAMEPLAY_TAG(Event_Gameplay_OnHealing, "Event.Gameplay.OnHealing")


UE_DEFINE_GAMEPLAY_TAG(Collision_IgnorePawn, "Collision.Ignore.Pawn")


UE_DEFINE_GAMEPLAY_TAG(AbilityActivationFail_IsDead, "Ability.Activation.Failure.IsDead")
UE_DEFINE_GAMEPLAY_TAG(AbilityActivationFail_OnCooldown, "Ability.Activation.Failure.OnCooldown")
UE_DEFINE_GAMEPLAY_TAG(AbilityActivationFail_CantAffordCost, "Ability.Activation.Failure.CantAffordCost")
UE_DEFINE_GAMEPLAY_TAG(AbilityActivationFail_BlockedByTags, "Ability.Activation.Failure.BlockedByTags")
UE_DEFINE_GAMEPLAY_TAG(AbilityActivationFail_MissingTags, "Ability.Activation.Failure.MissingTags")
UE_DEFINE_GAMEPLAY_TAG(AbilityActivationFail_Networking, "Ability.Activation.Failure.Networking")

UE_DEFINE_GAMEPLAY_TAG(Event_AbilityActivation_Fail, "Event.Ability.Activation.Failure");
UE_DEFINE_GAMEPLAY_TAG(Event_AbilityActivation_CantAffordCost, "Event.Ability.Activation.CantAffordCost");


FGASCourseNativeGameplayTags FGASCourseNativeGameplayTags::GameplayTags;
