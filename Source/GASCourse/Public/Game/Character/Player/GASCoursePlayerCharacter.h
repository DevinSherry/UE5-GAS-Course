// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Game/Input/GASCourseInputConfig.h"
#include "GASCourse/GASCourseCharacter.h"
#include "Components/TimelineComponent.h"
#include "Tasks/Task.h"
#include "InputAction.h"
#include "Game/Data/Camera/GASCoursePlayerCameraSettings.h"
#include "GASCoursePlayerCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInputBufferOpenedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInputBufferClosedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInputBufferFlushedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInputBufferConsumedDelegate, FGameplayTag, InputTag);

/**
 * Class representing the player character in the game.
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

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UGASCourseInputConfig> InputConfig;

public:

	AGASCoursePlayerCharacter(const FObjectInitializer& ObjectInitializer);

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION(BlueprintPure, Category="GASCourse|Input")
	UGASCourseInputConfig* GetInputConfig() const
	{
		return InputConfig;
	}
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GASCourse|Animation")
	TSubclassOf<UAnimInstance> UnArmedAnimLayer;

	void UpdateCharacterAnimLayer(TSubclassOf<UAnimInstance> NewAnimLayer) const;

public:
	//INPUT BUFFER SYSTEM
	
	/**
	 * OpenInputBuffer method is a BlueprintNativeEvent and BlueprintCallable function that is used to open the input buffer.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OpenInputBuffer();

	/**
	 * CloseInputBuffer method is a BlueprintNativeEvent and BlueprintCallable function that is used to close the input buffer.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void CloseInputBuffer();

	/**
	 * Flushes the input buffer by emptying the BufferedInputTags array.
	 * This method is responsible for clearing any buffered input tags.
	 *
	 * @return true if the input buffer was successfully flushed, false otherwise.
	 */
	UFUNCTION(BlueprintCallable)
	bool FlushInputBuffer();

	/** Activates the buffered input ability.
	 *
	 *  This method is responsible for activating the buffered input ability based on the first element in the BufferedInputTags array.
	 *  It checks for the existence of the AbilitySystemComponent and if the BufferedInputTags array is not empty before proceeding.
	 */
	UFUNCTION()
	void ActivateBufferedInputAbility();

	/**
	 * Delegate used for broadcasting events when the input buffer is opened.
	 */
	FInputBufferOpenedDelegate OnInputBufferOpenedEvent;
	/**
	 * Delegate used for broadcasting events when the input buffer is closed.
	 */
	FInputBufferClosedDelegate OnInputBufferClosedEvent;
	/**
	 * Delegate used for broadcasting events when the input buffer is flushed.
	 */
	FInputBufferFlushedDelegate OnInputBufferFlushedEvent;
	/**
	 * Delegate for broadcasting events when the input buffer is used.
	 * The OnInputBufferedUsedEvent delegate is used to notify listeners when input from the buffer is consumed.
	 */
	FInputBufferConsumedDelegate OnInputBufferedConsumedEvent;

	/**
	 * Returns whether the input buffer is currently open or closed.
	 *
	 * @return true if the input buffer is open, false if it is closed.
	 */
	bool IsInputBufferOpen() const
	{
		return bInputBufferOpen;
	}

	TArray<FGameplayTag> GetBufferedTags() const
	{
		return BufferedInputTags;
	}

private:
	/**
	 * Indicates whether the input buffer is currently open or closed.
	 * Setting this variable to true means the input buffer is open, allowing input to be buffered.
	 * Setting it to false means the input buffer is closed.
	 */
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bInputBufferOpen = false;

	/**
	 * Stores an array of Gameplay Tags that represent buffered input actions.
	 * These tags are used to store input actions to be processed later by the Input Buffer System.
	 * The system allows input actions to be recorded while the input buffer is open and then consumed when needed.
	 */
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<FGameplayTag> BufferedInputTags;
	

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
	void Look(const FInputActionValue& Value) override;

	void Input_CameraZoom(const FInputActionInstance& InputActionInstance);
	void Input_MoveCamera(const FInputActionInstance& InputActionInstance);
	void Input_MoveCameraCompleted(const FInputActionInstance& InputActionInstance);
	void UpdateCameraBoomTargetOffset(const FVector& InCameraBoomTargetOffset) const;
	void Input_RecenterCamera(const FInputActionInstance& InputActionInstance);
	void Input_RotateCameraAxis(const FInputActionValue& Value);
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
