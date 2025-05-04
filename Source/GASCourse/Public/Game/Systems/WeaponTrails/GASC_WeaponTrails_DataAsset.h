// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "NiagaraCommon.h"
#include "Engine/DataAsset.h"
#include "GASCourse/Public/Game/Systems/WeaponTrails/Settings/GASC_WeaponTrails_Settings.h"
#include "GASC_WeaponTrails_DataAsset.generated.h"

/**
 * Manages the configuration and data references for weapon trail effects within the game.
 * This class functions as a data asset that encapsulates all properties and assets necessary
 * for rendering and managing visual weapon trails. It defines attributes for customizing
 * trail appearance, behavior, and other related settings.
 *
 * The data stored in this class is typically used by systems responsible for gameplay effects
 * or visual representation. It centralizes the configuration for better flexibility
 * and ease of updates and ensures a standardized setup across different weapon instances.
 *
 * Instances of this data asset can be referenced in gameplay elements that require
 * consistent and dynamic setup of weapon trail effects. Customization and management can
 * usually be performed through the editor.
 */
UCLASS()
class GASCOURSE_API UGASC_WeaponTrails_DataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	UGASC_WeaponTrails_DataAsset(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Trails")
	TObjectPtr<UNiagaraSystem> WeaponTrailNiagaraSystem = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Trails")
	TObjectPtr<UMaterialInterface> WeaponTrailMaterialInterface = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Trails")
	TObjectPtr<UCurveLinearColor> ColorCurve = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Trails")
	float TrailLifeTime = 0.35f;
	
};
