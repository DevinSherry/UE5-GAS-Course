// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GameplayAbilitySystem/AttributeSets/GASCoursePerksAttributeSet.h"
#include "Net/UnrealNetwork.h"

UGASCoursePerksAttributeSet::UGASCoursePerksAttributeSet()
{
}

void UGASCoursePerksAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
}

void UGASCoursePerksAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue,
	float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
}

void UGASCoursePerksAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
}

void UGASCoursePerksAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASCoursePerksAttributeSet, NumofRicochetBounces, COND_None, REPNOTIFY_Always);
}

void UGASCoursePerksAttributeSet::OnRep_NumofRicochetBounces(const FGameplayAttributeData& OldNumofRicochetBounces)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGASCoursePerksAttributeSet, NumofRicochetBounces, OldNumofRicochetBounces);
}
