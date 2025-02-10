// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "Engine/DeveloperSettings.h"
#include "UGASC_AbilitySystemSettings.generated.h"

/**
 * UGASC_AbilitySystemSettings
 *
 * This class provides configurable settings for the ability system within the GASCourse framework.
 * It is derived from UDeveloperSettings to allow customization via the engine's settings interface.
 *
 * Configurable Properties:
 * - PassiveResourceBlocker: Specifies a gameplay effect class to be used as a passive resource blocker in the ability system.
 */
UCLASS(Config=Game, defaultconfig, meta = (DisplayName="GAS Course Ability System Globals"))
class GASCOURSE_API UGASC_AbilitySystemSettings : public UDeveloperSettings
{
	GENERATED_BODY()
public:
	/**
	 * PassiveResourceBlocker
	 *
	 * Represents a configurable gameplay effect class used as a passive resource blocker within the ability system.
	 * This property is part of the UGASC_AbilitySystemSettings class and enables customization of resource blocking
	 * mechanics in GASCourse.
	 */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "GASCourse|Ability System|Resource")
	TSubclassOf<UGameplayEffect> PassiveResourceBlocker;

	UGASC_AbilitySystemSettings();
};
