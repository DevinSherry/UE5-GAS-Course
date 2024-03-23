// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayEffect.h"
#include "GASCourseEffectDescriptor.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class GASCOURSE_API UGASCourseEffectDescriptor : public UObject
{
	GENERATED_BODY()

public:

	UGASCourseEffectDescriptor();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FText GetEffectDescriptor(FActiveGameplayEffectHandle GameplayEffectHandle);
	
	virtual UWorld* GetWorld() const override;
	
};
