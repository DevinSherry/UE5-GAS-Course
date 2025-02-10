// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GameplayAbilitySystem/AttributeSets/GASC_ResourceAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

UGASC_ResourceAttributeSet::UGASC_ResourceAttributeSet()
{
}

void UGASC_ResourceAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if(Attribute == GetMaxResourceAttribute())
	{
		AdjustAttributeForMaxChange(CurrentResource, MaxResource, NewValue, GetCurrentResourceAttribute());
	}

	if(Attribute == GetCurrentResourceAttribute())
	{
		NewValue = FMath::Clamp<float>(NewValue, 0.0f, MaxResource.GetCurrentValue());
	}
	
	if(Attribute == GetResourceRegenerationRateAttribute())
	{
		NewValue = FMath::Clamp<float>(NewValue, 0.0f, 100.0f);
	}
	
	if(Attribute == GetResourceRegenerationDelayAttribute())
	{
		NewValue = FMath::Clamp<float>(NewValue, 0.1f, 5.0f);
	}
}

void UGASC_ResourceAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue,
	float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
}

void UGASC_ResourceAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	if(Attribute == GetCurrentResourceAttribute())
	{
		NewValue = FMath::Clamp<float>(NewValue, 0.0f, MaxResource.GetCurrentValue());
		if (NewValue < CurrentResource.GetCurrentValue())
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
	
	if(Attribute == GetResourceRegenerationRateAttribute())
	{
		NewValue = FMath::Clamp<float>(NewValue, 0.0f, 100.0f);
	}
	
	if(Attribute == GetResourceRegenerationDelayAttribute())
	{
		NewValue = FMath::Clamp<float>(NewValue, 0.1f, 5.0f);
	}
}

void UGASC_ResourceAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
}

void UGASC_ResourceAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASC_ResourceAttributeSet, CurrentResource, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASC_ResourceAttributeSet, MaxResource, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASC_ResourceAttributeSet, ResourceRegenerationRate, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASC_ResourceAttributeSet, ResourceRegenerationDelay, COND_None, REPNOTIFY_Always);
}

void UGASC_ResourceAttributeSet::OnRep_CurrentResource(const FGameplayAttributeData& OldCurrentResource)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGASC_ResourceAttributeSet, CurrentResource, OldCurrentResource);
}

void UGASC_ResourceAttributeSet::OnRep_MaxResource(const FGameplayAttributeData& OldMaxResource)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGASC_ResourceAttributeSet, MaxResource, OldMaxResource);
}

void UGASC_ResourceAttributeSet::OnRep_ResourceRegenerationRate(
	const FGameplayAttributeData& OldResourceRegenerationRate)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGASC_ResourceAttributeSet, ResourceRegenerationRate, OldResourceRegenerationRate);
}

void UGASC_ResourceAttributeSet::OnRep_ResourceRegenerationDelay(
	const FGameplayAttributeData& OldResourceRegenerationDelay)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGASC_ResourceAttributeSet, ResourceRegenerationDelay, OldResourceRegenerationDelay);
}
