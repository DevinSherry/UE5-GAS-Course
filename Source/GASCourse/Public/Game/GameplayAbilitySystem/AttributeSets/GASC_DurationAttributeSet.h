// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Game/GameplayAbilitySystem/AttributeSets/GASCourseAttributeSet.h"
#include "GASC_DurationAttributeSet.generated.h"


// Uses macros from AttributeSet.h
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * UGASC_DurationAttributeSet is a class that extends UGASCourseAttributeSet. This class is responsible for
 * managing gameplay attributes related to durations, such as the ability duration multiplier,
 * the ability cooldown multiplier, and the status effect duration multiplier.
 *
 * It overrides base class functions to handle the lifecycle and replication of gameplay attributes.
 */
UCLASS()
class GASCOURSE_API UGASC_DurationAttributeSet : public UGASCourseAttributeSet
{
	GENERATED_BODY()

public:

	UGASC_DurationAttributeSet();

	// AttributeSet Overrides
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
public:
	
	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Durations", ReplicatedUsing=OnRep_AbilityDurationMultiplier)
	FGameplayAttributeData AbilityDurationMultiplier = 1.0f;
	ATTRIBUTE_ACCESSORS(UGASC_DurationAttributeSet, AbilityDurationMultiplier)
	
	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Durations", ReplicatedUsing=OnRep_AbilityCooldownMultiplier)
	FGameplayAttributeData AbilityCooldownMultiplier = 1.0f;
	ATTRIBUTE_ACCESSORS(UGASC_DurationAttributeSet, AbilityCooldownMultiplier)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Durations", ReplicatedUsing=OnRep_StatusEffectDurationMultiplier)
	FGameplayAttributeData StatusEffectDurationMultiplier = 1.0f;
	ATTRIBUTE_ACCESSORS(UGASC_DurationAttributeSet, StatusEffectDurationMultiplier)
	
	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Durations", ReplicatedUsing=OnRep_StatusEffectDurationReductionMultiplier)
	FGameplayAttributeData StatusEffectDurationReductionMultiplier = 0.0f;
	ATTRIBUTE_ACCESSORS(UGASC_DurationAttributeSet, StatusEffectDurationReductionMultiplier)
	


protected:
	
	UFUNCTION()
	virtual void OnRep_AbilityDurationMultiplier(const FGameplayAttributeData& OldAbilityDurationMultiplier);

	UFUNCTION()
	virtual void OnRep_AbilityCooldownMultiplier(const FGameplayAttributeData& OldAbilityCooldownMultiplier);

	UFUNCTION()
	virtual void OnRep_StatusEffectDurationMultiplier(const FGameplayAttributeData& OldStatusEffectDurationMultiplier);

	UFUNCTION()
	virtual void OnRep_StatusEffectDurationReductionMultiplier(const FGameplayAttributeData& OldStatusEffectDurationReductionMultiplier);
};
