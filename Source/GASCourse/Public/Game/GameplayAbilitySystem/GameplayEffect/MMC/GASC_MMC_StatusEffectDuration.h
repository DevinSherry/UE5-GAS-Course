// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "GASC_MMC_StatusEffectDuration.generated.h"

/**
 * UGASC_MMC_StatusEffectDuration is a class derived from UGameplayModMagnitudeCalculation used to
 * calculate the magnitude of a gameplay effect that modifies the duration of status effects.
 *
 * The calculation is determined based on specific attributes, capturing the modifiers for
 * duration multiplier and duration reduction multiplier.
 */
UCLASS()
class GASCOURSE_API UGASC_MMC_StatusEffectDuration : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
	
	UGASC_MMC_StatusEffectDuration();

public:
	
	float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:
	
	FGameplayEffectAttributeCaptureDefinition DurationMultiplierAttributeDef;
	FGameplayEffectAttributeCaptureDefinition DurationReductionMultiplierAttributeDef;
};
