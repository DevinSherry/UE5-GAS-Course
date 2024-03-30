// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayEffectUIData.h"
#include "GASCourseEffectDescriptor.h"
#include "GASCourseGameplayEffectUIData.generated.h"

/**
 * 
 */
UCLASS()
class GASCOURSE_API UGASCourseGameplayEffectUIData : public UGameplayEffectUIData
{
	GENERATED_BODY()

public:

	UGASCourseGameplayEffectUIData();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Data)
	TSoftObjectPtr<UMaterialInterface> StatusIcon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Data, meta = (MultiLine = "true"))
	FText StatusDescription;

	UFUNCTION(BlueprintCallable)
	FText ConstructStatusDescription(FActiveGameplayEffectHandle GameplayEffectHandle);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Data)
	TSubclassOf<UGASCourseEffectDescriptor> EffectDescriptor;

	UFUNCTION(BlueprintCallable, meta = (WorldContext))
	UGASCourseEffectDescriptor* InitializeDescriptor(UObject* WorldContextObject);

private:

	UGASCourseEffectDescriptor* EffectDescriptorObj = nullptr;

	virtual void OnGameplayEffectExecuted(FActiveGameplayEffectsContainer& ActiveGEContainer, FGameplayEffectSpec& GESpec, FPredictionKey& PredictionKey) const override;
	
};
