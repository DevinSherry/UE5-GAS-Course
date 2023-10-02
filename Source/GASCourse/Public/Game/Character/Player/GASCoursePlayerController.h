// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

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

	virtual void BeginPlayingState() override;

	UFUNCTION(BlueprintCallable, Category = "GASCourse|PlayerController")
	AGASCoursePlayerState* GetGASCoursePlayerState() const;

	UFUNCTION(BlueprintCallable, Category = "GASCourse|PlayerController")
	UGASCourseAbilitySystemComponent* GetGASCourseAbilitySystemComponent() const;
	
	virtual void PreProcessInput(const float DeltaTime, const bool bGamePaused) override;
	/** Method called after processing input */
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;
	
	UFUNCTION(Category = "GASCourse|PlayerController|Movement")
	FORCEINLINE FVector3d GetCachedDestination() const {return CachedDestination;};

	UFUNCTION(Category = "GASCourse|PlayerController|Movement")
	FORCEINLINE void SetCachedDestination(const FVector3d& NewCachedDestination)
	{
		if(NewCachedDestination != GetCachedDestination())
		{
			CachedDestination = NewCachedDestination;
		}
	}

	virtual void OnPossess(APawn* InPawn) override;

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GASCourse|HUD")
	void CreateHUD();

	void CreateHUD_Implementation();

protected:

	virtual void OnRep_PlayerState() override;

private:

	FVector3d CachedDestination = FVector3d(0.0f,0.0f,0.0f);
	
};
