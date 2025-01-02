// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GameplayAbilitySystem/GameplayEffect/New folder/GASC_MMC_StatusEffectDuration.h"
#include "GameplayEffectExecutionCalculation.h"
#include "Game/GameplayAbilitySystem/AttributeSets/GASC_DurationAttributeSet.h"

UGASC_MMC_StatusEffectDuration::UGASC_MMC_StatusEffectDuration()
{
	//ManaDef defined in header FGameplayEffectAttributeCaptureDefinition ManaDef;
	DurationMultiplierAttributeDef.AttributeToCapture = UGASC_DurationAttributeSet::GetStatusEffectDurationMultiplierAttribute();
	DurationMultiplierAttributeDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	DurationMultiplierAttributeDef.bSnapshot = true;
	//MaxManaDef defined in header FGameplayEffectAttributeCaptureDefinition MaxManaDef;
	DurationReductionMultiplierAttributeDef.AttributeToCapture = UGASC_DurationAttributeSet::GetStatusEffectDurationReductionMultiplierAttribute();
	DurationReductionMultiplierAttributeDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	DurationReductionMultiplierAttributeDef.bSnapshot = true;

	RelevantAttributesToCapture.Add(DurationMultiplierAttributeDef);
	RelevantAttributesToCapture.Add(DurationReductionMultiplierAttributeDef);
}

float UGASC_MMC_StatusEffectDuration::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// Gather the tags from the source and target as that can affect which buffs should be used
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float DurationMultiplier = 0.f;
	GetCapturedAttributeMagnitude(DurationMultiplierAttributeDef, Spec, EvaluationParameters, DurationMultiplier);
	DurationMultiplier = FMath::Max<float>(DurationMultiplier, 0.0f);

	float DurationReductionMultiplier = 0.f;
	GetCapturedAttributeMagnitude(DurationReductionMultiplierAttributeDef, Spec, EvaluationParameters, DurationReductionMultiplier);
	DurationReductionMultiplier = FMath::Max<float>(DurationReductionMultiplier, 0.0f); // Avoid divide by zero

	float Reduction = FMath::Max<float>(DurationMultiplier - DurationReductionMultiplier, 0.0f); // ( DurationMultiplier - DurationReductionMultiplier
	
	return Reduction;
}
