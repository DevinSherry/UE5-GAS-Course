// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EnhancedInputComponent.h"
#include "GameplayTagContainer.h"
#include "GASC_InputBuffer_Settings.h"
#include "InputAction.h"
#include "Components/ActorComponent.h"
#include "Game/Character/Player/GASCoursePlayerController.h"
#include "GASC_InputBufferComponent.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LOG_GASC_InputBufferComponent, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInputBufferOpenedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInputBufferClosedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInputBufferFlushedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInputBufferConsumedDelegate, UInputAction*, InputAction);

/**
 * UGASC_InputBufferComponent is an actor component designed to manage an input buffer system.
 * This system allows input actions and gameplay tags to be captured, stored, and processed
 * at a later time based on specific conditions, such as the state of the input buffer.
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GASCOURSE_API UGASC_InputBufferComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGASC_InputBufferComponent();

	//INPUT BUFFER SYSTEM

	virtual void InitializeComponent() override;

	void LoadInputActionsFromSettings();
	
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

	UFUNCTION()
	void AddInputActionToBuffer(UInputAction* InAction);

	UFUNCTION()
	void ListenToInputActions();

	UFUNCTION()
	void SimulateInputAction(const UInputAction* InputAction) const;

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
	
	/**
	 * Retrieves an array of buffered input actions.
	 *
	 * This method returns the current list of input actions that have been buffered for processing.
	 *
	 * @return An array of UInputAction pointers representing the buffered input actions.
	 */
	TArray<UInputAction*> GetBufferedInputActions() const
	{
		return BufferedInputActions;
	}

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	/**
	 * Indicates whether the input buffer is currently open or closed.
	 * Setting this variable to true means the input buffer is open, allowing input to be buffered.
	 * Setting it to false means the input buffer is closed.
	 */
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bInputBufferOpen = false;

	/**
	 * BufferedInputActions is an array that holds references to UInputAction objects.
	 * This property is used to store input actions that are buffered for later use within the Input Buffer System.
	 * The input actions in this array are processed after being recorded, ensuring actions are performed at appropriate times.
	 * It is a BlueprintReadOnly property, meaning it can be accessed from Blueprints but cannot be modified directly.
	 */
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<UInputAction*> BufferedInputActions;

	UPROPERTY()
	TArray<UInputAction*> InputActionsToConsume;

	UPROPERTY()
	AGASCoursePlayerController* OwningPlayerController = nullptr;

	UPROPERTY()
	AGASCourseCharacter* OwningCharacter = nullptr;

	UPROPERTY()
	const UGASC_InputBuffer_Settings* InputBufferSettings = nullptr;

	UPROPERTY()
	UEnhancedInputComponent* EnhancedInputComponent = nullptr;
	
	TArray<const FEnhancedInputActionEventBinding*> Bindings;
};
