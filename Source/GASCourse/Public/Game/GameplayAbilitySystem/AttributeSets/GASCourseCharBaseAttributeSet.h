// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GASCourseAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GASCourseCharBaseAttributeSet.generated.h"

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
class GASCOURSE_API UGASCourseCharBaseAttributeSet : public UGASCourseAttributeSet
{
	GENERATED_BODY()

public:

	UGASCourseCharBaseAttributeSet();

	// AttributeSet Overrides
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	
	UPROPERTY(BlueprintReadOnly, Category = "Character Base Attributes", ReplicatedUsing=OnRep_MovementSpeed)
	FGameplayAttributeData MovementSpeed;
	ATTRIBUTE_ACCESSORS(UGASCourseCharBaseAttributeSet, MovementSpeed)

	UPROPERTY(BlueprintReadOnly, Category = "Character Base Attributes", ReplicatedUsing=OnRep_CrouchSpeed)
	FGameplayAttributeData CrouchSpeed;
	ATTRIBUTE_ACCESSORS(UGASCourseCharBaseAttributeSet, CrouchSpeed)

	UPROPERTY(BlueprintReadOnly, Category = "Character Base Attributes", ReplicatedUsing=OnRep_JumpZVelocityOverride)
	FGameplayAttributeData JumpZVelocityOverride;
	ATTRIBUTE_ACCESSORS(UGASCourseCharBaseAttributeSet, JumpZVelocityOverride)

	UPROPERTY(BlueprintReadOnly, Category = "Character Base Attributes", ReplicatedUsing=OnRep_AirControlOverride)
	FGameplayAttributeData AirControlOverride;
	ATTRIBUTE_ACCESSORS(UGASCourseCharBaseAttributeSet, AirControlOverride)
	
protected:
	
	UFUNCTION()
	virtual void OnRep_MovementSpeed(const FGameplayAttributeData& OldMovementSpeed);
	
	UFUNCTION()
	virtual void OnRep_CrouchSpeed(const FGameplayAttributeData& OldCrouchSpeed);
	
	UFUNCTION()
	virtual void OnRep_JumpZVelocityOverride(const FGameplayAttributeData& OldJumpZVelocityOverride);
	
	UFUNCTION()
	virtual void OnRep_AirControlOverride(const FGameplayAttributeData& OldJAirControlOverride);
	
};
