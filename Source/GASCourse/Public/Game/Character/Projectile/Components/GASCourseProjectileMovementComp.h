// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/ProjectileMovementComponent.h"
#include "GASCourseProjectileMovementComp.generated.h"

/**
 * 
 */
UCLASS()
class UGASCourseProjectileMovementComp : public UProjectileMovementComponent
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Homing")
	bool bDisableHomingBasedOnDotProduct;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Homing")
	float DisableHomingDotProductMin = 0.0f;
	
};
