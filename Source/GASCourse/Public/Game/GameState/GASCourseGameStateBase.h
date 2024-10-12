// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameStateBase.h"
#include "CogCommon.h"
#include "GASCourseGameStateBase.generated.h"

/**
 * 
 */

class UCogWindowManager;

UCLASS()
class GASCOURSE_API AGASCourseGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

	AGASCourseGameStateBase(const class FObjectInitializer& ObjectInitializer);

	
	// To make sure it doesn't get garbage collected.
	UPROPERTY()
	TObjectPtr<UObject> CogWindowManagerRef = nullptr;

#if ENABLE_COG

	void InitializeCog();
	TObjectPtr<UCogWindowManager> CogWindowManager = nullptr;
#endif //ENABLE_COG


	void BeginPlay() override;
	void Tick(float DeltaSeconds) override;
	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
