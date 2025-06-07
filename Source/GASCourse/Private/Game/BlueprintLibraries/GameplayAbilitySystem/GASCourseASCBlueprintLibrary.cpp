// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/BlueprintLibraries/GameplayAbilitySystem/GASCourseASCBlueprintLibrary.h"
#include "AbilitySystemGlobals.h"
#include "Game/GameplayAbilitySystem/GASCourseAbilitySystemComponent.h"
#include "Game/Systems/Damage/GASCourseDamageExecution.h"
#include "Game/GameplayAbilitySystem/GASCourseNativeGameplayTags.h"
#include "Game/GameplayAbilitySystem/GASCourseGameplayEffect.h"

bool UGASCourseASCBlueprintLibrary::ApplyDamageToTarget(AActor* Target, AActor* Instigator, float Damage, const FDamageContext& DamageContext)
{
	//Initialize DoTContext to default values to make damage instant.
	constexpr FEffectOverTimeContext EffectOverTimeContext;
	if(UGameplayEffect* DamageEffect = ConstructDamageGameplayEffect(EGameplayEffectDurationType::Instant, EffectOverTimeContext))
	{
		return ApplyDamageToTarget_Internal(Target, Instigator, Damage, DamageContext, DamageEffect);
	}
	return false;
}

bool UGASCourseASCBlueprintLibrary::ApplyDamageToTargetDataHandle(FGameplayAbilityTargetDataHandle TargetHandle, AActor* Instigator, float Damage,
	FDamageContext DamageContext)
{
	TArray<AActor*> Targets = GetAllActorsFromTargetData(TargetHandle);
	bool bDamageApplied = false;

	for (int32 TargetIdx = 0; TargetIdx < Targets.Num(); ++TargetIdx)
	{
		if (!DamageContext.HitResult.bBlockingHit)
		{
			DamageContext.HitResult = GetHitResultFromTargetData(TargetHandle, TargetIdx);
		}
		bDamageApplied = ApplyDamageToTarget(Targets[TargetIdx], Instigator, Damage, DamageContext);
	}

	return bDamageApplied;
}

bool UGASCourseASCBlueprintLibrary::ApplyHealingToTarget_Internal(AActor* Target, AActor* Instigator, float InHealing,
	UGameplayEffect* GameplayEffect, FEffectOverTimeContext EffectOverTimeContext)
{
	if(!Instigator || !Target)
	{
		return false;
	}

	if(AGASCoursePlayerState* InstigatorPlayerState = Cast<AGASCoursePlayerState>(Instigator))
	{
		Instigator = InstigatorPlayerState->GetPawn();
	}

	if(AGASCoursePlayerState* TargetPlayerState = Cast<AGASCoursePlayerState>(Target))
	{
		Target = TargetPlayerState->GetPawn();
	}
	
	TSubclassOf<UGameplayEffectExecutionCalculation> HealingCalculationClass;
	if (const UGASC_AbilitySystemSettings* AbilitySystemSettings = GetDefault<UGASC_AbilitySystemSettings>())
	{
		HealingCalculationClass = AbilitySystemSettings->HealingExecution;
		if (!HealingCalculationClass)
		{
			UE_LOG(LogTemp, Warning, TEXT("Damage Calculation is not valid!"));
			return false;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Ability System Settings is not valid!"));
		return false;
	}
	
	if(AGASCourseCharacter* TargetCharacter = Cast<AGASCourseCharacter>(Target))
	{
		if(AGASCourseCharacter* InstigatorCharacter = Cast<AGASCourseCharacter>(Instigator))
		{
			UGASCourseAbilitySystemComponent* TargetASC = TargetCharacter->GetAbilitySystemComponent();
			check(TargetASC);

			UGASCourseAbilitySystemComponent* InstigatorASC = InstigatorCharacter->GetAbilitySystemComponent();
			check(InstigatorASC);
			
			if(UGASCourseGameplayEffect* HealingEffect = Cast<UGASCourseGameplayEffect>(GameplayEffect))
			{
				FGameplayEffectExecutionDefinition HealingExecutionDefinition;
				HealingExecutionDefinition.CalculationClass = HealingCalculationClass;
				HealingEffect->Executions.Emplace(HealingExecutionDefinition);
				if (HealingExecutionDefinition.CalculationClass)
				{
					FGameplayEffectContext* ContextHandle = UAbilitySystemGlobals::Get().AllocGameplayEffectContext();
					ContextHandle->AddInstigator(Instigator, Instigator);
					const FGameplayEffectSpecHandle HealingEffectHandle = FGameplayEffectSpecHandle(new FGameplayEffectSpec(HealingEffect, FGameplayEffectContextHandle(ContextHandle), 1.0f));

					//Pass in cache value through tag.
					if (HealingEffect->DurationPolicy == EGameplayEffectDurationType::HasDuration && HealingEffect->Period.GetValue() > 0.0f && EffectOverTimeContext.bApplyValueOverTotalDuration)
					{
						float EffectDuration = HealingEffectHandle.Data->GetDuration();
						float EffectPeriod = HealingEffectHandle.Data->GetPeriod();
						InHealing /= (EffectDuration / EffectPeriod);
					}

					AssignTagSetByCallerMagnitude(HealingEffectHandle, Data_IncomingHealing, InHealing);

					InstigatorASC->ApplyGameplayEffectSpecToTarget(*HealingEffectHandle.Data.Get(), TargetASC);
					return true;
				}
			}
		}
	}

	return false;
}

bool UGASCourseASCBlueprintLibrary::ApplyHealingToTargetDataHandle(FGameplayAbilityTargetDataHandle TargetHandle,
                                                                   AActor* Instigator, float InHealing)
{
	TArray<AActor*> Targets = GetAllActorsFromTargetData(TargetHandle);
	bool bHealingApplied = false;

	for (int32 TargetIdx = 0; TargetIdx < Targets.Num(); ++TargetIdx)
	{
		bHealingApplied = ApplyHealingToTarget(Targets[TargetIdx], Instigator, InHealing);
	}

	return bHealingApplied;
}

bool UGASCourseASCBlueprintLibrary::ApplyHealingToTarget(AActor* Target, AActor* Instigator, float InHealing)
{
	//Initialize DoTContext to default values to make damage instant.
	constexpr FEffectOverTimeContext EffectOverTimeContext;
	if(UGameplayEffect* HealingEffect = ConstructHealingGameplayEffect(EGameplayEffectDurationType::Instant, EffectOverTimeContext))
	{
		return ApplyHealingToTarget_Internal(Target, Instigator, InHealing, HealingEffect, EffectOverTimeContext);
	}
	return false;
}

bool UGASCourseASCBlueprintLibrary::ApplyHealingOverTimeToTarget(AActor* Target, AActor* Instigator, float InHealing,
	const FEffectOverTimeContext& EffectOverTimeContext)
{
	UGameplayEffect* HealingEffect = ConstructHealingGameplayEffect(EGameplayEffectDurationType::HasDuration, EffectOverTimeContext);
	return ApplyHealingToTarget_Internal(Target, Instigator, InHealing, HealingEffect, EffectOverTimeContext);
}

bool UGASCourseASCBlueprintLibrary::ApplyDamageOverTimeToTarget(AActor* Target, AActor* Instigator, float Damage, const FDamageContext& DamageContext,
                                                                const FEffectOverTimeContext& EffectOverTimeContext)
{
	UGameplayEffect* DamageEffect = ConstructDamageGameplayEffect(EGameplayEffectDurationType::HasDuration, EffectOverTimeContext);
	return ApplyDamageToTarget_Internal(Target, Instigator, Damage, DamageContext, DamageEffect);
}

bool UGASCourseASCBlueprintLibrary::ApplyPhysicalDamageToTarget(AActor* Target, AActor* Instigator, float Damage,
                                                                const FHitResult& HitResult, FDamageContext& DamageContext)
{
	DamageContext.DamageType = DamageType_Physical;
	DamageContext.HitResult = HitResult;
	constexpr FEffectOverTimeContext EffectOverTimeContext;
	UGameplayEffect* DamageEffect = ConstructDamageGameplayEffect(EGameplayEffectDurationType::Instant, EffectOverTimeContext);
	return ApplyDamageToTarget_Internal(Target, Instigator, Damage, DamageContext, DamageEffect);
}

bool UGASCourseASCBlueprintLibrary::ApplyFireDamageToTarget(AActor* Target, AActor* Instigator, float Damage,
                                                            const FHitResult& HitResult, FDamageContext& DamageContext, bool bApplyBurnStack)
{
	DamageContext.DamageType = DamageType_Elemental_Fire;
	if(bApplyBurnStack)
	{
		FGameplayTagContainer GrantedTags;
		//TODO: Add this to Native Gameplay Tags
		GrantedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Effect.Gameplay.Status.Burn.Stack")));
		DamageContext.GrantedTags = GrantedTags;
	}
	DamageContext.HitResult = HitResult;
	
	constexpr FEffectOverTimeContext EffectOverTimeContext;
	UGameplayEffect* DamageEffect = ConstructDamageGameplayEffect(EGameplayEffectDurationType::Instant, EffectOverTimeContext);
	
	return ApplyDamageToTarget_Internal(Target, Instigator, Damage, DamageContext, DamageEffect);
}

bool UGASCourseASCBlueprintLibrary::ApplyDamageToTarget_Internal(AActor* Target, AActor* Instigator, float Damage,
                                                                 const FDamageContext& DamageContext, UGameplayEffect* GameplayEffect)
{
	if(!Instigator || !Target)
	{
		return false;
	}

	if(AGASCoursePlayerState* InstigatorPlayerState = Cast<AGASCoursePlayerState>(Instigator))
	{
		Instigator = InstigatorPlayerState->GetPawn();
	}

	if(AGASCoursePlayerState* TargetPlayerState = Cast<AGASCoursePlayerState>(Target))
	{
		Target = TargetPlayerState->GetPawn();
	}
	
	TSubclassOf<UGameplayEffectExecutionCalculation> DamageCalculationClass;
	if (const UGASC_AbilitySystemSettings* AbilitySystemSettings = GetDefault<UGASC_AbilitySystemSettings>())
	{
		DamageCalculationClass = AbilitySystemSettings->DamageExecution;
		if (!DamageCalculationClass)
		{
			UE_LOG(LogTemp, Warning, TEXT("Damage Calculation is not valid!"));
			return false;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Ability System Settings is not valid!"));
		return false;
	}
	
	if(AGASCourseCharacter* TargetCharacter = Cast<AGASCourseCharacter>(Target))
	{
		if(AGASCourseCharacter* InstigatorCharacter = Cast<AGASCourseCharacter>(Instigator))
		{
			UGASCourseAbilitySystemComponent* TargetASC = TargetCharacter->GetAbilitySystemComponent();
			check(TargetASC);

			UGASCourseAbilitySystemComponent* InstigatorASC = InstigatorCharacter->GetAbilitySystemComponent();
			check(InstigatorASC);
			
			if(UGASCourseGameplayEffect* DamageEffect = Cast<UGASCourseGameplayEffect>(GameplayEffect))
			{
				FGameplayEffectExecutionDefinition DamageExecutionDefinition;
				DamageExecutionDefinition.CalculationClass = DamageCalculationClass;
				DamageEffect->Executions.Emplace(DamageExecutionDefinition);
				if (DamageExecutionDefinition.CalculationClass)
				{
					FGameplayEffectContext* ContextHandle = UAbilitySystemGlobals::Get().AllocGameplayEffectContext();
					ContextHandle->AddInstigator(Instigator, Instigator);
					const FGameplayEffectSpecHandle DamageEffectHandle = FGameplayEffectSpecHandle(new FGameplayEffectSpec(DamageEffect, FGameplayEffectContextHandle(ContextHandle), 1.0f));
					AssignTagSetByCallerMagnitude(DamageEffectHandle, Data_IncomingDamage, Damage);

					//Pass in cache value through tag.
					if (DamageEffect->DurationPolicy == EGameplayEffectDurationType::HasDuration && DamageEffect->Period.GetValue() > 0.0f)
					{
						AddGrantedTags(DamageEffectHandle, FGameplayTagContainer(Data_DamageOverTime));
					}
				
					if(DamageContext.HitResult.bBlockingHit)
					{
						ContextHandle->AddHitResult(DamageContext.HitResult);
					}

					AddGrantedTags(DamageEffectHandle, DamageContext.GrantedTags);
					AddGrantedTag(DamageEffectHandle, DamageContext.DamageType);

					InstigatorASC->ApplyGameplayEffectSpecToTarget(*DamageEffectHandle.Data.Get(), TargetASC);
				
					return true;
				}
			}
		}
	}

	return false;
}
UGameplayEffect* UGASCourseASCBlueprintLibrary::ConstructHealingGameplayEffect(EGameplayEffectDurationType DurationType,
	const FEffectOverTimeContext& EffectOverTimeContext)
{
	UGASCourseGameplayEffect* HealingEffect = NewObject<UGASCourseGameplayEffect>(GetTransientPackage());
	if(DurationType == EGameplayEffectDurationType::Instant)
	{
		HealingEffect->DurationPolicy = EGameplayEffectDurationType::Instant;
	}
	else
	{
		HealingEffect->DurationPolicy = EGameplayEffectDurationType::HasDuration;
	}

	//EffectOverTimeContext should specify FScalableFloat for duration parameter.
	FScalableFloat Duration;
	Duration.Value = EffectOverTimeContext.EffectDuration;
	HealingEffect->DurationMagnitude = FGameplayEffectModifierMagnitude(Duration);

	//EffectOverTimeContext should specify FScalableFloat for period parameter.
	FScalableFloat Period;
	Period.Value = EffectOverTimeContext.EffectPeriod;
	HealingEffect->Period = Period;
	HealingEffect->bExecutePeriodicEffectOnApplication = EffectOverTimeContext.bApplyEffectOnApplication;
	
	return HealingEffect;
}

UGameplayEffect* UGASCourseASCBlueprintLibrary::ConstructDamageGameplayEffect(EGameplayEffectDurationType DurationType,  const FEffectOverTimeContext& EffectOverTimeContext)
{
	UGASCourseGameplayEffect* DamageEffect = NewObject<UGASCourseGameplayEffect>(GetTransientPackage());
	if(DurationType == EGameplayEffectDurationType::Instant)
	{
		DamageEffect->DurationPolicy = EGameplayEffectDurationType::Instant;
	}
	else
	{
		DamageEffect->DurationPolicy = EGameplayEffectDurationType::HasDuration;
	}

	//DamageOverTimeContext should specify FScalableFloat for duration parameter.
	FScalableFloat Duration;
	Duration.Value = EffectOverTimeContext.EffectDuration;
	DamageEffect->DurationMagnitude = FGameplayEffectModifierMagnitude(Duration);

	//DamageOverTimeContext should specify FScalableFloat for period parameter.
	FScalableFloat Period;
	Period.Value = EffectOverTimeContext.EffectPeriod;
	DamageEffect->Period = Period;
	DamageEffect->bExecutePeriodicEffectOnApplication = EffectOverTimeContext.bApplyEffectOnApplication;
	
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

EGASCourseAbilitySlotType UGASCourseASCBlueprintLibrary::GetGameplayAbilitySlotTypeFromHandle(
	const UAbilitySystemComponent* AbilitySystem, const FGameplayAbilitySpecHandle& AbilitySpecHandle)
{
	EGASCourseAbilitySlotType AbilitySlot = EGASCourseAbilitySlotType::EmptySlot;
	// validate the ASC
	if (!AbilitySystem)
	{
		return AbilitySlot;
	}

	// get and validate the ability spec
	const FGameplayAbilitySpec* AbilitySpec = AbilitySystem->FindAbilitySpecFromHandle(AbilitySpecHandle);
	if (!AbilitySpec)
	{
		return AbilitySlot;
	}

	// try to get the ability instance
	if(const UGASCourseGameplayAbility* AbilityInstance = Cast<UGASCourseGameplayAbility>(AbilitySpec->GetPrimaryInstance()))
	{
		AbilitySlot = AbilityInstance->GetAbilitySlotType();
	}

	return AbilitySlot;
}

void UGASCourseASCBlueprintLibrary::GetAllAbilitiesofAbilitySlotType(const UAbilitySystemComponent* AbilitySystem,  EGASCourseAbilitySlotType AbilitySlot, 
	TArray<FGameplayAbilitySpecHandle>& OutAbilityHandles)
{
	if(AbilitySystem)
	{
		OutAbilityHandles.Empty(AbilitySystem->GetActivatableAbilities().Num());
		for (const FGameplayAbilitySpec& Spec : AbilitySystem->GetActivatableAbilities())
		{
			if(GetGameplayAbilitySlotTypeFromHandle(AbilitySystem, Spec.Handle) == AbilitySlot)
			{
				// add the spec handle to the list
				OutAbilityHandles.Add(Spec.Handle);
			}
		}
	}
}

void UGASCourseASCBlueprintLibrary::SendGameplayEventToTargetDataHandle(FGameplayAbilityTargetDataHandle TargetHandle,
	FGameplayTag EventTag, FGameplayEventData Payload)
{
	TArray<AActor*> Targets = GetAllActorsFromTargetData(TargetHandle);
	for(AActor* Target : Targets)
	{
		SendGameplayEventToActor(Target, EventTag, Payload);
	}
}

FGameplayAttribute UGASCourseASCBlueprintLibrary::GetGameplayAttributeFromModifierStruct(
	const FGameplayModifierInfo& ModifierInfo)
{
	FGameplayAttribute Attribute;

	if(ModifierInfo.Attribute.IsValid())
	{
		Attribute = ModifierInfo.Attribute;
	}

	return Attribute;
}

float UGASCourseASCBlueprintLibrary::GetModifierMagnitudeAtIndex(FActiveGameplayEffectHandle InGameplayEffect, int32 ModifierIdx,
	bool bFactorInStackCount)
{
	float OutModifierMagnitude = 0.0f;
	
	const FGameplayEffectSpec& Spec = GetSpecHandleFromGameplayEffect(InGameplayEffect);
	if(Spec.Def)
	{
		Spec.Def->Modifiers[ModifierIdx].ModifierMagnitude.AttemptCalculateMagnitude(Spec, OutModifierMagnitude);
	}
	
	return OutModifierMagnitude;
}

FGameplayEffectSpec UGASCourseASCBlueprintLibrary::GetSpecHandleFromGameplayEffect(FActiveGameplayEffectHandle InGameplayEffect)
{
	FGameplayEffectSpec OutSpec;
	if(const UAbilitySystemComponent* AbilitySystemComponent = InGameplayEffect.GetOwningAbilitySystemComponent())
	{
		if(const FActiveGameplayEffect* ActiveGameplayEffect = AbilitySystemComponent->GetActiveGameplayEffect(InGameplayEffect))
		{
			OutSpec = ActiveGameplayEffect->Spec;
		}
	}

	return OutSpec;
}

float UGASCourseASCBlueprintLibrary::GetPeriodFromGameplayEffect(FActiveGameplayEffectHandle InGameplayEffect)
{
	float OutPeriod = 0.0f;

	const FGameplayEffectSpec& Spec = GetSpecHandleFromGameplayEffect(InGameplayEffect);
	if(Spec.Def)
	{
		OutPeriod = Spec.GetPeriod();
	}
	
	return OutPeriod;
}

bool UGASCourseASCBlueprintLibrary::GrantAbilityToInputTag(UAbilitySystemComponent* InASC, TSubclassOf<UGASCourseGameplayAbility> Ability, int AbilityLevel,
                                                           FGameplayTag InputTag)
{
	if (!Ability)
	{
		return false;
	}

	FGameplayAbilitySpec AbilitySpec(Ability, AbilityLevel);
	AbilitySpec.SourceObject = InASC;
	AbilitySpec.GetDynamicSpecSourceTags().AddTag(InputTag);

	const FGameplayAbilitySpecHandle AbilitySpecHandle = InASC->GiveAbility(AbilitySpec);

	return true;
}

