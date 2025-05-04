// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GASCoursePlayerState.h"
#include "GameFramework/PlayerController.h"
#include "GASCourse/GASCourseCharacter.h"
#include "GASCoursePlayerController.generated.h"

/**
 * @brief A PlayerController class for the GAS Course.
 *
 * AGASCoursePlayerController is a custom PlayerController class that integrates various systems,
 * such as abilities and state trees, to manage player input, HUD creation, damage handling, and gameplay interactions.
 */
UCLASS(Config=Game)
class GASCOURSE_API AGASCoursePlayerController : public APlayerController
{
	GENERATED_BODY()

	/** State Tree for Player*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = StateTree, meta = (AllowPrivateAccess = "true"))
	class UStateTreeComponent* PlayerStateTreeComponent;

	void InitializeStateTree();


public:
	
	AGASCoursePlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void BeginPlayingState() override;

	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

	UFUNCTION(BlueprintCallable, Category = "GASCourse|PlayerController")
	AGASCoursePlayerState* GetGASCoursePlayerState() const;

	UFUNCTION(BlueprintCallable, Category = "GASCourse|PlayerController")
	UGASCourseAbilitySystemComponent* GetGASCourseAbilitySystemComponent() const;
	
	virtual void PreProcessInput(const float DeltaTime, const bool bGamePaused) override;
	/** Method called after processing input */
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;

	virtual void OnPossess(APawn* InPawn) override;

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GASCourse|HUD")
	void CreateHUD();

	void CreateHUD_Implementation();

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
	UPROPERTY(Transient)
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
	UPROPERTY(Transient)
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

	virtual bool InputKey(const FInputKeyParams& Params) override;

protected:

	virtual void OnRep_PlayerState() override;
	virtual void OnRep_Pawn() override;

	virtual void Tick(float DeltaSeconds) override;
	
	void OnDamageDealtCallback(const FGameplayEventData* Payload);

	UFUNCTION(BlueprintImplementableEvent)
	void OnDamageDealt(const FGameplayEventData& Payload);

	UFUNCTION()
	void GetMousePositionInViewport();
};
