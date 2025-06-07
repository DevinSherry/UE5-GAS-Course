// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TargetingSystem/TargetingPreset.h"
#include "GASC_TargetingPreset_AOE.generated.h"

/**
 * UGASC_TargetingPreset_AOE is a targeting preset class designed to manage area-of-effect (AOE)
 * targeting behavior in Unreal Engine's Gameplay Ability System. This class is derived from
 * UTargetingPreset.
 *
 * The class initializes and configures an AOE selection task to define the targeting shape,
 * size, and collision parameters. It supports creating AOE-based targeting effects such as
 * spheres with adjustable radius and the ability to exclude specific actors like the source
 * actor or instigator actor.
 *
 * Key responsibilities:
 * - Sets up and configures an area-of-effect targeting task with specific shape types and dimensions.
 * - Configures collision channels and object types for the targeting task.
 * - Allows filtering and exclusion of source or instigator actors from the target results.
 *
 * This class provides flexibility for implementing AOE-based targeting within the Gameplay Ability System.
 */

UCLASS()
class GASCOURSE_API UGASC_TargetingPreset_AOE : public UTargetingPreset
{
	GENERATED_BODY()
	
public:

	UGASC_TargetingPreset_AOE(const FObjectInitializer& ObjectInitializer);
	
};
