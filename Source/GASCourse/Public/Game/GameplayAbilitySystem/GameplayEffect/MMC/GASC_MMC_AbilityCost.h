// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "GASC_MMC_AbilityCost.generated.h"

/**
 * UGASC_MMC_AbilityCost is a class used for calculating the base magnitude of an ability's cost
 * within the gameplay ability system. This is a custom implementation derived from
 * UGameplayModMagnitudeCalculation.
 *
 * This class overrides the CalculateBaseMagnitude_Implementation function to provide specific
 * logic for determining the ability cost based on the ability instance.
 *
 * Key Functionality:
 * - It computes the ability cost by retrieving relevant data from the associated ability instance
 *   available in the Gameplay Effect Spec's context.
 * - Returns the cost of the ability at its current level if the ability is valid.
 * - If the ability instance is invalid, it returns a default value of 0.0.
 */
UCLASS()
class GASCOURSE_API UGASC_MMC_AbilityCost : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
	
	
public:

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
	
};
