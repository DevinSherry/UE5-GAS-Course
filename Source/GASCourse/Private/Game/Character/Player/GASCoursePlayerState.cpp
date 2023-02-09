// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Character/Player/GASCoursePlayerState.h"

AGASCoursePlayerState::AGASCoursePlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UGASCourseAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
}

UGASCourseAbilitySystemComponent* AGASCoursePlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
