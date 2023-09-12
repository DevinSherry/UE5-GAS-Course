// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Viewport/GASCourseGameViewportClient.h"

void UGASCourseGameViewportClient::Init(FWorldContext& WorldContext, UGameInstance* OwningGameInstance,
                                        bool bCreateNewAudioDevice)
{
	Super::Init(WorldContext, OwningGameInstance, bCreateNewAudioDevice);
}

bool UGASCourseGameViewportClient::RequiresUncapturedAxisInput() const
{
	return false;
}

void UGASCourseGameViewportClient::Activated(FViewport* InViewport, const FWindowActivateEvent& InActivateEvent)
{
	Super::Activated(Viewport, InActivateEvent);
	if(GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(CenterMouseCursorTimerHandle, this, &ThisClass::OnViewportBeginDraw, 0.1f, false);
	}
}

void UGASCourseGameViewportClient::SetViewport(FViewport* InViewportFrame)
{
	Super::SetViewport(InViewportFrame);
}

void UGASCourseGameViewportClient::OnViewportBeginDraw()
{
	if(GetWorld())
	{
		FVector2D Viewportsize;
		GetViewportSize(Viewportsize);
		const int32 X = static_cast<int32>(Viewportsize.X * 0.5f);
		const int32 Y = static_cast<int32>(Viewportsize.Y * 0.5f);
	
		Viewport->SetMouse(X,Y);
		GetWorld()->GetTimerManager().ClearTimer(CenterMouseCursorTimerHandle);
	}
}
