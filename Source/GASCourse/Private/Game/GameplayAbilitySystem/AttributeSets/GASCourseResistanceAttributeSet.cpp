// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GameplayAbilitySystem/AttributeSets/GASCourseResistanceAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

UGASCourseResistanceAttributeSet::UGASCourseResistanceAttributeSet()
{
}

void UGASCourseResistanceAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	if(Attribute == GetMaxResistanceAttribute())
	{
		AdjustAttributeForMaxChange(Resistance, MaxResistance, NewValue, GetResistanceAttribute());
	}

	if(Attribute == GetResistanceAttribute())
	{
		NewValue = FMath::Clamp<float>(NewValue, 0.0f, MaxResistance.GetCurrentValue());
	}
}

void UGASCourseResistanceAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue,
	float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
}

void UGASCourseResistanceAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
}

void UGASCourseResistanceAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASCourseResistanceAttributeSet, Resistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASCourseResistanceAttributeSet, MaxResistance, COND_None, REPNOTIFY_Always);;
}

void UGASCourseResistanceAttributeSet::OnRep_Resistance(const FGameplayAttributeData& OldResistance)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGASCourseResistanceAttributeSet, Resistance, OldResistance);
}

void UGASCourseResistanceAttributeSet::OnRep_MaxResistance(const FGameplayAttributeData& OldMaxResistance)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGASCourseResistanceAttributeSet, Resistance, OldMaxResistance);
}
