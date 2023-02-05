// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GameInstance/GASCourseGameInstance.h"

#include "AbilitySystemGlobals.h"

void UGASCourseGameInstance::Init()
{
	Super::Init();
	UAbilitySystemGlobals::Get().InitGlobalData();
}
