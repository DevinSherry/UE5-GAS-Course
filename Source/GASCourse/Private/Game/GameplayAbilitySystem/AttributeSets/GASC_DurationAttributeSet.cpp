// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GameplayAbilitySystem/AttributeSets/GASC_DurationAttributeSet.h"
#include "Game/GameplayAbilitySystem/GASCourseNativeGameplayTags.h"
#include "GameplayEffectExtension.h"
#include "Game/GameplayAbilitySystem/GASCourseGameplayEffect.h"
#include "GASCourse/GASCourseCharacter.h"
#include "Net/UnrealNetwork.h"


UGASC_DurationAttributeSet::UGASC_DurationAttributeSet()
{
}

void UGASC_DurationAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if(Attribute == GetAbilityCooldownMultiplierAttribute())
	{
		NewValue = FMath::Clamp<float>(NewValue, 0.1f, 10.0f);
	}
	
	if(Attribute == GetAbilityCooldownMultiplierAttribute())
	{
		NewValue = FMath::Clamp<float>(NewValue, 0.1f, 10.0f);
	}
	
	if(Attribute == GetStatusEffectDurationMultiplierAttribute())
	{
		NewValue = FMath::Clamp<float>(NewValue, 0.1f, 10.0f);
	}
	
	if(Attribute == GetStatusEffectDurationReductionMultiplierAttribute())
	{
		NewValue = FMath::Clamp<float>(NewValue, 0.0f, 1.0f);
	}
}

void UGASC_DurationAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue,
	float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
}

void UGASC_DurationAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
}

void UGASC_DurationAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASC_DurationAttributeSet, AbilityDurationMultiplier, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASC_DurationAttributeSet, AbilityCooldownMultiplier, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASC_DurationAttributeSet, StatusEffectDurationMultiplier, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASC_DurationAttributeSet, StatusEffectDurationReductionMultiplier, COND_None, REPNOTIFY_Always);
}

void UGASC_DurationAttributeSet::OnRep_AbilityDurationMultiplier(
	const FGameplayAttributeData& OldAbilityDurationMultiplier)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGASC_DurationAttributeSet, AbilityDurationMultiplier, OldAbilityDurationMultiplier);
}

void UGASC_DurationAttributeSet::OnRep_AbilityCooldownMultiplier(
	const FGameplayAttributeData& OldAbilityCooldownMultiplier)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGASC_DurationAttributeSet, AbilityCooldownMultiplier, OldAbilityCooldownMultiplier);
}

void UGASC_DurationAttributeSet::OnRep_StatusEffectDurationMultiplier(
	const FGameplayAttributeData& OldStatusEffectDurationMultiplier)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGASC_DurationAttributeSet, StatusEffectDurationMultiplier, OldStatusEffectDurationMultiplier);
}


void UGASC_DurationAttributeSet::OnRep_StatusEffectDurationReductionMultiplier(
	const FGameplayAttributeData& OldStatusEffectDurationReductionMultiplier)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGASC_DurationAttributeSet, StatusEffectDurationReductionMultiplier, OldStatusEffectDurationReductionMultiplier);
}
