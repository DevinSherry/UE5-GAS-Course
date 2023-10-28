// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GASCourseAttributeSet.generated.h"

/**
 * 
 */

// Uses macros from AttributeSet.h
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class GASCOURSE_API UGASCourseAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:

	UGASCourseAttributeSet();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	
	UPROPERTY(BlueprintReadOnly, Category = "Character Base Attributes", ReplicatedUsing=OnRep_OneAttribute)
	FGameplayAttributeData OneAttribute;
	ATTRIBUTE_ACCESSORS(UGASCourseAttributeSet, OneAttribute)

	UPROPERTY(BlueprintReadOnly, Category = "Character Base Attributes", ReplicatedUsing=OnRep_TwoAttribute)
	FGameplayAttributeData TwoAttribute;
	ATTRIBUTE_ACCESSORS(UGASCourseAttributeSet, TwoAttribute)

protected:

	// Helper function to proportionally adjust the value of an attribute when it's associated max attribute changes.
	// (i.e. When MaxHealth increases, Health increases by an amount that maintains the same percentage as before)
	void AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute,
		float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty);

	UFUNCTION()
	virtual void OnRep_OneAttribute(const FGameplayAttributeData& OldOneAttribute);
	
	UFUNCTION()
	virtual void OnRep_TwoAttribute(const FGameplayAttributeData& OldTwoAttribute);
	
};
