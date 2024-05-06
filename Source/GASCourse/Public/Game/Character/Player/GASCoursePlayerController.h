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

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

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

	UFUNCTION(BlueprintGetter)
	FORCEINLINE bool IsUsingGamepad() {return bUsingGamepad;}

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GASCourse|HUD")
	void CreateHUD();

	void CreateHUD_Implementation();

	/**
	 * @brief Structure representing the result of a hit test performed under the mouse cursor.
	 *
	 * This structure holds information about the hit result under the mouse cursor, including the location, normal, distance, and other properties of the hit.
	 * The hit result is typically the result of performing a collision check or a raycast from the cursor position.
	 *
	 * @note The structure is decorated with various UPROPERTY attributes to control replication, accessibility, and persistence in Blueprints.
	 *
	 * @see FHitResult
	 */
	UPROPERTY(Replicated, BlueprintReadOnly, Transient)
	FHitResult HitResultUnderMouseCursor;

	/**
	 * \brief Represents the direction of the mouse cursor projected onto the world space.
	 *
	 * This variable is used to store the direction of the mouse cursor after it has been deprojected onto the world space.
	 *
	 * \details The mouse direction is deprojected onto the world space to determine the direction in which the mouse cursor is pointing in the game world.
	 * This allows for accurate targeting and interaction between the player and the game environment.
	 *
	 * \note This variable is replicated to all clients to ensure consistency across the network.
	 * It is also marked as BlueprintReadOnly to prevent modification from outside the class.
	 * Finally, it is marked as Transient to indicate that it is not relevant for serialization.
	 *
	 * \see FVector
	 */
	UPROPERTY(Replicated, BlueprintReadOnly, Transient)
	FVector MouseDirectionDeprojectedToWorld;

	/**
	 * @brief The MousePositionDeprojectedToWorld variable represents the position of the mouse cursor in the world after deprojection.
	 *
	 * It is a 3D vector using the FVector class and is marked as Replicated, BlueprintReadOnly, and Transient.
	 * Replicated means the variable is replicated across networked instances.
	 * BlueprintReadOnly means the variable can only be read and not modified from blueprints.
	 * Transient means the variable is not serialized and does not need to be saved or restored.
	 *
	 * This variable is typically updated when the mouse cursor is moved and is used to determine the world position of the cursor.
	 *
	 * @see FVector
	 */
	UPROPERTY(Replicated, BlueprintReadOnly, Transient)
	FVector MousePositionDeprojectedToWorld;

	/**
	 * @brief The HitResultUnderMouseCursorObjectTypes variable is an array of object types used for performing object type queries in hit results.
	 *
	 * @details This variable should be set through the Unreal Editor's EditDefaultsOnly properties panel. It is exposed to Blueprint for read-only access.
	 * The array stores object types as TEnumAsByte<EObjectTypeQuery> values. These object types are used for filtering hit results based on the type of object hit.
	 * By specifying different object types, different types of objects can be included or excluded from the hit results.
	 *
	 * @see EObjectTypeQuery
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TEnumAsByte<EObjectTypeQuery>> HitResultUnderMouseCursorObjectTypes;

	UPROPERTY(Replicated, BlueprintReadOnly, Transient)
	FRotator CameraRotation;

	/**
	 * @brief Get the hit result under the mouse cursor.
	 *
	 * This function is used to get the hit result under the current position of the mouse cursor.
	 * It returns the hit result which stores information about the object hit by the mouse cursor.
	 */
	UFUNCTION(Client, Reliable, Category="GASCourse|PlayerController|Mouse")
	void GetHitResultUnderMouseCursor();
	
	UFUNCTION(Client, Reliable, Category="GASCourse|PlayerController|Mouse")
	void GetMousePositionInViewport();

	UFUNCTION(Server, Reliable, Category="GASCourse|PlayerController|Mouse")
	void UpdateHitResultOnServer(FHitResult InHitResult);
	
	UFUNCTION(Server, Reliable, Category="GASCourse|PlayerController|Mouse")
	void UpdateMousePositionInViewport(FVector InMousePosition, FVector InMouseDirection);
	
	UFUNCTION(Server, Reliable, Category= "GASCourse|PlayerController|Gamepad")
	void GetCameraRotation(FRotator InCameraRotation);

	UFUNCTION(Client, Reliable, Category= "GASCourse|PlayerController|Gamepad")
	void Client_GetCameraRotation();

	UFUNCTION(Server, Reliable, Category= "GASCourse|PlayerController|Gamepad")
	void GetIsUsingGamepad(bool bInUsingGamepad);

	UFUNCTION(Client, Reliable, Category= "GASCourse|PlayerController|Gamepad")
	void Client_GetIsUsingGamepad();

	UFUNCTION(BlueprintCallable, Reliable, Client)
	void StopMovement_Client();

	UFUNCTION(Reliable, Server)
	void StopMovement_Server();

	UFUNCTION(Reliable, NetMulticast)
	void StopMovement_Multicast();

	bool InputKey(const FInputKeyParams& Params) override;

protected:

	virtual void OnRep_PlayerState() override;
	virtual void OnRep_Pawn() override;

	virtual void Tick(float DeltaSeconds) override;
	
	void OnDamageDealtCallback(const FGameplayEventData* Payload);

	UFUNCTION(BlueprintImplementableEvent)
	void OnDamageDealt(const FGameplayEventData& Payload);

	UPROPERTY(Replicated, BlueprintReadOnly, Transient, BlueprintGetter=IsUsingGamepad)
	bool bUsingGamepad;

private:

	FVector3d CachedDestination = FVector3d(0.0f,0.0f,0.0f);
	
};
