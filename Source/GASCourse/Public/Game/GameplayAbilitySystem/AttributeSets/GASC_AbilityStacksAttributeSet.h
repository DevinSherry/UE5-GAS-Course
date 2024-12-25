// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Game/GameplayAbilitySystem/AttributeSets/GASCourseAttributeSet.h"
#include "GASC_AbilityStacksAttributeSet.generated.h"

// Uses macros from AttributeSet.h
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)


/**
 * 
 */
UCLASS()
class GASCOURSE_API UGASC_AbilityStacksAttributeSet : public UGASCourseAttributeSet
{
	GENERATED_BODY()
	
public:

	UGASC_AbilityStacksAttributeSet();

	// AttributeSet Overrides
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	
	UPROPERTY(BlueprintReadOnly, Category = "Character Stacking Attributes", ReplicatedUsing=OnRep_CurrentDodgeStackCount)
	FGameplayAttributeData CurrentDodgeStackCount;
	ATTRIBUTE_ACCESSORS(UGASC_AbilityStacksAttributeSet, CurrentDodgeStackCount)

	UPROPERTY(BlueprintReadOnly, Category = "Character Stacking Attributes", ReplicatedUsing=OnRep_MaxDodgeStackCount)
	FGameplayAttributeData MaxDodgeStackCount;
	ATTRIBUTE_ACCESSORS(UGASC_AbilityStacksAttributeSet, MaxDodgeStackCount)

	UPROPERTY(BlueprintReadOnly, Category = "Character Stacking Attributes", ReplicatedUsing=OnRep_StackDurationModifier)
	FGameplayAttributeData StackDurationModifier = 1.0f;
	ATTRIBUTE_ACCESSORS(UGASC_AbilityStacksAttributeSet, StackDurationModifier)

protected:
	
	UFUNCTION()
	virtual void OnRep_CurrentDodgeStackCount(const FGameplayAttributeData& OldCurrentDodgeStackCount);
	
	UFUNCTION()
	virtual void OnRep_MaxDodgeStackCount(const FGameplayAttributeData& OldMaxDodgeStackCount);

	UFUNCTION()
	virtual void OnRep_StackDurationModifier(const FGameplayAttributeData& OldStackDurationModifier);
	
};
