// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GASCoursePlayerState.h"
#include "GameFramework/PlayerController.h"
#include "GASCoursePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class GASCOURSE_API AGASCoursePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	
	AGASCoursePlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category = "Lyra|PlayerController")
	AGASCoursePlayerState* GetGASCoursePlayerState() const;

	UFUNCTION(BlueprintCallable, Category = "Lyra|PlayerController")
	UGASCourseAbilitySystemComponent* GetGASCourseAbilitySystemComponent() const;
	
	virtual void PreProcessInput(const float DeltaTime, const bool bGamePaused) override;
	/** Method called after processing input */
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;
	
};
