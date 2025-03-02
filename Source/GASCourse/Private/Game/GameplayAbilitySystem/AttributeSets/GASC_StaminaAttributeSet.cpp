// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GameplayAbilitySystem/AttributeSets/GASC_StaminaAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

UGASC_StaminaAttributeSet::UGASC_StaminaAttributeSet()
{
}

void UGASC_StaminaAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if(Attribute == GetMaxStaminaAttribute())
	{
		AdjustAttributeForMaxChange(CurrentStamina, MaxStamina, NewValue, GetCurrentStaminaAttribute());
	}

	if(Attribute == GetCurrentStaminaAttribute())
	{
		NewValue = FMath::Clamp<float>(NewValue, 0.0f, MaxStamina.GetCurrentValue());
	}
	
	if(Attribute == GetStaminaRegenerationRateAttribute())
	{
		NewValue = FMath::Clamp<float>(NewValue, 0.0f, 100.0f);
	}
	
	if(Attribute == GetStaminaRegenerationDelayAttribute())
	{
		NewValue = FMath::Clamp<float>(NewValue, 0.1f, 5.0f);
	}
}

void UGASC_StaminaAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue,
	float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
}

void UGASC_StaminaAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	if(Attribute == GetCurrentStaminaAttribute())
	{
		NewValue = FMath::Clamp<float>(NewValue, 0.0f, MaxStamina.GetCurrentValue());
		if (NewValue < CurrentStamina.GetCurrentValue())
		{
			if (AbilitySystemSettings)
			{
				if (const UGameplayEffect* PassiveRegenBlockerEffect = AbilitySystemSettings->PassiveResourceBlocker.GetDefaultObject())
				{
					FGameplayEffectContextHandle Context = FGameplayEffectContextHandle();
					Context.AddInstigator(GetOwningActor(), GetOwningActor());
					Context.AddSourceObject(this);
					GetOwningAbilitySystemComponentChecked()->ApplyGameplayEffectToSelf(PassiveRegenBlockerEffect, 1.0f, Context);
				}
			}
		}
	}
	
	if(Attribute == GetStaminaRegenerationRateAttribute())
	{
		NewValue = FMath::Clamp<float>(NewValue, 0.0f, 100.0f);
	}
	
	if(Attribute == GetStaminaRegenerationDelayAttribute())
	{
		NewValue = FMath::Clamp<float>(NewValue, 0.1f, 5.0f);
	}
}

void UGASC_StaminaAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
}

void UGASC_StaminaAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASC_StaminaAttributeSet, CurrentStamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASC_StaminaAttributeSet, MaxStamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASC_StaminaAttributeSet, StaminaRegenerationRate, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASC_StaminaAttributeSet, StaminaRegenerationDelay, COND_None, REPNOTIFY_Always);
}

void UGASC_StaminaAttributeSet::OnRep_CurrentStamina(const FGameplayAttributeData& OldCurrentStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGASC_StaminaAttributeSet, CurrentStamina, OldCurrentStamina);
}

void UGASC_StaminaAttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGASC_StaminaAttributeSet, MaxStamina, OldMaxStamina);
}

void UGASC_StaminaAttributeSet::OnRep_StaminaRegenerationRate(
	const FGameplayAttributeData& OldStaminaRegenerationRate)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGASC_StaminaAttributeSet, StaminaRegenerationRate, OldStaminaRegenerationRate);
}

void UGASC_StaminaAttributeSet::OnRep_StaminaRegenerationDelay(
	const FGameplayAttributeData& OldStaminaRegenerationDelay)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGASC_StaminaAttributeSet, StaminaRegenerationDelay, OldStaminaRegenerationDelay);
}
