// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GASCourseCharacter.h"
#include "GASCourseNPC_Base.generated.h"

/**
 * 
 */
UCLASS()
class GASCOURSE_API AGASCourseNPC_Base : public AGASCourseCharacter
{
	GENERATED_BODY()

public:
	AGASCourseNPC_Base(const FObjectInitializer& ObjectInitializer);
	
	//Add GASCourseAbilitySystemComponent on PossessedBy
	virtual void PossessedBy(AController* NewController) override;
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	
};