// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayEffect.h"
#include "GASCourseEffectDescriptor.generated.h"

/**
 * @class UGASCourseEffectDescriptor
 * @brief This class is responsible for defining the effect descriptor for a gameplay effect in GAS Course.
 *
 * The UGASCourseEffectDescriptor class is a subclass of UObject and provides the functionality for getting the effect descriptor of a gameplay effect.
 * It allows for blueprint implementations of the GetEffectDescriptor function to provide custom effect descriptor text based on the specified GameplayEffectHandle.
 */
UCLASS(Blueprintable)
class GASCOURSE_API UGASCourseEffectDescriptor : public UObject
{
	GENERATED_BODY()

public:

	UGASCourseEffectDescriptor();

	UFUNCTION(BlueprintNativeEvent)
	FText GetEffectDescriptor(FActiveGameplayEffectHandle GameplayEffectHandle);
	
	virtual UWorld* GetWorld() const override;

};
