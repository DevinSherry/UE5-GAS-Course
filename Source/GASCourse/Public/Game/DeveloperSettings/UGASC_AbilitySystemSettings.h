// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

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
UCLASS(Config=Game, defaultconfig, meta = (DisplayName="GASCourse Ability System Settings"))
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

	/**
	 * HapticFeedback_AbilityActivationFail
	 *
	 * Represents the force feedback effect class to be used when an ability activation fails.
	 * This property is part of the UGASC_AbilitySystemSettings class and is configurable through
	 * the engine's settings interface. It allows for customization of haptic feedback to enhance
	 * the player experience by providing tactile feedback in failure scenarios within the ability system.
	 */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "GASCourse|Ability System|Abilities|Haptic Feedback")
	TSoftObjectPtr<UForceFeedbackEffect> HapticFeedback_AbilityActivationFail;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "GASCourse|Health", meta=(ClampMin="0.0", ClampMax="1.0"))
	float LowHealthPercentage;

	UGASC_AbilitySystemSettings();
};
