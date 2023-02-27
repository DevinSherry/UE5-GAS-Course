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
}

void FGASCourseNativeGameplayTags::AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment)
{
	OutTag = UGameplayTagsManager::Get().AddNativeGameplayTag(FName(TagName), FString(TEXT("(Native) ")) + FString(TagComment));
}
