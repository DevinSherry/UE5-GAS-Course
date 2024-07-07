// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MVVMViewModelBase.h"
#include "GASC_UVM_Health.generated.h"

/**
 * 
 */
UCLASS()
class GASCOURSE_API UGASC_UVM_Health : public UMVVMViewModelBase
{
	GENERATED_BODY()
	
public:

	UGASC_UVM_Health();

	UFUNCTION(BlueprintPure, FieldNotify)
	float GetCurrentHealth() const;

	UFUNCTION(BlueprintCallable)
	void SetCurrentHealth(const float& NewCurrentHealth);

	UFUNCTION(BlueprintPure, FieldNotify)
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable)
	void SetMaxHealth(const float& NewMaxHealth);

	UFUNCTION(BlueprintPure, FieldNotify)
	float GetHealthPercentage() const;

	UFUNCTION(BlueprintPure, FieldNotify)
	FText GetHealthAsDisplay() const;

private:

	UPROPERTY(BlueprintReadOnly, FieldNotify, Setter, Getter,  meta=(AllowPrivateAccess))
	float CurrentHealth = 0.0f;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Setter, Getter,  meta=(AllowPrivateAccess))
	float MaxHealth = 0.0f;
	
};
