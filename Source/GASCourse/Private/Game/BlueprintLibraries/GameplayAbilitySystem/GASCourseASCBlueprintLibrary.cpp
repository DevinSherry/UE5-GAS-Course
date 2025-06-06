// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/BlueprintLibraries/GameplayAbilitySystem/GASCourseASCBlueprintLibrary.h"
#include "AbilitySystemLog.h"
#include "AbilitySystemGlobals.h"
#include "Game/GameplayAbilitySystem/GASCourseAbilitySystemComponent.h"
#include "Game/Systems/Damage/GASCourseDamageExecution.h"
#include "Game/GameplayAbilitySystem/GASCourseNativeGameplayTags.h"
#include "Game/GameplayAbilitySystem/GASCourseGameplayEffect.h"

bool UGASCourseASCBlueprintLibrary::ApplyDamageToTarget(AActor* Target, AActor* Instigator, float Damage, const FDamageContext& DamageContext)
{
	//Initialize DoTContext to default values to make damage instant.
	constexpr FDamageOverTimeContext DamageOverTimeContext;
	if(UGameplayEffect* DamageEffect = ConstructDamageGameplayEffect(EGameplayEffectDurationType::Instant, DamageOverTimeContext, Damage))
	{
		return ApplyDamageToTarget_Internal(Target, Instigator, Damage, DamageContext, DamageEffect);
	}
	return false;
}

bool UGASCourseASCBlueprintLibrary::ApplyDamageToTargetDataHandle(FGameplayAbilityTargetDataHandle TargetHandle,
	AActor* Instigator, float Damage, const FDamageContext& DamageContext)
{
	TArray<AActor*> Targets = GetAllActorsFromTargetData(TargetHandle);
	bool bDamageApplied = false;
	
	for(AActor* Target: Targets)
	{
		bDamageApplied = ApplyDamageToTarget(Target, Instigator, Damage, DamageContext);
	}
	return bDamageApplied;
}

bool UGASCourseASCBlueprintLibrary::ApplyDamageOverTimeToTarget(AActor* Target, AActor* Instigator, float Damage,
                                                                const FDamageContext& DamageContext, const FDamageOverTimeContext& DamageOverTimeContext)
{
	UGameplayEffect* DamageEffect = ConstructDamageGameplayEffect(EGameplayEffectDurationType::HasDuration, DamageOverTimeContext, Damage);
	return ApplyDamageToTarget_Internal(Target, Instigator, Damage, DamageContext, DamageEffect);
}

bool UGASCourseASCBlueprintLibrary::ApplyPhysicalDamageToTarget(AActor* Target, AActor* Instigator, float Damage,
                                                                const FHitResult& HitResult, FDamageContext& DamageContext)
{
	DamageContext.DamageType = DamageType_Physical;
	DamageContext.HitResult = HitResult;
	constexpr FDamageOverTimeContext DamageOverTimeContext;
	UGameplayEffect* DamageEffect = ConstructDamageGameplayEffect(EGameplayEffectDurationType::Instant, DamageOverTimeContext, Damage);
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
	
	constexpr FDamageOverTimeContext DamageOverTimeContext;
	UGameplayEffect* DamageEffect = ConstructDamageGameplayEffect(EGameplayEffectDurationType::Instant, DamageOverTimeContext, Damage);
	
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

	//TODO: Add check to verify ability system component + consider damage/health interface for Non-GAS actors
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
				DamageExecutionDefinition.CalculationClass = LoadClass<UGASCourseDamageExecution>(Instigator, TEXT("/Game/GASCourse/Game/Systems/Damage/DamageExecution_Base.DamageExecution_Base_C"));
				DamageEffect->Executions.Emplace(DamageExecutionDefinition);
				if (DamageExecutionDefinition.CalculationClass)
				{
					UE_LOG(LogTemp, Warning, TEXT("Damage Calculation: %s"), *DamageExecutionDefinition.CalculationClass->GetName());
				}
				UE_LOG(LogTemp, Warning, TEXT("Damage Calculation is not valid!"));
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

	return false;
}

UGameplayEffect* UGASCourseASCBlueprintLibrary::ConstructDamageGameplayEffect(EGameplayEffectDurationType DurationType,  const FDamageOverTimeContext& DamageOverTimeContext, float InDamageValue)
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
	Duration.Value = DamageOverTimeContext.DamageDuration;
	DamageEffect->DurationMagnitude = FGameplayEffectModifierMagnitude(Duration);

	//DamageOverTimeContext should specify FScalableFloat for period parameter.
	FScalableFloat Period;
	Period.Value = DamageOverTimeContext.DamagePeriod;
	DamageEffect->Period = Period;
	DamageEffect->bExecutePeriodicEffectOnApplication = DamageOverTimeContext.bApplyDamageOnApplication;
	
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

