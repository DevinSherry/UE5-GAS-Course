// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/CameraManager/GASCourseCameraManager.h"

#include "MaterialHLSLTree.h"
#include "GASCourse/GASCourseCharacter.h"


void AGASCourseCameraManager::UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime)
{
	Super::UpdateViewTarget(OutVT, DeltaTime);
}

void AGASCourseCameraManager::ModifyViewTargetLocation(FVector NewLocation, float BlendTime)
{
	FTViewTarget NewViewTarget;

	NewViewTarget = ViewTarget;
	NewViewTarget.POV.Location += NewLocation;
	
	BlendViewTargets(ViewTarget, NewViewTarget, BlendTime);
}