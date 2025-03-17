// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameStateBase.h"
#include "GASCourseGameStateBase.generated.h"

/**
 * @class AGASCourseGameStateBase
 * @brief Represents a custom game state based on AGameStateBase for GASCourse.
 *
 * This class is designed to provide additional functionality specific to the GASCourse module.
 * It overrides fundamental lifecycle methods from the base AGameStateBase class to facilitate
 * custom behavior during the game state transitions.
 *
 * @details
 * - The class ensures ticking functionality is enabled during initialization.
 * - It serves as the main game state handler for game logic that requires state awareness.
 */

UCLASS()
class GASCOURSE_API AGASCourseGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

	AGASCourseGameStateBase(const class FObjectInitializer& ObjectInitializer);
	
	void BeginPlay() override;
	void Tick(float DeltaSeconds) override;
	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
