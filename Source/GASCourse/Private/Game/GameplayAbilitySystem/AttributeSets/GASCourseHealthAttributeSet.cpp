// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GameplayAbilitySystem/AttributeSets/GASCourseHealthAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

UGASCourseHealthAttributeSet::UGASCourseHealthAttributeSet()
{
}

void UGASCourseHealthAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if(Attribute == GetMaxHealthAttribute())
	{
		AdjustAttributeForMaxChange(CurrentHealth, MaxHealth, NewValue, GetCurrentHealthAttribute());
	}

	if(Attribute == GetCurrentHealthAttribute())
	{
		NewValue = FMath::Clamp<float>(NewValue, 0.0f, MaxHealth.GetCurrentValue());
	}
}

void UGASCourseHealthAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue,
	float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
}

void UGASCourseHealthAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if(Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		const float LocalDamage = GetIncomingDamage();
		SetIncomingDamage(0.0f);
		SetCurrentHealth(GetCurrentHealth() - LocalDamage);

		if(GetCurrentHealth() <= 0.0f)
		{
			FGameplayEventData OnDeathPayload;
			OnDeathPayload.Instigator = Data.EffectSpec.GetContext().GetOriginalInstigator();
			OnDeathPayload.Target = GetOwningActor();
			OnDeathPayload.ContextHandle = Data.EffectSpec.GetContext();
			OnDeathPayload.EventMagnitude = LocalDamage;
			GetOwningAbilitySystemComponent()->HandleGameplayEvent(FGameplayTag::RequestGameplayTag(FName("Event.Gameplay.OnDeath")), &OnDeathPayload);
		}
	}
}

void UGASCourseHealthAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASCourseHealthAttributeSet, CurrentHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASCourseHealthAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
}

void UGASCourseHealthAttributeSet::OnRep_CurrentHealth(const FGameplayAttributeData& OldCurrentHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGASCourseHealthAttributeSet, CurrentHealth, OldCurrentHealth);
}

void UGASCourseHealthAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGASCourseHealthAttributeSet, MaxHealth, OldMaxHealth);
}
