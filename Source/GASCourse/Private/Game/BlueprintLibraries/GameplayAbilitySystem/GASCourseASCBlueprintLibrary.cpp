// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/BlueprintLibraries/GameplayAbilitySystem/GASCourseASCBlueprintLibrary.h"
#include "Game/GameplayAbilitySystem/GASCourseAbilitySystemComponent.h"
#include "Game/Systems/Damage/GASCourseDamageExecution.h"
#include "Game/GameplayAbilitySystem/GASCourseNativeGameplayTags.h"
#include "Game/GameplayAbilitySystem/GASCourseGameplayEffect.h"

bool UGASCourseASCBlueprintLibrary::ApplyDamageToTarget(AActor* Target, AActor* Instigator, float Damage, const FDamageContext& DamageContext)
{
	return ApplyDamageToTarget_Internal(Target, Instigator, Damage, DamageContext);
}

bool UGASCourseASCBlueprintLibrary::ApplyPhysicalDamageToTarget(AActor* Target, AActor* Instigator, float Damage,
	const FHitResult& HitResult, FDamageContext& DamageContext)
{
	DamageContext.DamageType = DamageType_Physical;
	DamageContext.HitResult = HitResult;
	return ApplyDamageToTarget_Internal(Target, Instigator, Damage, DamageContext);
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
	return ApplyDamageToTarget_Internal(Target, Instigator, Damage, DamageContext);
}

bool UGASCourseASCBlueprintLibrary::ApplyDamageToTarget_Internal(AActor* Target, AActor* Instigator, float Damage,
                                                                 const FDamageContext& DamageContext)
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
			UGASCourseGameplayEffect* DamageEffect = NewObject<UGASCourseGameplayEffect>(GetTransientPackage(), FName(TEXT("Damage")));
			DamageEffect->DurationPolicy = EGameplayEffectDurationType::Instant;

			const int32 ExecutionIdx = DamageEffect->Executions.Num();
			DamageEffect->Executions.SetNum(ExecutionIdx + 1);
			FGameplayEffectExecutionDefinition& DamageInfo = DamageEffect->Executions[ExecutionIdx];

			const TSubclassOf<UGASCourseDamageExecution> DamageExecutionBPClass = LoadClass<UGASCourseDamageExecution>(GetTransientPackage(), TEXT("/Game/GASCourse/Game/Systems/Damage/DamageExecution_Base.DamageExecution_Base_C"));
			if (DamageExecutionBPClass->GetClass() != nullptr)
			{
				DamageInfo.CalculationClass = DamageExecutionBPClass;
			}
			
			const int32 ModifiersIdx = DamageInfo.CalculationModifiers.Num();
			DamageInfo.CalculationModifiers.SetNum(ModifiersIdx + 1);
			FGameplayEffectExecutionScopedModifierInfo& DamageModifiers = DamageInfo.CalculationModifiers[ModifiersIdx];
			DamageModifiers.ModifierOp = EGameplayModOp::Additive;
			
			FSetByCallerFloat CallerFloat;
			CallerFloat.DataName = FName("");
			CallerFloat.DataTag = Data_IncomingDamage;
			DamageModifiers.ModifierMagnitude = FGameplayEffectModifierMagnitude(CallerFloat);
		
			DamageEffect->Executions[0].CalculationModifiers[0] = DamageModifiers;
			const FGameplayEffectSpecHandle DamageEffectHandle = MakeSpecHandle(DamageEffect, Instigator, Instigator, 1.0f);
			AssignTagSetByCallerMagnitude(DamageEffectHandle, Data_IncomingDamage, Damage);
			
			FGameplayEffectContextHandle ContextHandle = GetEffectContext(DamageEffectHandle);
			if(DamageContext.HitResult.bBlockingHit)
			{
				ContextHandle.AddHitResult(DamageContext.HitResult);
			}
			if(!DamageContext.GrantedTags.IsEmpty())
			{
				AddGrantedTags(DamageEffectHandle, DamageContext.GrantedTags);
			}
			InstigatorASC->ApplyGameplayEffectSpecToTarget(*DamageEffectHandle.Data.Get(), TargetASC);
		}
	}
	
	return true;
}
