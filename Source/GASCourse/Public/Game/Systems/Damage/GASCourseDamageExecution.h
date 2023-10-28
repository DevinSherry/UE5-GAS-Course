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

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
	
};
