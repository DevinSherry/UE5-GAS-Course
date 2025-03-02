// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MVVMViewModelBase.h"
#include "GASC_UVM_Health.generated.h"

/**
 * UGASC_UVM_Health is a ViewModel class extending UMVVMViewModelBase, designed to manage the health
 * properties of an actor, including current health, maximum health, and health display information.
 * It provides methods to get and set health values, calculate the health percentage, and format health
 * information as text for display purposes.
 */
UCLASS()
class GASCOURSE_API UGASC_UVM_Health : public UMVVMViewModelBase
{
	GENERATED_BODY()
	
public:

	UGASC_UVM_Health();

	UFUNCTION(BlueprintPure, FieldNotify)
	float GetCurrentHealth() const;

	UFUNCTION(BlueprintPure, FieldNotify)
	float GetDelayedCurrentHealth() const;

	UFUNCTION(BlueprintCallable)
	void SetCurrentHealth(const float& NewCurrentHealth);
	
	UFUNCTION(BlueprintCallable)
	void SetDelayedCurrentHealth(const float& NewDelayedCurrentHealth);

	UFUNCTION(BlueprintPure, FieldNotify)
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable)
	void SetMaxHealth(const float& NewMaxHealth);

	UFUNCTION(BlueprintPure, FieldNotify)
	float GetHealthPercentage() const;

	UFUNCTION(BlueprintPure, FieldNotify)
	float GetDelayedHealthPercentage() const;

	UFUNCTION(BlueprintPure, FieldNotify)
	FText GetHealthAsDisplay() const;

private:

	UPROPERTY(BlueprintReadOnly, FieldNotify, Setter, Getter,  meta=(AllowPrivateAccess))
	float CurrentHealth = 0.0f;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Setter, Getter,  meta=(AllowPrivateAccess))
	float DelayedCurrentHealth = 0.0f;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Setter, Getter,  meta=(AllowPrivateAccess))
	float MaxHealth = 0.0f;
	
};
