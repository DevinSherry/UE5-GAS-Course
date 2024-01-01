// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/BlueprintLibraries/GameplayAbilitySystem/GASCourseASCBlueprintLibrary.h"
#include "Game/GameplayAbilitySystem/GASCourseAbilitySystemComponent.h"
#include "Game/Systems/Damage/GASCourseDamageExecution.h"
#include "Game/GameplayAbilitySystem/GASCourseNativeGameplayTags.h"
#include "Game/GameplayAbilitySystem/GASCourseGameplayEffect.h"

bool UGASCourseASCBlueprintLibrary::ApplyDamageToTarget(AActor* Target, AActor* Instigator, float Damage, const FDamageContext& DamageContext)
{
	//Initialize DoTContext to default values to make damage instant.
	constexpr FDamageOverTimeContext DamageOverTimeContext;
	UGameplayEffect* DamageEffect = ConstructDamageGameplayEffect(EGameplayEffectDurationType::Instant, DamageOverTimeContext);
	return ApplyDamageToTarget_Internal(Target, Instigator, Damage, DamageContext, DamageEffect);
}

bool UGASCourseASCBlueprintLibrary::ApplyDamageOverTimeToTarget(AActor* Target, AActor* Instigator, float Damage,
	const FDamageContext& DamageContext, const FDamageOverTimeContext& DamageOverTimeContext)
{
	UGameplayEffect* DamageEffect = ConstructDamageGameplayEffect(EGameplayEffectDurationType::HasDuration, DamageOverTimeContext);
	return ApplyDamageToTarget_Internal(Target, Instigator, Damage, DamageContext, DamageEffect);
}

bool UGASCourseASCBlueprintLibrary::ApplyPhysicalDamageToTarget(AActor* Target, AActor* Instigator, float Damage,
                                                                const FHitResult& HitResult, FDamageContext& DamageContext)
{
	DamageContext.DamageType = DamageType_Physical;
	DamageContext.HitResult = HitResult;
	constexpr FDamageOverTimeContext DamageOverTimeContext;
	UGameplayEffect* DamageEffect = ConstructDamageGameplayEffect(EGameplayEffectDurationType::Instant, DamageOverTimeContext);
	return ApplyDamageToTarget_Internal(Target, Instigator, Damage, DamageContext, DamageEffect);
}

bool UGASCourseASCBlueprintLibrary::ApplyFireDamageToTarget(AActor* Target, AActor* Instigator, float Damage,
                                                            const FHitResult& HitResult, FDamageContext& DamageContext, bool bApplyBurnStack)
{
	DamageContext.DamageType = DamageType_Elemental_Fire;
	if(bApplyBurnStack)
	{
		FGameplayTagContainer GrantedTags;
		GrantedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Effect.Gameplay.Status.Burn.Stack")));
		DamageContext.GrantedTags = GrantedTags;
	}
	DamageContext.HitResult = HitResult;
	
	constexpr FDamageOverTimeContext DamageOverTimeContext;
	UGameplayEffect* DamageEffect = ConstructDamageGameplayEffect(EGameplayEffectDurationType::Instant, DamageOverTimeContext);
	
	return ApplyDamageToTarget_Internal(Target, Instigator, Damage, DamageContext, DamageEffect);
}

bool UGASCourseASCBlueprintLibrary::ApplyDamageToTarget_Internal(AActor* Target, AActor* Instigator, float Damage,
                                                                 const FDamageContext& DamageContext, UGameplayEffect* GameplayEffect)
{
	if(!Instigator && !Target)
	{
		return false;
	}

	//TODO: Add check to verify ability system component + consider damage/health interface for Non-GAS actors
	if(UGASCourseAbilitySystemComponent* TargetASC = Target->GetComponentByClass<UGASCourseAbilitySystemComponent>())
	{
		if(UGASCourseAbilitySystemComponent* InstigatorASC = Instigator->GetComponentByClass<UGASCourseAbilitySystemComponent>())
		{
			if(UGASCourseGameplayEffect* DamageEffect = Cast<UGASCourseGameplayEffect>(GameplayEffect))
			{
							
				const int32 ExecutionIdx = DamageEffect->Executions.Num();
				DamageEffect->Executions.SetNum(ExecutionIdx + 1);
				FGameplayEffectExecutionDefinition& DamageInfo = DamageEffect->Executions[ExecutionIdx];

				const TSubclassOf<UGASCourseDamageExecution> DamageExecutionBPClass = LoadClass<UGASCourseDamageExecution>(GetTransientPackage(), TEXT("/Game/GASCourse/Game/Systems/Damage/DamageExecution_Base.DamageExecution_Base_C"));
				if (DamageExecutionBPClass->GetClass() != nullptr)
				{
					DamageInfo.CalculationClass = DamageExecutionBPClass;
				}
			
				int32 ModifiersIdx = DamageInfo.CalculationModifiers.Num();
				DamageInfo.CalculationModifiers.SetNum(ModifiersIdx + 2);
				FGameplayEffectExecutionScopedModifierInfo& DamageModifiers = DamageInfo.CalculationModifiers[ModifiersIdx];
				DamageModifiers.ModifierOp = EGameplayModOp::Additive;
			
				FSetByCallerFloat CallerFloat;
				CallerFloat.DataName = FName("");
				CallerFloat.DataTag = Data_IncomingDamage;
				DamageModifiers.ModifierMagnitude = FGameplayEffectModifierMagnitude(CallerFloat);
		
				DamageEffect->Executions[0].CalculationModifiers[0] = DamageModifiers;
				const FGameplayEffectSpecHandle DamageEffectHandle = MakeSpecHandle(DamageEffect, Instigator, Instigator, 1.0f);
				AssignTagSetByCallerMagnitude(DamageEffectHandle, Data_IncomingDamage, Damage);

				//TODO: Investigate how to add custom calculation class to damage application for randomization.
				/*
				FGameplayEffectExecutionScopedModifierInfo& DamageCalculationClass = DamageInfo.CalculationModifiers[++ModifiersIdx];
				DamageCalculationClass.ModifierOp = EGameplayModOp::Additive;
				*/
			
				FGameplayEffectContextHandle ContextHandle = GetEffectContext(DamageEffectHandle);
				if(DamageContext.HitResult.bBlockingHit)
				{
					ContextHandle.AddHitResult(DamageContext.HitResult);
				}
			
				AddGrantedTags(DamageEffectHandle, DamageContext.GrantedTags);
				AddGrantedTag(DamageEffectHandle, DamageContext.DamageType);
			
				InstigatorASC->ApplyGameplayEffectSpecToTarget(*DamageEffectHandle.Data.Get(), TargetASC);
				return true;
			}
		}
	}
	
	return false;
}

UGameplayEffect* UGASCourseASCBlueprintLibrary::ConstructDamageGameplayEffect(EGameplayEffectDurationType DurationType,  const FDamageOverTimeContext& DamageOverTimeContext)
{
	UGASCourseGameplayEffect* DamageEffect = NewObject<UGASCourseGameplayEffect>(GetTransientPackage(), FName(TEXT("Damage")));
	if(DurationType == EGameplayEffectDurationType::Instant)
	{
		DamageEffect->DurationPolicy = EGameplayEffectDurationType::Instant;
	}
	else
	{
		DamageEffect->DurationPolicy = EGameplayEffectDurationType::HasDuration;
				
		//DamageOverTimeContext should specify FScalableFloat for duration parameter.
		FScalableFloat Duration;
		Duration.Value = DamageOverTimeContext.DamageDuration;
		DamageEffect->DurationMagnitude = FGameplayEffectModifierMagnitude(Duration);

		//DamageOverTimeContext should specify FScalableFloat for period parameter.
		FScalableFloat Period;
		Period.Value = DamageOverTimeContext.DamagePeriod;
		DamageEffect->Period = Period;
		DamageEffect->bExecutePeriodicEffectOnApplication = DamageOverTimeContext.bApplyDamageOnApplication;
	}
	
	return DamageEffect;
}

bool UGASCourseASCBlueprintLibrary::FindDamageTypeTagInContainer(const FGameplayTagContainer& InContainer, FGameplayTag& DamageTypeTag)
{
	if(InContainer.HasTag(FGameplayTag::RequestGameplayTag(FName("Damage.Type"))))
	{
		for(FGameplayTag Tag : InContainer.GetGameplayTagArray())
		{
			if(Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Damage.Type"))))
			{
				DamageTypeTag = Tag;
				return true;
			}
		}
	}
	
	return false;
}
