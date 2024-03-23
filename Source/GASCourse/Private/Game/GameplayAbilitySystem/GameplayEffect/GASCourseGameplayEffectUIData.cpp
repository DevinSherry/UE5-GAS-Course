// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GameplayAbilitySystem/GameplayEffect/GASCourseGameplayEffectUIData.h"
#include "GameplayEffect.h"
#include "Game/GameplayAbilitySystem/GameplayEffect/EffectDescriptor/GASCourseEffectDescriptor.h"


UGASCourseGameplayEffectUIData::UGASCourseGameplayEffectUIData()
{

}

FText UGASCourseGameplayEffectUIData::ConstructStatusDescription(FActiveGameplayEffectHandle GameplayEffectHandle)
{
	if(EffectDescriptorObj)
	{
		StatusDescription = EffectDescriptorObj->GetEffectDescriptor(GameplayEffectHandle);
	}
	else
	{
		const UGameplayEffect* ParentGameplayEffect = GetOwner();
		const FText EffectPeriod = ParentGameplayEffect->DurationMagnitude.GetValueForEditorDisplay();
		StatusDescription = EffectPeriod;
	}
	return StatusDescription;
}

UGASCourseEffectDescriptor* UGASCourseGameplayEffectUIData::InitializeDescriptor(UObject* WorldContextObject)
{
	if(EffectDescriptor)
	{
		EffectDescriptorObj = NewObject<UGASCourseEffectDescriptor>(WorldContextObject, EffectDescriptor);
		return EffectDescriptorObj;
	}

	return nullptr;
}

void UGASCourseGameplayEffectUIData::OnGameplayEffectExecuted(FActiveGameplayEffectsContainer& ActiveGEContainer,
                                                              FGameplayEffectSpec& GESpec, FPredictionKey& PredictionKey) const
{
	Super::OnGameplayEffectExecuted(ActiveGEContainer, GESpec, PredictionKey);
}
