// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GameplayAbilitySystem/GASCourseAbilitySystemComponent.h"

UGASCourseAbilitySystemComponent::UGASCourseAbilitySystemComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ReplicationMode = EGameplayEffectReplicationMode::Mixed;
}
