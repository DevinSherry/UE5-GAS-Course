// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Systems/Healing/GASCourseHealingExecution.h"
#include "Game/GameplayAbilitySystem/AttributeSets/GASCourseHealthAttributeSet.h"
#include "Game/GameplayAbilitySystem/GASCourseAbilitySystemComponent.h"
#include "Game/GameplayAbilitySystem/GASCourseGameplayEffect.h"
#include "Game/GameplayAbilitySystem/GASCourseNativeGameplayTags.h"

struct GASCourseHealingStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(IncomingHealing);
	DECLARE_ATTRIBUTE_CAPTUREDEF(StatusDamageHealingCoefficient)
	DECLARE_ATTRIBUTE_CAPTUREDEF(ElementalDamageHealingCoefficient)
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalDamageHealingCoefficient)
	DECLARE_ATTRIBUTE_CAPTUREDEF(AllDamageHealingCoefficient)
	DECLARE_ATTRIBUTE_CAPTUREDEF(CurrentHealth)
	DECLARE_ATTRIBUTE_CAPTUREDEF(MaxHealth)

	GASCourseHealingStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGASCourseHealthAttributeSet, IncomingHealing, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGASCourseHealthAttributeSet, StatusDamageHealingCoefficient, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGASCourseHealthAttributeSet, ElementalDamageHealingCoefficient, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGASCourseHealthAttributeSet, PhysicalDamageHealingCoefficient, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGASCourseHealthAttributeSet, AllDamageHealingCoefficient, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGASCourseHealthAttributeSet, CurrentHealth, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGASCourseHealthAttributeSet, MaxHealth, Source, true);
	}
};

static const GASCourseHealingStatics& HealingStatics()
{
	static GASCourseHealingStatics DStatics;
	return DStatics;
} 

UGASCourseHealingExecution::UGASCourseHealingExecution()
{
	RelevantAttributesToCapture.Add(HealingStatics().IncomingHealingDef);
	RelevantAttributesToCapture.Add(HealingStatics().StatusDamageHealingCoefficientDef);
	RelevantAttributesToCapture.Add(HealingStatics().ElementalDamageHealingCoefficientDef);
	RelevantAttributesToCapture.Add(HealingStatics().PhysicalDamageHealingCoefficientDef);
	RelevantAttributesToCapture.Add(HealingStatics().AllDamageHealingCoefficientDef);
	RelevantAttributesToCapture.Add(HealingStatics().CurrentHealthDef);
	RelevantAttributesToCapture.Add(HealingStatics().MaxHealthDef);
}

void UGASCourseHealingExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	UGASCourseAbilitySystemComponent* TargetAbilitySystemComponent = Cast<UGASCourseAbilitySystemComponent>(ExecutionParams.GetTargetAbilitySystemComponent());
	UGASCourseAbilitySystemComponent* SourceAbilitySystemComponent = Cast<UGASCourseAbilitySystemComponent>(ExecutionParams.GetSourceAbilitySystemComponent());

	AActor* SourceActor = SourceAbilitySystemComponent ? SourceAbilitySystemComponent->GetAvatarActor() : nullptr;
	AActor* TargetActor = TargetAbilitySystemComponent ? TargetAbilitySystemComponent->GetAvatarActor() : nullptr;

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	// Gather the tags from the source and target as that can affect which buffs should be used
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	/**
	 * Find and store relevant attributes for health and healing coefficients.
	 */
	
	float Healing = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(HealingStatics().IncomingHealingDef, EvaluationParameters, Healing);
	Healing += FMath::Max<float>(Spec.GetSetByCallerMagnitude(Data_IncomingHealing, true, -1.0f), 0.0f);

	float StatusHealingCoefficient = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(HealingStatics().StatusDamageHealingCoefficientDef, EvaluationParameters, StatusHealingCoefficient);

	float ElementalHealingCoefficient = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(HealingStatics().ElementalDamageHealingCoefficientDef, EvaluationParameters, ElementalHealingCoefficient);

	float PhysicalHealingCoefficient = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(HealingStatics().PhysicalDamageHealingCoefficientDef, EvaluationParameters, PhysicalHealingCoefficient);

	float AllHealingCoefficient = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(HealingStatics().AllDamageHealingCoefficientDef, EvaluationParameters, AllHealingCoefficient);

	float CurrentHealth = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(HealingStatics().CurrentHealthDef, EvaluationParameters, CurrentHealth);
	
	float MaxHealth = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(HealingStatics().MaxHealthDef, EvaluationParameters, MaxHealth);
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//Find and store relevant damage type gameplay tags for reference in coefficient calculation.
	FGameplayTagContainer DamageTypeTags;
	Spec.GetAllGrantedTags(DamageTypeTags);

	float TotalHealing = 0.0f;
	if(DamageTypeTags.HasTag(DamageType_Status))
	{
		TotalHealing += Healing * StatusHealingCoefficient;
	}
	
	if(DamageTypeTags.HasTag(DamageType_Physical))
	{
		TotalHealing += Healing * PhysicalHealingCoefficient;
	}
	
	if(DamageTypeTags.HasTag(DamageType_Elemental))
	{
		TotalHealing += Healing * ElementalHealingCoefficient;
	}
	
	TotalHealing += Healing * AllHealingCoefficient;
	if (TotalHealing > 0.f)
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(HealingStatics().IncomingHealingProperty, EGameplayModOp::Additive, TotalHealing));
		
		// Broadcast life steal event to Target ASC
		if (TargetAbilitySystemComponent && CurrentHealth != MaxHealth)
		{
			FGameplayEventData LifeStealDealtPayload;
			LifeStealDealtPayload.Instigator = SourceAbilitySystemComponent->GetAvatarActor();
			LifeStealDealtPayload.Target = TargetAbilitySystemComponent->GetAvatarActor();
			LifeStealDealtPayload.EventMagnitude = TotalHealing;
			LifeStealDealtPayload.ContextHandle = Spec.GetContext();
			LifeStealDealtPayload.InstigatorTags = Spec.DynamicGrantedTags;
			
			TargetAbilitySystemComponent->HandleGameplayEvent(Event_Gameplay_OnHealing, &LifeStealDealtPayload);
		}
	}
}
