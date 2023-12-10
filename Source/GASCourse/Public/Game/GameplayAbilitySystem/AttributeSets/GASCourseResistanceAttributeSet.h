// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Game/GameplayAbilitySystem/AttributeSets/GASCourseAttributeSet.h"
#include "GASCourseResistanceAttributeSet.generated.h"

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
class GASCOURSE_API UGASCourseResistanceAttributeSet : public UGASCourseAttributeSet
{
	GENERATED_BODY()

public:

	UGASCourseResistanceAttributeSet();

	// AttributeSet Overrides
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	
	UPROPERTY(BlueprintReadOnly, Category = "Character Resistance Attributes", ReplicatedUsing=OnRep_Resistance)
	FGameplayAttributeData Resistance;
	ATTRIBUTE_ACCESSORS(UGASCourseResistanceAttributeSet, Resistance)

	UPROPERTY(BlueprintReadOnly, Category = "Character Resistance Attributes", ReplicatedUsing=OnRep_MaxResistance)
	FGameplayAttributeData MaxResistance;
	ATTRIBUTE_ACCESSORS(UGASCourseResistanceAttributeSet, MaxResistance)
	
protected:
	
	UFUNCTION()
	virtual void OnRep_Resistance(const FGameplayAttributeData& OldResistance);
	
	UFUNCTION()
	virtual void OnRep_MaxResistance(const FGameplayAttributeData& OldMaxResistance);
	
};
