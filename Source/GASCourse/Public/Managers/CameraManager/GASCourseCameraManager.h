// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "GASCourseCameraManager.generated.h"

/**
 * 
 */
UCLASS()
class GASCOURSE_API AGASCourseCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()
	
	virtual void UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime) override;

public:

	UFUNCTION(BlueprintCallable)
	virtual void ModifyViewTargetLocation(FVector NewLocation, float BlendTime);

private:

	UPROPERTY()
	bool bNewViewTarget = false;

	UPROPERTY()
	float NewViewTargetBlendTime = 0.0f;

	UPROPERTY()
	FVector NewViewTargetLocation;
};
