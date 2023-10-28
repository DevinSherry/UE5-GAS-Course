// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GameplayAbilitySystem/AttributeSets/GASCourseAttributeSet.h"
#include "Net/UnrealNetwork.h"

UGASCourseAttributeSet::UGASCourseAttributeSet()
{
}

void UGASCourseAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UGASCourseAttributeSet, OneAttribute, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASCourseAttributeSet, TwoAttribute, COND_None, REPNOTIFY_Always);
}

void UGASCourseAttributeSet::AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute,
	const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty)
{
	UAbilitySystemComponent* AbilityComp = GetOwningAbilitySystemComponent();
	const float CurrentMaxValue = MaxAttribute.GetCurrentValue();
	if (!FMath::IsNearlyEqual(CurrentMaxValue, NewMaxValue) && AbilityComp)
	{
		// Change current value to maintain the current Val / Max percent
		const float CurrentValue = AffectedAttribute.GetCurrentValue();
		float NewDelta = (CurrentMaxValue > 0.f) ? (CurrentValue * NewMaxValue / CurrentMaxValue) - CurrentValue : NewMaxValue;

		AbilityComp->ApplyModToAttributeUnsafe(AffectedAttributeProperty, EGameplayModOp::Additive, NewDelta);
	}
}

void UGASCourseAttributeSet::OnRep_OneAttribute(const FGameplayAttributeData& OldOneAttribute)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGASCourseAttributeSet, OneAttribute, OldOneAttribute);
}

void UGASCourseAttributeSet::OnRep_TwoAttribute(const FGameplayAttributeData& OldTwoAttribute)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGASCourseAttributeSet, TwoAttribute, OldTwoAttribute);
}
