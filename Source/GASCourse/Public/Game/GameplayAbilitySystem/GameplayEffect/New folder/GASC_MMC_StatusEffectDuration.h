// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "GASC_MMC_StatusEffectDuration.generated.h"

/**
 * 
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
