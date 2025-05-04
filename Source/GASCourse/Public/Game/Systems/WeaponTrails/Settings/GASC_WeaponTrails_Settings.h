// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DeveloperSettings.h"
#include "GASC_WeaponTrails_Settings.generated.h"

/**
 * @class UGASC_WeaponTrails_Settings
 * @brief Configuration settings for weapon trail materials in the GASCourse project.
 *
 * This class serves as a derived UDeveloperSettings object for managing weapon trail-related settings.
 * These settings are defined in a configuration file and can be utilized to map weapon trail types
 * to their corresponding material interfaces.
 */
UCLASS(Config=Game, defaultconfig, meta = (DisplayName="GASCourse Weapon Trail Settings"))
class GASCOURSE_API UGASC_WeaponTrails_Settings : public UDeveloperSettings
{
	GENERATED_BODY()
	
	//Maybe define FNames here for niagara override parameters?
};
