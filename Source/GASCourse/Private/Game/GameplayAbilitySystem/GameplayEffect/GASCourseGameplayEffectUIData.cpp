// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GameplayAbilitySystem/GameplayEffect/GASCourseGameplayEffectUIData.h"
#include "GameplayEffect.h"

FText UGASCourseGameplayEffectUIData::ConstructStatusDescription()
{
	const UGameplayEffect* ParentGameplayEffect = GetOwner();
	FText EffectPeriod = ParentGameplayEffect->DurationMagnitude.GetValueForEditorDisplay();
	StatusDescription = EffectPeriod;
	return StatusDescription;
}
