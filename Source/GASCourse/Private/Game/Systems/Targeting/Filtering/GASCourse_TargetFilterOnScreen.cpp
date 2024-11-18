// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Systems/Targeting/Filtering/GASCourse_TargetFilterOnScreen.h"


bool UGASCourse_TargetFilterOnScreen::ShouldFilterTarget(const FTargetingRequestHandle& TargetingHandle,
	const FTargetingDefaultResultData& TargetData) const
{
	if (const FTargetingSourceContext* SourceContext = FTargetingSourceContext::Find(TargetingHandle))
	{
		if (APawn* InstigatorPawn = Cast<APawn>(SourceContext->SourceActor))
		{
			//APlayerController* PlayerController = Cast<APlayerController>(InstigatorPawn->GetLocalViewingPlayerController());
			if(!TargetData.HitResult.HasValidHitObjectHandle())
			{
				return Super::ShouldFilterTarget(TargetingHandle, TargetData);
			}
			
			FVector TargetLocation = TargetData.HitResult.Location;
			if(APlayerController* PlayerController = Cast<APlayerController>(InstigatorPawn->GetController()))
			{
				int32 ScreenSizeX;
				int32 ScreenSizeY;
				PlayerController->GetViewportSize(ScreenSizeX, ScreenSizeY);

				FVector2d ScreenPosition;
				PlayerController->ProjectWorldLocationToScreen(TargetLocation, ScreenPosition, false);

				return !(ScreenPosition.X < ScreenSizeX && ScreenPosition.X >= ViewThresholdMinX
					&& ScreenPosition.Y < ScreenSizeY && ScreenPosition.Y >= ViewThresholdMinY);
			}
		}
	}
			
	return Super::ShouldFilterTarget(TargetingHandle, TargetData);
}
