// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/Input/GASCourseInputConfig.h"
#include "GASCourse/GASCourseCharacter.h"
#include "Components/TimelineComponent.h"
#include "Tasks/Task.h"
#include "InputAction.h"
#include "GASCoursePlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class GASCOURSE_API AGASCoursePlayerCharacter : public AGASCourseCharacter
{
	GENERATED_BODY()

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContextKBM;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContextGamepad;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GASCourse|Camera Settings", meta = (AllowPrivateAccess = "true"))
	float MaxCameraBoomDistance = 500.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GASCourse|Camera Settings", meta = (AllowPrivateAccess = "true"))
	float MinCameraBoomDistance = 250.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GASCourse|Camera Settings", meta = (AllowPrivateAccess = "true"))
	float CameraZoomDistanceStep = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GASCourse|Camera Settings", meta = (AllowPrivateAccess = "true"))
	float MaxCameraMovementSpeed = 30.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GASCourse|Camera Settings", meta = (AllowPrivateAccess = "true"))
	float CameraMaxVectorDistance = 3000.0f;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "GASCourse|Camera Settings", meta = (AllowPrivateAccess = "true"))
	UCurveFloat* RecenterCameraCurve;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "GASCourse|Camera Settings", meta = (AllowPrivateAccess = "true"))
	UCurveFloat* MoveCameraCurve;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "GASCourse|Camera Settings", meta = (AllowPrivateAccess = "true"))
	float RecenterCameraInterpSpeed = 0.1f;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "GASCourse|Camera Settings", meta = (AllowPrivateAccess = "true"))
	float MoveCameraInterpSpeed = 0.1f;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "GASCourse|Camera Settings", meta = (AllowPrivateAccess = "true"))
	float MinCameraPitchAngle = -10.0f;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "GASCourse|Camera Settings", meta = (AllowPrivateAccess = "true"))
	float MaxCameraPitchAngle = 40.0f;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "GASCourse|Camera Settings", meta = (AllowPrivateAccess = "true"))
	float EdgePanningSpeed = 30.0f;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "GASCourse|Camera Settings", meta = (AllowPrivateAccess = "true"))
	UInputAction* EnableRotateCameraAxis;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "GASCourse|Camera Settings", meta = (AllowPrivateAccess = "true"))
	float CameraTargetOffsetZDownTraceLength = -5000.0f;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "GASCourse|Camera Settings", meta = (AllowPrivateAccess = "true"))
	float CameraTargetOffsetZDownTraceRadius = 30.0f;

public:

	AGASCoursePlayerCharacter(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UGASCourseInputConfig> InputConfig;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GASCourse|Animation")
	TSubclassOf<UAnimInstance> UnArmedAnimLayer;

	void UpdateCharacterAnimLayer(TSubclassOf<UAnimInstance> NewAnimLayer) const;

protected:

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Add GASCourseAbilitySystemComponent on PossessedBy
	virtual void PossessedBy(AController* NewController) override;

	virtual void OnRep_PlayerState() override;
	virtual void OnRep_Controller() override;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	
	void Input_AbilityInputTagPressed(FGameplayTag InputTag);
	void Input_AbilityInputTagReleased(FGameplayTag InputTag);

	void Move(const FInputActionValue& Value) override;

	void Input_CameraZoom(const FInputActionInstance& InputActionInstance);
	void Input_MoveCamera(const FInputActionInstance& InputActionInstance);
	void Input_MoveCameraCompleted(const FInputActionInstance& InputActionInstance);
	void UpdateCameraBoomTargetOffset(const FVector& InCameraBoomTargetOffset) const;
	void Input_RecenterCamera(const FInputActionInstance& InputActionInstance);
	void Input_RotateCameraAxis(const FInputActionInstance& InputActionInstance);

	/** Called for left-click based movement */
	void PointClickMovement(const FInputActionValue& Value);
	void PointClickMovementStarted(const FInputActionValue& Value);
	void PointClickMovementCompleted(const FInputActionInstance& InputActionInstance);

	void MoveToMouseHitResultLocation();

	UFUNCTION()
	void RecenterCameraBoomTargetOffset();

	UFUNCTION()
	void RecenterCameraBoomTimelineFinished();

	UFUNCTION()
	void UpdateCameraMovementSpeed();

	UFUNCTION()
	void UpdateCameraMovementSpeedTimelineFinished();

	void CameraEdgePanning();

	UFUNCTION()
	void SetMousePositionToScreenCenter();

	UFUNCTION()
	void UpdateCameraTargetOffsetZ();

public:

	UE::Tasks::TTask<FVector> MultithreadTask;
	FVector GetWorldDirection(const FVector& CachedDirection) const;

	UE::Tasks::TTask<FHitResult> HitResultMultithreadTask;

private:

	FTimeline ResetCameraOffsetTimeline;
	FTimeline MoveCameraTimeline;
	void InitializeCamera();

	void OnWindowFocusChanged(bool bIsInFocus);
	bool bIsWindowFocused;

	float CurrentCameraMovementSpeed;
	bool bCameraSpeedTimelineFinished;
	bool bCameraSpeedTimelineActivated;
	
};
