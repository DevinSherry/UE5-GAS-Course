// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Systems/Damage/GASCourseDamageExecution.h"
#include "GASCourseAbilitySystemComponent.h"
#include "Game/GameplayAbilitySystem/AttributeSets/GASCourseHealthAttributeSet.h"

struct GASCourseDamageStatics
{
	
	DECLARE_ATTRIBUTE_CAPTUREDEF(IncomingDamage);

	GASCourseDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGASCourseHealthAttributeSet, IncomingDamage, Source, true);
	}
};

static const GASCourseDamageStatics& DamageStatics()
{
	static GASCourseDamageStatics DStatics;
	return DStatics;
}

UGASCourseDamageExecution::UGASCourseDamageExecution()
{
	RelevantAttributesToCapture.Add(DamageStatics().IncomingDamageDef);
}

void UGASCourseDamageExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	
	UAbilitySystemComponent* TargetAbilitySystemComponent = ExecutionParams.GetTargetAbilitySystemComponent();
	UAbilitySystemComponent* SourceAbilitySystemComponent = ExecutionParams.GetSourceAbilitySystemComponent();

	AActor* SourceActor = SourceAbilitySystemComponent ? SourceAbilitySystemComponent->GetAvatarActor() : nullptr;
	AActor* TargetActor = TargetAbilitySystemComponent ? TargetAbilitySystemComponent->GetAvatarActor() : nullptr;

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	// Gather the tags from the source and target as that can affect which buffs should be used
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Damage = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().IncomingDamageDef, EvaluationParameters, Damage);

	// Add SetByCaller damage if it exists
	Damage += FMath::Max<float>(Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.IncomingDamage")), false, -1.0f), 0.0f);

	float UnmitigatedDamage = Damage; // Can multiply any damage boosters here
	
	float MitigatedDamage = UnmitigatedDamage;

	if (MitigatedDamage > 0.f)
	{
		// Set the Target's damage meta attribute
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics().IncomingDamageProperty, EGameplayModOp::Additive, MitigatedDamage));
	}

	// Broadcast damages to Target ASC
	if (UGASCourseAbilitySystemComponent* TargetASC = Cast<UGASCourseAbilitySystemComponent>(TargetAbilitySystemComponent))
	{
		UGASCourseAbilitySystemComponent* SourceASC = Cast<UGASCourseAbilitySystemComponent>(SourceAbilitySystemComponent);
		FGameplayEventData DamageDealtPayload;
		DamageDealtPayload.Instigator = SourceAbilitySystemComponent->GetOwnerActor();
		DamageDealtPayload.Target = TargetAbilitySystemComponent->GetOwnerActor();
		DamageDealtPayload.EventMagnitude = MitigatedDamage;
		SourceAbilitySystemComponent->HandleGameplayEvent(FGameplayTag::RequestGameplayTag(FName("Event.Gameplay.OnDamageDealt")), &DamageDealtPayload);
	}
}