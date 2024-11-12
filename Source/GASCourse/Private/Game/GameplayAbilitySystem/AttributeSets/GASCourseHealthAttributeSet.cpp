// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GameplayAbilitySystem/AttributeSets/GASCourseHealthAttributeSet.h"
#include "Game/GameplayAbilitySystem/GASCourseNativeGameplayTags.h"
#include "GameplayEffectExtension.h"
#include "Game/GameplayAbilitySystem/GASCourseGameplayEffect.h"
#include "GASCourse/GASCourseCharacter.h"
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

	if(Attribute == GetStatusDamageHealingCoefficientAttribute())
	{
		NewValue = FMath::Clamp<float>(NewValue, 0.0f, 1.0f);
	}

	if(Attribute == GetAllDamageHealingCoefficientAttribute())
	{
		NewValue = FMath::Clamp<float>(NewValue, 0.0f, 1.0f);
	}
	
	if(Attribute == GetElementalDamageHealingCoefficientAttribute())
	{
		NewValue = FMath::Clamp<float>(NewValue, 0.0f, 1.0f);
	}

	if(Attribute == GetPhysicalDamageHealingCoefficientAttribute())
	{
		NewValue = FMath::Clamp<float>(NewValue, 0.0f, 1.0f);
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

	// Get the Target actor, which should be our owner
	AActor* TargetActor = nullptr;
	AController* TargetController = nullptr;
	AGASCourseCharacter* TargetCharacter = nullptr;
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		TargetCharacter = Cast<AGASCourseCharacter>(TargetActor);
	}

	if(Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		const float LocalDamage = GetIncomingDamage();
		SetIncomingDamage(0.0f);

		bool bIsAlive = TargetCharacter->IsCharacterAlive();
		
		const float HealthBeforeDamage = CurrentHealth.GetCurrentValue();
		const float NewHealth = CurrentHealth.GetCurrentValue() - LocalDamage;
		
		SetCurrentHealth(FMath::Clamp(NewHealth, 0.0f, GetMaxHealth()));
	
		if(NewHealth <= 0.0f && bIsAlive)
		{
			FGameplayEventData OnDeathPayload;
			OnDeathPayload.EventTag = Event_OnDeath;
			OnDeathPayload.Instigator = Data.EffectSpec.GetContext().GetOriginalInstigator();
			OnDeathPayload.Target = GetOwningActor();
			OnDeathPayload.ContextHandle = Data.EffectSpec.GetContext();
			OnDeathPayload.EventMagnitude = LocalDamage;
			GetOwningAbilitySystemComponent()->HandleGameplayEvent(Event_OnDeath, &OnDeathPayload);
		}
	}
	//Passive Healing Event
	if(Data.EvaluatedData.Attribute == GetIncomingHealingAttribute() && CurrentHealth.GetCurrentValue() != MaxHealth.GetCurrentValue())
	{
		const float LocalIncomingHealing = GetIncomingHealing();
		SetIncomingHealing(0.0f);
		float NewCurrentHealth = GetCurrentHealth() + LocalIncomingHealing;
		SetCurrentHealth(NewCurrentHealth);

		FGameplayEventData OnHealingPayload;
		OnHealingPayload.EventTag = Event_Gameplay_OnHealing;
		OnHealingPayload.Instigator = Data.EffectSpec.GetContext().GetOriginalInstigator();
		OnHealingPayload.Target = GetOwningActor();
		OnHealingPayload.ContextHandle = Data.EffectSpec.GetContext();
		OnHealingPayload.EventMagnitude = LocalIncomingHealing;
		GetOwningAbilitySystemComponent()->HandleGameplayEvent(Event_Gameplay_OnHealing, &OnHealingPayload);
	}
	else if (Data.EvaluatedData.Attribute == GetCurrentHealthAttribute())
	{
		SetCurrentHealth(FMath::Clamp(GetCurrentHealth(), 0.0f, GetMaxHealth()));
	}
}

void UGASCourseHealthAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASCourseHealthAttributeSet, CurrentHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASCourseHealthAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASCourseHealthAttributeSet, StatusDamageHealingCoefficient, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASCourseHealthAttributeSet, ElementalDamageHealingCoefficient, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASCourseHealthAttributeSet, PhysicalDamageHealingCoefficient, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASCourseHealthAttributeSet, AllDamageHealingCoefficient, COND_None, REPNOTIFY_Always);
}

void UGASCourseHealthAttributeSet::OnRep_CurrentHealth(const FGameplayAttributeData& OldCurrentHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGASCourseHealthAttributeSet, CurrentHealth, OldCurrentHealth);
}

void UGASCourseHealthAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGASCourseHealthAttributeSet, MaxHealth, OldMaxHealth);
}

void UGASCourseHealthAttributeSet::OnRep_StatusDamageHealingCoefficient(const FGameplayAttributeData& OldDamageOverTimeHealingCoefficient)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGASCourseHealthAttributeSet, StatusDamageHealingCoefficient, OldDamageOverTimeHealingCoefficient);
}

void UGASCourseHealthAttributeSet::OnRep_ElementalDamageHealingCoefficient(
	const FGameplayAttributeData& OldElementalDamageHealingCoefficient)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGASCourseHealthAttributeSet, ElementalDamageHealingCoefficient, OldElementalDamageHealingCoefficient);
}

void UGASCourseHealthAttributeSet::OnRep_PhysicalDamageHealingCoefficient(
	const FGameplayAttributeData& OldPhysicalDamageHealingCoefficient)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGASCourseHealthAttributeSet, PhysicalDamageHealingCoefficient, OldPhysicalDamageHealingCoefficient);
}

void UGASCourseHealthAttributeSet::OnRep_AllDamageHealingCoefficient(
	const FGameplayAttributeData& OldAllDamageHealingCoefficient)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGASCourseHealthAttributeSet, AllDamageHealingCoefficient, OldAllDamageHealingCoefficient);
}
