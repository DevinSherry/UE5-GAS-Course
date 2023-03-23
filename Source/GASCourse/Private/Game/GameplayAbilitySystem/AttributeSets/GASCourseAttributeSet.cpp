// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GameplayAbilitySystem/AttributeSets/GASCourseAttributeSet.h"
#include "Net/UnrealNetwork.h"

UGASCourseAttributeSet::UGASCourseAttributeSet()
{
}

void UGASCourseAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UGASCourseAttributeSet, OneAttribute, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASCourseAttributeSet, TwoAttribute, COND_None, REPNOTIFY_Always);
}

void UGASCourseAttributeSet::OnRep_OneAttribute(const FGameplayAttributeData& OldOneAttribute)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGASCourseAttributeSet, OneAttribute, OldOneAttribute);
}

void UGASCourseAttributeSet::OnRep_TwoAttribute(const FGameplayAttributeData& OldTwoAttribute)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGASCourseAttributeSet, TwoAttribute, OldTwoAttribute);
}
