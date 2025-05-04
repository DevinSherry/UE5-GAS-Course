// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DeveloperSettings.h"
#include "GASC_MeleeSubsystem_Settings.generated.h"

/**
 * @brief The UGASC_MeleeSubsystem_Settings class is a configuration class derived from UDeveloperSettings.
 *
 * This class is used to store global settings for the melee trace subsystem of the GAS Course project.
 * It provides configurable properties for debugging purposes and is marked for use
 * as a game-wide configuration class using the Config system.
 *
 * @details
 * The settings can be accessed through the editor or via code, and the configurations
 * are stored in the designated config file. Developers can use this class to customize
 * debug visuals and their behavior for the melee tracing in the gameplay systems.
 *
 * The class includes the following configurable properties:
 * - `MeleeTraceDebugColor`: Determines the color to be used for debug drawing melee traces.
 * - `DebugDrawTime`: Specifies the duration for which the debug draw visuals will persist.
 */
UCLASS(Config=Game, defaultconfig, meta = (DisplayName="GASCourse Melee Trace System Settings"))
class GASCOURSE_API UGASC_MeleeSubsystem_Settings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	/**
	 * @brief Determines the color used for debug drawing melee traces in the gameplay systems.
	 *
	 * This property configures the debug visual color for melee trace lines in the Melee Trace Subsystem.
	 * It is editable and can be set through both the editor and configuration files.
	 *
	 * @details
	 * The value is of type FColor, with the default set to Red. This setting allows developers
	 * to easily identify and visualize melee traces during debugging or when debugging is enabled.
	 */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Melee Trace Subsystem")
	FColor MeleeTraceDebugColor = FColor::Red;

	/**
	 * @brief Defines the debug color for hit indicators in melee trace debugging.
	 *
	 * This property specifies the color used to represent hits in debug visuals for the
	 * Melee Trace Subsystem during gameplay or testing. It is configurable and intended
	 * to assist in visualizing hit results when debugging melee interactions.
	 *
	 * @details
	 * The default value is set to FColor::Green. Developers can modify this color via the
	 * editor or configuration files to better distinguish hit indicators within debug traces.
	 */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Melee Trace Subsystem")
	FColor MeleeTraceHitDebugColor = FColor::Green;

	/**
	 * @brief Defines the debug color for interpolated trace visuals in melee debugging.
	 *
	 * This property sets the color used to represent interpolated positions in the debug visuals
	 * for the Melee Trace Subsystem. It is primarily intended for debugging purposes to help
	 * visualize intermediate points or trace interpolation in the gameplay systems.
	 *
	 * @details
	 * The default value is set to FColor::Transparent. Developers can adjust this color via
	 * the editor or configuration files to better suit their debugging needs, such as
	 * differentiating interpolated traces from other debug visuals.
	 */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Melee Trace Subsystem")
	FColor MeleeTraceInterpolatedDebugColor = FColor::Transparent;

	/**
	 * @brief Specifies the duration for which debug draw visuals will persist for melee traces.
	 *
	 * This property defines the time, in seconds, that debug lines or visuals representing melee traces
	 * will remain visible during gameplay or debugging sessions. It is primarily used for visualizing
	 * melee trace interactions in the debugging process.
	 *
	 * @details
	 * The DebugDrawTime value is editable through the editor or configuration files and serves to give
	 * developers greater flexibility in managing the persistence of debug visuals. The default value is 1.0 seconds.
	 */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Melee Trace Subsystem")
	float DebugDrawTime = 1.0f;
	
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Melee Trace Subsystem")
	TArray<TEnumAsByte<EObjectTypeQuery>> CollisionObjectTypes;

	UGASC_MeleeSubsystem_Settings();
	
};
