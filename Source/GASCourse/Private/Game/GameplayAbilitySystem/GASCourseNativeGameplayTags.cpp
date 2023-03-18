// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GameplayAbilitySystem/GASCourseNativeGameplayTags.h"
#include "GameplayTagsManager.h"

FGASCourseNativeGameplayTags FGASCourseNativeGameplayTags::GameplayTags;

void FGASCourseNativeGameplayTags::InitializeNativeTags()
{
	UGameplayTagsManager& GameplayTagsManager = UGameplayTagsManager::Get();

	GameplayTags.AddAllTags(GameplayTagsManager);

	GameplayTagsManager.DoneAddingNativeTags();
}

void FGASCourseNativeGameplayTags::AddAllTags(UGameplayTagsManager& Manager)
{
	AddTag(InputTag_Move, "Input.NativeAction.Move", "Move input.");
	AddTag(InputTag_Look_Mouse, "Input.NativeAction.MouseLook", "Look (mouse) input.");
	AddTag(InputTag_Look_Stick, "Input.NativeAction.GamepadLook", "Look (stick) input.");
	AddTag(InputTag_Jump, "Input.NativeAction.Jump", "Jump input");
	AddTag(InputTag_Fire, "Input.NativeAction.WeaponFire", "Fire input.");
	AddTag(InputTag_Crouch, "Input.NativeAction.Crouch", "Crouch input");

	AddTag(Status_Crouching, "Status.Crouching", "Character is crouching");
	AddTag(Status_Falling, "Status.Falling", "Character is falling");
	AddTag(Status_IsMoving, "Status.IsMoving", "Character is moving, via input methods");
	AddTag(Status_BlockMovementInput, "Status.MovementInputBlocked", "Character cannot move, via input");
	AddTag(Status_AbilityInputBlocked, "Status.AbilityInputBlocked", "Character cannot use any abilities");
	AddTag(Status_Death, "Status.Death", "Character is dead.");
}

void FGASCourseNativeGameplayTags::AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment)
{
	OutTag = UGameplayTagsManager::Get().AddNativeGameplayTag(FName(TagName), FString(TEXT("(Native) ")) + FString(TagComment));
}
