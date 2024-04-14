// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GameplayAbilitySystem/GameplayEffect/EffectDescriptor/GASCourseEffectDescriptor.h"

UGASCourseEffectDescriptor::UGASCourseEffectDescriptor()
{
}

FText UGASCourseEffectDescriptor::GetEffectDescriptor_Implementation(FActiveGameplayEffectHandle GameplayEffectHandle)
{
	FText Empty;
	return Empty;
}

UWorld* UGASCourseEffectDescriptor::GetWorld() const
{
	if(GetOuter() && !HasAnyFlags(RF_ClassDefaultObject))
	{
		return GetOuter()->GetWorld();
	}

	return nullptr;
}

