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
	FText GetEffectDescriptor(FActiveGameplayEffectHandle GameplayEffectHandle, FGameplayEffectSpecHandle InSpecHandle);
	
	virtual UWorld* GetWorld() const override;

	//TODO: Make Spec Handle reference variable that is set on initialization.

};
