// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/GameViewportClient.h"
#include "GASCourseGameViewportClient.generated.h"

/**
 * 
 */



UCLASS()
class GASCOURSE_API UGASCourseGameViewportClient : public UGameViewportClient
{
	GENERATED_BODY()

	virtual void Init(FWorldContext& WorldContext, UGameInstance* OwningGameInstance, bool bCreateNewAudioDevice) override;
	virtual bool RequiresUncapturedAxisInput() const override;
	virtual void Activated(FViewport* InViewport, const FWindowActivateEvent& InActivateEvent) override;
	virtual void SetViewport(FViewport* InViewportFrame) override;

	void OnViewportBeginDraw();

	FTimerHandle CenterMouseCursorTimerHandle;
	
};
