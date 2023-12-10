// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayEffectExecutionCalculation.h"
#include "GASCourseDamageExecution.generated.h"

/**
 * 
 */
UCLASS()
class UGASCourseDamageExecution : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	
	UGASCourseDamageExecution();

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
