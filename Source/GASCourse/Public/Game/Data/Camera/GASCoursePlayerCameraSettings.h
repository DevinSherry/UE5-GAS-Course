// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataAsset.h"
#include "InputAction.h"
#include "GASCoursePlayerCameraSettings.generated.h"

/**
 * 
 */
UCLASS()
class GASCOURSE_API UGASCoursePlayerCameraSettings : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GASCourse|Camera Settings", meta = (AllowPrivateAccess = "true"))
	float MaxCameraBoomDistance = 500.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GASCourse|Camera Settings", meta = (AllowPrivateAccess = "true"))
	float MinCameraBoomDistance = 250.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GASCourse|Camera Settings", meta = (AllowPrivateAccess = "true"))
	float DefaultCameraBoomLength = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GASCourse|Camera Settings", meta = (AllowPrivateAccess = "true"))
	float DefaultSocketOffsetZ = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GASCourse|Camera Settings", meta = (AllowPrivateAccess = "true"))
	float CameraZoomDistanceStep = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GASCourse|Camera Settings", meta = (AllowPrivateAccess = "true"))
	float CameraMovementSpeedMin = 30.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GASCourse|Camera Settings", meta = (AllowPrivateAccess = "true"))
	float CameraMovementSpeedMax = 30.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GASCourse|Camera Settings", meta = (AllowPrivateAccess = "true"))
	float CameraMaxVectorDistance = 3000.0f;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "GASCourse|Camera Settings", meta = (AllowPrivateAccess = "true"))
	UCurveFloat* RecenterCameraCurve;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "GASCourse|Camera Settings", meta = (AllowPrivateAccess = "true"))
	UCurveFloat* MoveCameraCurve;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "GASCourse|Camera Settings", meta = (AllowPrivateAccess = "true"))
	UCurveFloat* RotateCameraCurve;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "GASCourse|Camera Settings", meta = (AllowPrivateAccess = "true"))
	UCurveFloat* CameraEdgePanningCurve;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "GASCourse|Camera Settings", meta = (AllowPrivateAccess = "true"))
	float RecenterCameraInterpSpeed = 0.1f;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "GASCourse|Camera Settings", meta = (AllowPrivateAccess = "true"))
	float MoveCameraInterpSpeed = 0.1f;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "GASCourse|Camera Settings", meta = (AllowPrivateAccess = "true"))
	float CameraEdgePanningInterpSpeed = 0.1f;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "GASCourse|Camera Settings", meta = (AllowPrivateAccess = "true"))
	float MinCameraPitchAngle = -10.0f;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "GASCourse|Camera Settings", meta = (AllowPrivateAccess = "true"))
	float MaxCameraPitchAngle = 40.0f;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "GASCourse|Camera Settings", meta = (AllowPrivateAccess = "true"))
	float CameraRotationSpeedMultiplier = 1.0f;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "GASCourse|Camera Settings", meta = (AllowPrivateAccess = "true"))
	float RotateCameraInterpSpeed = 1.0f;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "GASCourse|Camera Settings", meta = (AllowPrivateAccess = "true"))
	float EdgePanningSpeedMin = 30.0f;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "GASCourse|Camera Settings", meta = (AllowPrivateAccess = "true"))
	float EdgePanningSpeedMax = 30.0f;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "GASCourse|Camera Settings", meta = (AllowPrivateAccess = "true"))
	UInputAction* EnableRotateCameraAxis;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "GASCourse|Camera Settings", meta = (AllowPrivateAccess = "true"))
	float CameraTargetOffsetZDownTraceLength = -5000.0f;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "GASCourse|Camera Settings", meta = (AllowPrivateAccess = "true"))
	float CameraTargetOffsetZDownTraceRadius = 30.0f;

	/**
	 * Minimum distance at which the camera is detached from the character.
	 *
	 * This variable controls the minimum distance at which the camera can detach from the character and start moving independently.
	 * It is used to determine if the camera should snap back and attach to the character, if they try to move, while within this minimum distance.
	 *
	 * @see AGASCoursePlayerCharacter
	 * @see CameraBoom
	 * @see FollowCamera
	 */
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "GASCourse|Camera Settings", meta = (AllowPrivateAccess = "true"))
	float MinDistanceToDetachCamera = 300.0f;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "GASCourse|Camera Settings", meta = (AllowPrivateAccess = "true"))
	bool bAutoReAttachCameraWithinMinDistance = true;
	
};
