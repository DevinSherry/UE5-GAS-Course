// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Systems/Damage/GASCourseDamageExecution.h"
#include "Game/GameplayAbilitySystem/AttributeSets/GASCourseHealthAttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemGlobals.h"
#include "Game/GameplayAbilitySystem/GASCourseAbilitySystemComponent.h"
#include "Game/GameplayAbilitySystem/GASCourseGameplayEffect.h"
#include "Game/GameplayAbilitySystem/GASCourseNativeGameplayTags.h"
#include "Game/Systems/Healing/GASCourseHealingExecution.h"

struct GASCourseDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(IncomingDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalDamageMultiplier);

	GASCourseDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGASCourseHealthAttributeSet, IncomingDamage, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGASCourseHealthAttributeSet, CriticalChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGASCourseHealthAttributeSet, CriticalDamageMultiplier, Source, false);
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
	RelevantAttributesToCapture.Add(DamageStatics().CriticalChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalDamageMultiplierDef);
}

void UGASCourseDamageExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
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
	
	float Damage = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().IncomingDamageDef, EvaluationParameters, Damage);

	// Add SetByCaller damage if it exists
	Damage += FMath::Max<float>(Spec.GetSetByCallerMagnitude(Data_IncomingDamage, false, -1.0f), 0.0f);

	/*
	 * Critical Chance + Critical Damage
	 */
	float CriticalChance = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalChanceDef, EvaluationParameters, CriticalChance);
	
	float CriticalDamageMultiplier = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalDamageMultiplierDef, EvaluationParameters, CriticalDamageMultiplier);
	
	float RolledChancePercentage = FMath::RandRange(0.0f, 1.0f);
	bool bCriticalHit = RolledChancePercentage <= CriticalChance;
	if (bCriticalHit)
	{
		Damage += FMath::Floor(Damage * CriticalDamageMultiplier);
	}

	float UnmitigatedDamage = Damage; // Can multiply any damage boosters here
	
	float MitigatedDamage = UnmitigatedDamage;

	if (MitigatedDamage > 0.f)
	{
		// Set the Target's damage meta attribute
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics().IncomingDamageProperty, EGameplayModOp::Additive, MitigatedDamage));
	}

	UGASCourseGameplayEffect* IncomingHealingGameplayEffect = NewObject<UGASCourseGameplayEffect>(GetTransientPackage());
	IncomingHealingGameplayEffect->DurationPolicy = EGameplayEffectDurationType::Instant;
	if(UGASCourseGameplayEffect* HealingEffect = Cast<UGASCourseGameplayEffect>(IncomingHealingGameplayEffect))
	{
		FGameplayEffectExecutionDefinition HealingExecutionDefinition;
		HealingExecutionDefinition.CalculationClass = LoadClass<UGASCourseHealingExecution>(SourceActor, TEXT("/Game/GASCourse/Game/Systems/Healing/HealingExecution_Base.HealingExecution_Base_C"));
		if(HealingExecutionDefinition.CalculationClass)
		{
			HealingEffect->Executions.Emplace(HealingExecutionDefinition);
			FGameplayEffectContext* ContextHandle = UAbilitySystemGlobals::Get().AllocGameplayEffectContext();
			ContextHandle->AddInstigator(SourceActor, SourceActor);
			const FGameplayEffectSpecHandle HealingEffectHandle =FGameplayEffectSpecHandle(new FGameplayEffectSpec(HealingEffect, FGameplayEffectContextHandle(ContextHandle), 1.0f));
				//UAbilitySystemBlueprintLibrary::MakeSpecHandle(HealingEffect, SourceActor, SourceActor, 1.0f);
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(HealingEffectHandle, Data_IncomingHealing, MitigatedDamage);
			UAbilitySystemBlueprintLibrary::AddGrantedTags(HealingEffectHandle, Spec.DynamicGrantedTags);
			SourceAbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*HealingEffectHandle.Data.Get(), SourceAbilitySystemComponent);
		}
	}

	// Broadcast damages to Target ASC & SourceASC
	if (TargetAbilitySystemComponent && SourceAbilitySystemComponent)
	{
		FGameplayEventData DamageDealtPayload;
		DamageDealtPayload.Instigator = SourceAbilitySystemComponent->GetAvatarActor();
		DamageDealtPayload.Target = TargetAbilitySystemComponent->GetAvatarActor();
		DamageDealtPayload.EventMagnitude = MitigatedDamage;
		DamageDealtPayload.ContextHandle = Spec.GetContext();
		DamageDealtPayload.InstigatorTags = Spec.DynamicGrantedTags;
		if(Spec.GetContext().GetHitResult())
		{
			FHitResult HitResultFromContext = *Spec.GetContext().GetHitResult();
			DamageDealtPayload.TargetData = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromHitResult(HitResultFromContext); 
		}

		if (bCriticalHit)
		{
			DamageDealtPayload.InstigatorTags.AddTag(DamageType_Critical);
		}
		
		if(TargetAbilitySystemComponent->HasMatchingGameplayTag(Status_Death))
		{
			return;
		}
		
		SourceAbilitySystemComponent->HandleGameplayEvent(FGameplayTag::RequestGameplayTag(FName("Event.Gameplay.OnDamageDealt")), &DamageDealtPayload);
		TargetAbilitySystemComponent->HandleGameplayEvent(FGameplayTag::RequestGameplayTag(FName("Event.Gameplay.OnDamageReceived")), &DamageDealtPayload);

		//TODO: Instead of sending event, pass in status effect tag into gameplay status table
		TargetAbilitySystemComponent->ApplyGameplayStatusEffect(TargetAbilitySystemComponent, SourceAbilitySystemComponent, Spec.DynamicGrantedTags);
	}
}
 