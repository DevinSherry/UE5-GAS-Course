// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/GameplayAbilitySystem/AttributeSets/GASCourseAttributeSet.h"
#include "GASC_ResourceAttributeSet.generated.h"

// Uses macros from AttributeSet.h
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * UGASC_ResourceAttributeSet is a subclass of UGASCourseAttributeSet and serves as a specialized
 * container for gameplay resource attributes used in the Gameplay Ability System (GAS).
 *
 * This class extends the functionality of UGASCourseAttributeSet, allowing for additional
 * resource-specific attributes to be defined and managed. These attributes can be critical to
 * gameplay, particularly in mechanics such as health, mana, stamina, or other resource-based
 * metrics.
 *
 * The class utilizes Unreal Engine's attribute system, allowing for game state changes to be
 * replicated, notified, and automatically managed when used alongside game-specific
 * functionalities like gameplay abilities.
 *
 * Primary Features:
 * - Inherits from UGASCourseAttributeSet and benefits from its replication methods and utilities.
 * - Designed to define and manage specialized resource-related gameplay attributes.
 * - Intended for use in extending projects utilizing the Gameplay Ability System (GAS).
 *
 * This class can be further extended or used as it is to define resource-based gameplay attributes
 * for characters and other applicable game entities.
 */
UCLASS()
class GASCOURSE_API UGASC_ResourceAttributeSet : public UGASCourseAttributeSet
{
	GENERATED_BODY()
	
public:

	UGASC_ResourceAttributeSet();

	// AttributeSet Overrides
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	
	UPROPERTY(BlueprintReadOnly, Category = "Character Resource Attributes", ReplicatedUsing=OnRep_CurrentResource)
	FGameplayAttributeData CurrentResource;
	ATTRIBUTE_ACCESSORS(UGASC_ResourceAttributeSet, CurrentResource)

	UPROPERTY(BlueprintReadOnly, Category = "Character Resource Attributes", ReplicatedUsing=OnRep_MaxResource)
	FGameplayAttributeData MaxResource;
	ATTRIBUTE_ACCESSORS(UGASC_ResourceAttributeSet, MaxResource)

	UPROPERTY(BlueprintReadOnly, Category = "Character Resource Attributes", ReplicatedUsing=OnRep_ResourceRegenerationRate)
	FGameplayAttributeData ResourceRegenerationRate = 5.0f;
	ATTRIBUTE_ACCESSORS(UGASC_ResourceAttributeSet, ResourceRegenerationRate)

	UPROPERTY(BlueprintReadOnly, Category = "Character Resource Attributes", ReplicatedUsing=OnRep_ResourceRegenerationDelay)
	FGameplayAttributeData ResourceRegenerationDelay = 1.0f;
	ATTRIBUTE_ACCESSORS(UGASC_ResourceAttributeSet, ResourceRegenerationDelay)

protected:
	
	UFUNCTION()
	virtual void OnRep_CurrentResource(const FGameplayAttributeData& OldCurrentResource);
	
	UFUNCTION()
	virtual void OnRep_MaxResource(const FGameplayAttributeData& OldMaxResource);

	UFUNCTION()
	virtual void OnRep_ResourceRegenerationRate(const FGameplayAttributeData& OldResourceRegenerationRate);
	
	UFUNCTION()
	virtual void OnRep_ResourceRegenerationDelay(const FGameplayAttributeData& OldResourceRegenerationDelay);
	
};
