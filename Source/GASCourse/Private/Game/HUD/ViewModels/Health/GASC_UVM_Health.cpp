// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/HUD/ViewModels/Health/GASC_UVM_Health.h"
#include "Kismet/KismetMathLibrary.h"


UGASC_UVM_Health::UGASC_UVM_Health()
{
}

float UGASC_UVM_Health::GetCurrentHealth() const
{
	return CurrentHealth;
}

float UGASC_UVM_Health::GetDelayedCurrentHealth() const
{
	return DelayedCurrentHealth;
}

float UGASC_UVM_Health::GetDelayedHealthPercentage() const
{
	if(MaxHealth != 0.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Current Health: %f | Max Health: %f"), DelayedCurrentHealth, MaxHealth);
		return DelayedCurrentHealth / MaxHealth;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Max Health == 0.0!"));
		return 0.0f;
	}
}

float UGASC_UVM_Health::GetMaxHealth() const
{
	return MaxHealth;
}

void UGASC_UVM_Health::SetCurrentHealth(const float& NewCurrentHealth)
{
	if(UE_MVVM_SET_PROPERTY_VALUE(CurrentHealth, NewCurrentHealth))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetHealthPercentage);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetHealthAsDisplay);
	}
}

void UGASC_UVM_Health::SetDelayedCurrentHealth(const float& NewDelayedCurrentHealth)
{
	if(UE_MVVM_SET_PROPERTY_VALUE(DelayedCurrentHealth, NewDelayedCurrentHealth))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetDelayedHealthPercentage);
	}
}

void UGASC_UVM_Health::SetMaxHealth(const float& NewMaxHealth)
{
	if(UE_MVVM_SET_PROPERTY_VALUE(MaxHealth, NewMaxHealth))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetHealthPercentage);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetHealthAsDisplay);
	}
}

float UGASC_UVM_Health::GetHealthPercentage() const
{
	if(MaxHealth != 0.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Current Health: %f | Max Health: %f"), CurrentHealth, MaxHealth);
		return CurrentHealth / MaxHealth;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Max Health == 0.0!"));
		return 0.0f;
	}
}

FText UGASC_UVM_Health::GetHealthAsDisplay() const
{
	uint32 CurrentHealthAsInt = UKismetMathLibrary::FFloor(CurrentHealth);
	FString CurrentHealthString = FString::FromInt(CurrentHealthAsInt);

	uint32 MaxHealthAsInt = UKismetMathLibrary::FFloor(MaxHealth);
	FString MaxHealthString = FString::FromInt(MaxHealthAsInt);

	FString HealthString = CurrentHealthString + "/" + MaxHealthString;
	return FText::AsCultureInvariant(HealthString);
}

