// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Abilities/GameplayAbilityWorldReticle.h"
#include "GASCourseAbilityWorldReticle.generated.h"

/**
 * 
 */
UCLASS()
class GASCOURSE_API AGASCourseAbilityWorldReticle : public AGameplayAbilityWorldReticle
{
	GENERATED_BODY()

public:
	
	virtual void FaceTowardsImpactNormal(bool bInFaceImpactNormal);
	
	virtual void Tick(float DeltaSeconds) override;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, meta = (ExposeOnSpawn = "true"), Category = "Reticle")
	bool bFaceImpactNormal = true;
	
};
