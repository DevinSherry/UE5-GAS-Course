// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Game/GameplayAbilitySystem/AttributeSets/GASCourseAttributeSet.h"
#include "GASC_StaminaAttributeSet.generated.h"

// Uses macros from AttributeSet.h
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * UGASC_StaminaAttributeSet is a specialized attribute set used for managing stamina-related attributes
 * in the GasCourse system. It serves as a container and handler for gameplay attributes such as
 * current stamina, maximum stamina, stamina regeneration rate, and stamina regeneration delay.
 */
UCLASS()
class GASCOURSE_API UGASC_StaminaAttributeSet : public UGASCourseAttributeSet
{
	GENERATED_BODY()
	
public:

	UGASC_StaminaAttributeSet();

	// AttributeSet Overrides
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	
	UPROPERTY(BlueprintReadOnly, Category = "Character Resource Attributes", ReplicatedUsing=OnRep_CurrentStamina)
	FGameplayAttributeData CurrentStamina;
	ATTRIBUTE_ACCESSORS(UGASC_StaminaAttributeSet, CurrentStamina)

	UPROPERTY(BlueprintReadOnly, Category = "Character Resource Attributes", ReplicatedUsing=OnRep_MaxStamina)
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(UGASC_StaminaAttributeSet, MaxStamina)

	UPROPERTY(BlueprintReadOnly, Category = "Character Resource Attributes", ReplicatedUsing=OnRep_StaminaRegenerationRate)
	FGameplayAttributeData StaminaRegenerationRate = 5.0f;
	ATTRIBUTE_ACCESSORS(UGASC_StaminaAttributeSet, StaminaRegenerationRate)

	UPROPERTY(BlueprintReadOnly, Category = "Character Resource Attributes", ReplicatedUsing=OnRep_StaminaRegenerationDelay)
	FGameplayAttributeData StaminaRegenerationDelay = 1.0f;
	ATTRIBUTE_ACCESSORS(UGASC_StaminaAttributeSet, StaminaRegenerationDelay)

protected:
	
	UFUNCTION()
	virtual void OnRep_CurrentStamina(const FGameplayAttributeData& OldCurrentStamina);
	
	UFUNCTION()
	virtual void OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina);

	UFUNCTION()
	virtual void OnRep_StaminaRegenerationRate(const FGameplayAttributeData& OldStaminaRegenerationRate);
	
	UFUNCTION()
	virtual void OnRep_StaminaRegenerationDelay(const FGameplayAttributeData& OldStaminaRegenerationDelay);
	
};
