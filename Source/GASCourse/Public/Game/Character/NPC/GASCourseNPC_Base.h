// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GASCourse/GASCourseCharacter.h"
#include "GASCourseNPC_Base.generated.h"

/**
 * @class AGASCourseNPC_Base
 * @brief Base class for Non-Playable Characters (NPCs) in the GASCourse project.
 *
 * This class inherits from AGASCourseCharacter and provides functionality for NPCs
 * specific to the GASCourse infrastructure. It includes overridden behavior for
 * possession, replication, and lifecycle management.
 */
UCLASS()
class GASCOURSE_API AGASCourseNPC_Base : public AGASCourseCharacter
{
	GENERATED_BODY()

public:
	AGASCourseNPC_Base(const FObjectInitializer& ObjectInitializer);
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

protected:
	
	virtual void OnRep_Controller() override;
	
	//Add GASCourseAbilitySystemComponent on PossessedBy
	virtual void PossessedBy(AController* NewController) override;
	
};
