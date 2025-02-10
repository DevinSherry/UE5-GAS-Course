// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GameplayAbilitySystem/GameplayEffect/MMC/GASC_MMC_AbilityCost.h"
#include "Game/GameplayAbilitySystem/GASCourseGameplayAbility.h"

float UGASC_MMC_AbilityCost::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	if (const UGASCourseGameplayAbility* Ability = Cast<UGASCourseGameplayAbility>(Spec.GetContext().GetAbilityInstance_NotReplicated()))
	{
		UE_LOG(LogAbilitySystemComponent, Warning, TEXT("Ability Cost Value at Level: %f"), Ability->GetAbilityCost().GetValueAtLevel(Ability->GetAbilityLevel()));
		return Ability->GetAbilityCost().GetValueAtLevel(Ability->GetAbilityLevel());
	}
	else
	{
		return 0.0f;
	}
}
