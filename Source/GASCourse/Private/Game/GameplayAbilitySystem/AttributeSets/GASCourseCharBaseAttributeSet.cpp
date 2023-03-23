// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GameplayAbilitySystem/AttributeSets/GASCourseCharBaseAttributeSet.h"

#include "Game/Character/Player/GASCoursePlayerState.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GASCourse/GASCourseCharacter.h"
#include "Net/UnrealNetwork.h"

UGASCourseCharBaseAttributeSet::UGASCourseCharBaseAttributeSet()
{
}

void UGASCourseCharBaseAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	//TODO: Clamp movement speed to low/high values to prevent crazy values from getting in.
}

void UGASCourseCharBaseAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue,
	float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	const AGASCoursePlayerState* OwnerPS = Cast<AGASCoursePlayerState>(GetOwningActor());
	if(!OwnerPS)
	{
		return;
	}
	const AGASCourseCharacter* OwnerCharacter = Cast<AGASCourseCharacter>(OwnerPS->GetPawn());
	if(!OwnerCharacter)
	{
		return;
	}
	if(Attribute == GetMovementSpeedAttribute())
	{
		OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = NewValue;
	}
}

void UGASCourseCharBaseAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
}

void UGASCourseCharBaseAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UGASCourseCharBaseAttributeSet, MovementSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASCourseCharBaseAttributeSet, CrouchSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGASCourseCharBaseAttributeSet, JumpZVelocityOverride, COND_None, REPNOTIFY_Always);
}

void UGASCourseCharBaseAttributeSet::AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute,
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

void UGASCourseCharBaseAttributeSet::OnRep_MovementSpeed(const FGameplayAttributeData& OldMovementSpeed)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGASCourseCharBaseAttributeSet, MovementSpeed, OldMovementSpeed);
}

void UGASCourseCharBaseAttributeSet::OnRep_CrouchSpeed(const FGameplayAttributeData& OldCrouchSpeed)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGASCourseCharBaseAttributeSet, CrouchSpeed, OldCrouchSpeed)
}

void UGASCourseCharBaseAttributeSet::OnRep_JumpZVelocityOverride(const FGameplayAttributeData& OldJumpZVelocityOverride)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGASCourseCharBaseAttributeSet, JumpZVelocityOverride, OldJumpZVelocityOverride);
}
