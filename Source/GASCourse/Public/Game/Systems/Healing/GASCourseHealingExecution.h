// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayEffectExecutionCalculation.h"
#include "GASCourseHealingExecution.generated.h"

/**
 * \class UGASCourseHealingExecution
 * \brief Handles the execution for custom healing gameplay effects.
 *
 * This class is responsible for implementing the logic required to calculate and apply healing effects during execution of gameplay effects.
 * It captures relevant attributes associated with healing coefficients, current, and maximum health, and uses them in its calculations to apply healing.
 */
UCLASS()
class GASCOURSE_API UGASCourseHealingExecution : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
	
	UGASCourseHealingExecution();

	/** \brief Executes the custom gameplay effect implementation.
	 *
	 * This method is invoked when a gameplay effect with a custom execution is applied. It calculates and applies the damage to the target actor,
	 * taking into account any damage modifiers and tags. It also broadcasts the damage dealt event to the target and source ability system components.
	 *
	 * \param ExecutionParams The parameters for the execution of the gameplay effect.
	 * \param OutExecutionOutput The output data of the execution of the gameplay effect.
	 */
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
	
	
};
