// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/Interface.h"
#include "GASC_CharacterWeapon_Interface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGASC_CharacterWeapon_Interface : public UInterface
{
	GENERATED_BODY()
};

/**
 * @brief Interface for character weapon functionality, allowing for retrieval and modification of weapon mesh components.
 */
class GASCOURSE_API IGASC_CharacterWeapon_Interface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent)
	UStaticMeshComponent* GetWeaponMeshComponent();

	UFUNCTION(BlueprintNativeEvent)
	void SetWeaponMeshComponent(UStaticMeshComponent* WeaponMesh);
	
	
};
