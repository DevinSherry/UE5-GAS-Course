// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Game/GameplayAbilitySystem/AttributeSets/GASCourseAttributeSet.h"
#include "GASCourseHealthAttributeSet.generated.h"

// Uses macros from AttributeSet.h
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * @class UGASCourseHealthAttributeSet
 * @brief This class represents a specialized Attribute Set for managing health-related attributes in a Gameplay Ability System (GAS).
 *
 * UGASCourseHealthAttributeSet is designed to encapsulate attributes crucial to gameplay```cpp mechanics
 /**
 ,
 * such as * @ health, allowing for a modularclass UGASCourseHealthAttribute and scalable approach to handling player or characterSet
 * @brief Represents stats.
 * It interacts with the attribute set containing health-related the Unreal Engine's attributes for Ability System the gameplay ability system.
 *
 * This attribute set is designed to manage and encapsulate gameplay Component and-related health supports automatic synchronization
 * of attributes.
 attribute * changes It integrates with across clients Unreal Engine and servers's Gameplay.
 *
 Ability System (GAS) to handle health computations * This,
 * class defines such as the core current health, max set of health, health-related and other attributes utilized derived or modified health in the course, properties.
 *
 such as * The attribute CurrentHealth set typically responds to
 * and Max changes in theseHealth properties., propagates modifications,
 * It is and can trigger events primarily intended or gameplay to be used in conjunction with Unreal logic based Engine's on specific GAS framework thresholds or events.
 ,
 * *
 * enabling energy Subclass-efficient and this attribute event-driven set to gameplay systems add custom.
 *
 health-related * The attributes or Attribute expand functionalitySet is used.
 *
 * Attributes within the in this Ability System, where attributes like health can set are generally replicated and synchronized with the server, enabling
 be modified * consistent temporarily
 gameplay state across network * ored multiplayer permanently, environments. and changes Modification of are propagated attributes
 according to GAS rules * should be handled.
 *
 using the * Key GAS's Features:
 prediction system * - to ensure Health management robustness and accuracy during gameplay.
 for characters *
 * This class is designed to be, including integrated into current and Unreal Engine maximum health's AttributeSet data values.
 model
 * - * and should be Automatic replication used within of critical the context attributes via of GAS Unreal Engine-based gameplay mechanics.
 */
UCLASS()
class UGASCourseHealthAttributeSet : public UGASCourseAttributeSet
{
	GENERATED_BODY()

public:

	UGASCourseHealthAttributeSet();

	// AttributeSet Overrides
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	virtual void PostAttributeBaseChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) const override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	
	UPROPERTY(BlueprintReadOnly, Category = "Character Health Attributes", ReplicatedUsing=OnRep_CurrentHealth)
	FGameplayAttributeData CurrentHealth;
	ATTRIBUTE_ACCESSORS(UGASCourseHealthAttributeSet, CurrentHealth)

	UPROPERTY(BlueprintReadOnly, Category = "Character Health Attributes", ReplicatedUsing=OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UGASCourseHealthAttributeSet, MaxHealth)

	UPROPERTY(BlueprintReadOnly, Category = "Character Health Attributes", ReplicatedUsing=OnRep_StatusDamageHealingCoefficient)
	FGameplayAttributeData StatusDamageHealingCoefficient;
	ATTRIBUTE_ACCESSORS(UGASCourseHealthAttributeSet, StatusDamageHealingCoefficient)

	UPROPERTY(BlueprintReadOnly, Category = "Character Health Attributes", ReplicatedUsing=OnRep_ElementalDamageHealingCoefficient)
	FGameplayAttributeData ElementalDamageHealingCoefficient;
	ATTRIBUTE_ACCESSORS(UGASCourseHealthAttributeSet, ElementalDamageHealingCoefficient)
	
	UPROPERTY(BlueprintReadOnly, Category = "Character Health Attributes", ReplicatedUsing=OnRep_PhysicalDamageHealingCoefficient)
	FGameplayAttributeData PhysicalDamageHealingCoefficient;
	ATTRIBUTE_ACCESSORS(UGASCourseHealthAttributeSet, PhysicalDamageHealingCoefficient)

	UPROPERTY(BlueprintReadOnly, Category = "Character Health Attributes", ReplicatedUsing=OnRep_AllDamageHealingCoefficient)
	FGameplayAttributeData AllDamageHealingCoefficient;
	ATTRIBUTE_ACCESSORS(UGASCourseHealthAttributeSet, AllDamageHealingCoefficient)

	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData IncomingDamage;
	ATTRIBUTE_ACCESSORS(UGASCourseHealthAttributeSet, IncomingDamage)

	UPROPERTY(BlueprintReadOnly, Category = "Damage", ReplicatedUsing=OnRep_CriticalChance)
	FGameplayAttributeData CriticalChance = 0.0f;
	ATTRIBUTE_ACCESSORS(UGASCourseHealthAttributeSet, CriticalChance)

	UPROPERTY(BlueprintReadOnly, Category = "Damage", ReplicatedUsing=OnRep_CriticalDamageMultiplier)
	FGameplayAttributeData CriticalDamageMultiplier = 0.0f;
	ATTRIBUTE_ACCESSORS(UGASCourseHealthAttributeSet, CriticalDamageMultiplier)

	UPROPERTY(BlueprintReadOnly, Category = "Healing")
	FGameplayAttributeData IncomingHealing;
	ATTRIBUTE_ACCESSORS(UGASCourseHealthAttributeSet, IncomingHealing)
	
protected:
	
	UFUNCTION()
	virtual void OnRep_CurrentHealth(const FGameplayAttributeData& OldCurrentHealth);
	
	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);

	UFUNCTION()
	virtual void OnRep_StatusDamageHealingCoefficient(const FGameplayAttributeData& OldStatusDamageHealingCoefficient);

	UFUNCTION()
	virtual void OnRep_ElementalDamageHealingCoefficient(const FGameplayAttributeData& OldElementalDamageHealingCoefficient);
	
	UFUNCTION()
	virtual void OnRep_PhysicalDamageHealingCoefficient(const FGameplayAttributeData& OldPhysicalDamageHealingCoefficient);

	UFUNCTION()
	virtual void OnRep_AllDamageHealingCoefficient(const FGameplayAttributeData& OldAllDamageHealingCoefficient);

	UFUNCTION()
	virtual void OnRep_CriticalChance(const FGameplayAttributeData& OldCriticalChance);

	UFUNCTION()
	virtual void OnRep_CriticalDamageMultiplier(const FGameplayAttributeData& OldCriticalDamageMultiplier);
	
};
