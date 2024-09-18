// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Game/Input/GASCourseInputConfig.h"
#include "GASCourse/GASCourseCharacter.h"
#include "Components/TimelineComponent.h"
#include "Tasks/Task.h"
#include "InputAction.h"
#include "Game/Data/Camera/GASCoursePlayerCameraSettings.h"
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
	
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GASCourse|Camera Settings", meta = (AllowPrivateAccess = "true"))
	UGASCoursePlayerCameraSettings* CameraSettingsData;

public:

	AGASCoursePlayerCharacter(const FObjectInitializer& ObjectInitializer);

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

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

	void BindASCInput();

	void Move(const FInputActionValue& Value) override;

	void Input_CameraZoom(const FInputActionInstance& InputActionInstance);
	void Input_MoveCamera(const FInputActionInstance& InputActionInstance);
	void Input_MoveCameraCompleted(const FInputActionInstance& InputActionInstance);
	void UpdateCameraBoomTargetOffset(const FVector& InCameraBoomTargetOffset) const;
	void Input_RecenterCamera(const FInputActionInstance& InputActionInstance);
	void Input_RotateCameraAxis(const FInputActionInstance& InputActionInstance);
	void Input_RotateCameraCompleted(const FInputActionInstance& InputActionInstance);
	void Input_ToggleCameraMovement(const FInputActionInstance& InputActionInstance);
	void Input_ToggleCameraRotation(const FInputActionInstance& InputActionInstance);
	void Input_ToggleCameraRotation_Canceled(const FInputActionInstance& InputActionInstance);

	void MoveToMouseHitResultLocation();

	UFUNCTION()
	void RecenterCameraBoomTargetOffset();

	UFUNCTION()
	void RecenterCameraBoomTimelineFinished();

	UFUNCTION()
	void UpdateCameraMovementSpeed();

	UFUNCTION()
	void UpdateCameraMovementSpeedTimelineFinished();

	UFUNCTION()
	void UpdateCameraRotationSpeed();

	UFUNCTION()
	void UpdateCameraRotationSpeedTimelineFinished();
	
	UFUNCTION()
	void UpdateCameraEdgePanningSpeed();

	UFUNCTION()
	void UpdateCameraEdgePanningSpeedTimelineFinished();
	
	void CameraEdgePanning();

	UFUNCTION()
	void UpdateCameraTargetOffsetZ();

	UFUNCTION()
	float GetEdgePanningSpeedBasedOnZoomDistance() const;

	UFUNCTION()
	float GetCameraMovementSpeedBasedOnZoomDistance() const;

	UFUNCTION()
	void OnMovementUpdated(float DeltaSeconds, FVector OldLocation, FVector OldVelocity);
	
	void AutoAttachCameraWithinMinDistance();

public:

	UE::Tasks::TTask<FVector> MultithreadTask;
	FVector GetWorldDirection(const FVector& CachedDirection) const;

	UE::Tasks::TTask<FHitResult> HitResultMultithreadTask;

private:

	FTimeline ResetCameraOffsetTimeline;
	FTimeline MoveCameraTimeline;
	FTimeline RotateCameraTimeline;
	FTimeline CameraEdgePanningTimeline;
	
	void InitializeCamera();

	void OnWindowFocusChanged(bool bIsInFocus);
	bool bIsWindowFocused;

	float CurrentCameraMovementSpeed;
	bool bCameraSpeedTimelineFinished;
	bool bCameraSpeedTimelineActivated;

	float CurrentCameraRotationSpeed;
	bool bCameraRotationTimelineFinished;
	bool bCameraRotationTimelineActivated;

	float CurrentCameraEdgePanningSpeed;
	bool bCameraEdgePanningTimelineFinished;
	bool bCameraEdgePanningTimelineActivated;

	bool bHasPointClickMovementTarget = false;
	bool bCanMoveCamera = false;
	bool bCanRotateCamera = false;
	
};
