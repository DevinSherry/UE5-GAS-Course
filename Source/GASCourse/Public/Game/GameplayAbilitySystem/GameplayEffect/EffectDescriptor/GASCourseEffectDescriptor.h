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

	UFUNCTION(BlueprintNativeEvent)
	FText GetEffectDescriptor(FActiveGameplayEffectHandle GameplayEffectHandle, FGameplayEffectSpecHandle InSpecHandle);
	
	virtual UWorld* GetWorld() const override;

};
