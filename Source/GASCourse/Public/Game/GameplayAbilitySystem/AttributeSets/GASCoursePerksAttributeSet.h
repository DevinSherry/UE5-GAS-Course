// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/GameplayAbilitySystem/AttributeSets/GASCourseAttributeSet.h"
#include "GASCoursePerksAttributeSet.generated.h"


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
class GASCOURSE_API UGASCoursePerksAttributeSet : public UGASCourseAttributeSet
{
	GENERATED_BODY()
	
public:

	UGASCoursePerksAttributeSet();
	
	// AttributeSet Overrides
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(BlueprintReadOnly, Category = "Character Perks Attributes", ReplicatedUsing=OnRep_NumofRicochetBounces)
	FGameplayAttributeData NumofRicochetBounces;
	ATTRIBUTE_ACCESSORS(UGASCoursePerksAttributeSet, NumofRicochetBounces)

	UPROPERTY(BlueprintReadOnly, Category = "Character Perks Attributes", ReplicatedUsing=OnRep_DodgeDistanceMultiplier)
	FGameplayAttributeData DodgeDistanceMultiplier = 1.0f;
	ATTRIBUTE_ACCESSORS(UGASCoursePerksAttributeSet, DodgeDistanceMultiplier)

protected:
	
	UFUNCTION()
	virtual void OnRep_NumofRicochetBounces(const FGameplayAttributeData& OldNumofRicochetBounces);

	UFUNCTION()
	virtual void OnRep_DodgeDistanceMultiplier(const FGameplayAttributeData& OldDodgeDistanceMultiplier);
	
};
