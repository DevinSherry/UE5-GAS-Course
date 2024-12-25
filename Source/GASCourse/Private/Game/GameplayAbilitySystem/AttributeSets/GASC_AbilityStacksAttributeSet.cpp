// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GameplayAbilitySystem/AttributeSets/GASC_AbilityStacksAttributeSet.h"
#include "Game/GameplayAbilitySystem/GASCourseNativeGameplayTags.h"
#include "GameplayEffectExtension.h"
#include "Game/GameplayAbilitySystem/GASCourseGameplayEffect.h"
#include "GASCourse/GASCourseCharacter.h"
#include "Net/UnrealNetwork.h"


UGASC_AbilityStacksAttributeSet::UGASC_AbilityStacksAttributeSet()
{
}

void UGASC_AbilityStacksAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if(Attribute == GetCurrentDodgeStackCountAttribute())
	{
		NewValue = FMath::Clamp<float>(NewValue, 0.0f, MaxDodgeStackCount.GetCurrentValue());
	}

	if(Attribute == GetStackDurationModifierAttribute())
	{
		NewValue = FMath::Clamp<float>(NewValue, 0.0f, 10.0f);
	}
}

void UGASC_AbilityStacksAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue,
	float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
}

void UGASC_AbilityStacksAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if(Data.EvaluatedData.Attribute == GetCurrentDodgeStackCountAttribute())
	{
		SetCurrentDodgeStackCount(FMath::Clamp(GetCurrentDodgeStackCount(), 0.0f, GetMaxDodgeStackCount()));
	}

	if(Data.EvaluatedData.Attribute == GetStackDurationModifierAttribute())
	{
		SetStackDurationModifier(FMath::Clamp(GetStackDurationModifier(), 0.0f, 10.0f));
	}
}

void UGASC_AbilityStacksAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASC_AbilityStacksAttributeSet, CurrentDodgeStackCount, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASC_AbilityStacksAttributeSet, MaxDodgeStackCount, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASC_AbilityStacksAttributeSet, StackDurationModifier, COND_None, REPNOTIFY_Always);
}

void UGASC_AbilityStacksAttributeSet::OnRep_CurrentDodgeStackCount(
	const FGameplayAttributeData& OldCurrentDodgeStackCount)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGASC_AbilityStacksAttributeSet, CurrentDodgeStackCount, OldCurrentDodgeStackCount);
}

void UGASC_AbilityStacksAttributeSet::OnRep_MaxDodgeStackCount(const FGameplayAttributeData& OldMaxDodgeStackCount)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGASC_AbilityStacksAttributeSet, MaxDodgeStackCount, OldMaxDodgeStackCount);
}

void UGASC_AbilityStacksAttributeSet::OnRep_StackDurationModifier(
	const FGameplayAttributeData& OldStackDurationModifier)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGASC_AbilityStacksAttributeSet, StackDurationModifier, OldStackDurationModifier);
}
