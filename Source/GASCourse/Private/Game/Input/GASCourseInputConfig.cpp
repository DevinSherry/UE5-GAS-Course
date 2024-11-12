// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Input/GASCourseInputConfig.h"
#include "GameplayTagContainer.h"
#include "InputAction.h"

const UInputAction* UGASCourseInputConfig::FindInputActionForTag(const FGameplayTag& InputTag) const
{
	for (const FTaggedInputAction& TaggedInputAction : TaggedInputActions)
	{
		if (TaggedInputAction.InputAction && TaggedInputAction.InputTag == InputTag)
		{
			return TaggedInputAction.InputAction;
		}
	}

	return nullptr;
}

const UInputAction* UGASCourseInputConfig::FindTaggedAbilityActionForTag(const FGameplayTag& InputTag) const
{
	for (const FTaggedAbilityAction& TaggedAbilityAction : TaggedAbilityActions)
	{
		if (TaggedAbilityAction.InputAction && TaggedAbilityAction.InputTag == InputTag)
		{
			return TaggedAbilityAction.InputAction;
		}
	}

	return nullptr;
}

const FGameplayTag& UGASCourseInputConfig::FindTagForAbilityAction(const UInputAction* InputAction) const
{
	for (const FTaggedAbilityAction& TaggedAbilityAction : TaggedAbilityActions)
	{
		if (TaggedAbilityAction.InputAction && TaggedAbilityAction.InputAction == InputAction)
		{
			return TaggedAbilityAction.InputTag;
		}
	}

	return FGameplayTag::EmptyTag;
}

const FGameplayTag& UGASCourseInputConfig::FindTagForInputAction(const UInputAction* InputAction) const
{
	for (const FTaggedInputAction& TaggedInputAction : TaggedInputActions)
	{
		if (TaggedInputAction.InputAction && TaggedInputAction.InputAction == InputAction)
		{
			return TaggedInputAction.InputTag;
		}
	}

	return FGameplayTag::EmptyTag;
}
