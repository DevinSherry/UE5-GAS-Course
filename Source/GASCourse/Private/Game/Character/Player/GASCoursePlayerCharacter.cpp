// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/Character/Player/GASCoursePlayerCharacter.h"
#include "Game/Character/Player/GASCoursePlayerState.h"
#include "Game/Input/GASCourseEnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Game/Character/Player/GASCoursePlayerController.h"
#include "Game/GameplayAbilitySystem/GASCourseNativeGameplayTags.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Game/Character/Components/InputBuffer/GASC_InputBufferComponent.h"

#if WITH_EDITOR
#include "Editor/EditorEngine.h"
#include "UnrealEd.h"
#endif

//////////////////////////////////////////////////////////////////////////
// AGASCourseCharacter

AGASCoursePlayerCharacter::AGASCoursePlayerCharacter(const FObjectInitializer& ObjectInitializer) :
Super(ObjectInitializer)
{
	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = false; // Rotate the arm based on the controller
	CameraBoom->bInheritPitch = false;
	CameraBoom->bInheritRoll = false;
	CameraBoom->bInheritYaw = false;
	CameraBoom->bDoCollisionTest = false;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	InputBufferComponent = CreateDefaultSubobject<UGASC_InputBufferComponent>(TEXT("InputBufferComponent"));
}

void AGASCoursePlayerCharacter::UpdateCharacterAnimLayer(TSubclassOf<UAnimInstance> NewAnimLayer) const
{
	if(NewAnimLayer)
	{
		GetMesh()->LinkAnimClassLayers(NewAnimLayer);
	}
}

void AGASCoursePlayerCharacter::InitializeCamera()
{
	if (CameraSettingsData)
	{
		GetCameraBoom()->TargetArmLength = CameraSettingsData->DefaultCameraBoomLength;
		GetCameraBoom()->SocketOffset = FVector(0.0f,0.0f, CameraSettingsData->DefaultSocketOffsetZ);
	}
}

bool AGASCoursePlayerCharacter::SimulateInputActionFromBuffer(FGameplayTag InputTag)
{
	if (!InputTag.IsValid())
	{
		return false;
	}
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (const UInputAction* BufferedAction = InputConfig->FindTaggedAbilityActionForTag(InputTag))
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
			{
				FInputActionValue Value(EInputActionValueType::Boolean, FVector(1.0f, 1.0f, 1.0f));
				const TArray<UInputModifier*>& Modifiers = {};
				const TArray<UInputTrigger*>& Triggers = {};
				Subsystem->InjectInputForAction(BufferedAction, Value, Modifiers, Triggers);
			}

		}
	}
	
	return Super::SimulateInputActionFromBuffer(InputTag);
}

//////////////////////////////////////////////////////////////////////////
// Input
void AGASCoursePlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings
	if (UGASCourseEnhancedInputComponent* EnhancedInputComponent = CastChecked<UGASCourseEnhancedInputComponent>(PlayerInputComponent))
	{
		check(EnhancedInputComponent);
		
		if(InputConfig)
		{
			check(InputConfig);
			//Jumping - TODO: Remove this
			EnhancedInputComponent->BindActionByTag(InputConfig, InputTag_Jump, ETriggerEvent::Triggered, this, &ThisClass::Jump);
			EnhancedInputComponent->BindActionByTag(InputConfig, InputTag_Jump, ETriggerEvent::Completed, this, &ThisClass::StopJumping);

			//Moving - TODO: Remove this
			EnhancedInputComponent->BindActionByTag(InputConfig, InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Move);
			EnhancedInputComponent->BindActionByTag(InputConfig, InputTag_Move, ETriggerEvent::Completed, this, &ThisClass::StopMove);

			//Looking
			EnhancedInputComponent->BindActionByTag(InputConfig, InputTag_Look_Stick, ETriggerEvent::Triggered, this, &ThisClass::Look);
			
			EnhancedInputComponent->BindActionByTag(InputConfig, InputTag_RotateCameraAxis, ETriggerEvent::Triggered, this, &ThisClass::Input_RotateCameraAxis);
			
			//Crouching
			EnhancedInputComponent->BindActionByTag(InputConfig, InputTag_Crouch, ETriggerEvent::Triggered, this, &ThisClass::Input_Crouch);

			//Camera Zoom
			EnhancedInputComponent->BindActionByTag(InputConfig, InputTag_CameraZoom,ETriggerEvent::Triggered, this, &ThisClass::Input_CameraZoom);

			TArray<uint32> BindHandles;
			EnhancedInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::Input_AbilityInputTagPressed, &ThisClass::Input_AbilityInputTagReleased, /*out*/ BindHandles);

			BindASCInput();
		}
	}
}

void AGASCoursePlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if(AGASCoursePlayerState* PS = GetPlayerState<AGASCoursePlayerState>())
	{
		AbilitySystemComponent = Cast<UGASCourseAbilitySystemComponent>(PS->GetAbilitySystemComponent());
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, this);
		InitializeAbilitySystem(AbilitySystemComponent);
	}

	if (AGASCoursePlayerController* PlayerController = Cast<AGASCoursePlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContextKBM, 0);
			Subsystem->AddMappingContext(DefaultMappingContextGamepad, 0);
		}
		PlayerController->CreateHUD();
	}
	
	UpdateCharacterAnimLayer(UnArmedAnimLayer);
	InitializeCamera();
}

void AGASCoursePlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	if(AGASCoursePlayerState* PS = GetPlayerState<AGASCoursePlayerState>())
	{
		AbilitySystemComponent = Cast<UGASCourseAbilitySystemComponent>(PS->GetAbilitySystemComponent());
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, this);
		
		if (AGASCoursePlayerController* PlayerController = Cast<AGASCoursePlayerController>(Controller))
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				Subsystem->AddMappingContext(DefaultMappingContextKBM, 0);
				Subsystem->AddMappingContext(DefaultMappingContextGamepad, 0);
			}
			
			PlayerController->CreateHUD();
			BindASCInput();
		}
		
		UpdateCharacterAnimLayer(UnArmedAnimLayer);
		InitializeCamera();
	}
	
	AbilitySystemComponent->RefreshAbilityActorInfo();

}

void AGASCoursePlayerCharacter::OnRep_Controller()
{
	Super::OnRep_Controller();
	if(AbilitySystemComponent)
	{
		AbilitySystemComponent->RefreshAbilityActorInfo();
	}
}

void AGASCoursePlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AGASCoursePlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AGASCoursePlayerCharacter::Input_AbilityInputTagPressed(FGameplayTag InputTag)
{
	if(UGASCourseAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		if(ASC->HasMatchingGameplayTag(Status_Block_AbilityInput))
		{
			UE_LOG(LogTemp, Warning, TEXT(""));
			return;
		}
		if(!InputTag.IsValid())
		{
			return;
		}
		//If input buffer is open, do not process input
		if(!InputBufferComponent->IsInputBufferOpen())
		{
			ASC->AbilityInputTagPressed(InputTag);
		}
	}
}

void AGASCoursePlayerCharacter::Input_AbilityInputTagReleased(FGameplayTag InputTag)
{
	if(UGASCourseAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		if(ASC->HasMatchingGameplayTag(Status_Block_AbilityInput))
		{
			return;
		}
		ASC->AbilityInputTagReleased(InputTag);
	}
}

void AGASCoursePlayerCharacter::BindASCInput()
{
	if (IsValid(InputComponent))
	{
		if(UGASCourseAbilitySystemComponent* MyASC = CastChecked<UGASCourseAbilitySystemComponent>( GetAbilitySystemComponent()))
		{
			// Set up action bindings
			if (UGASCourseEnhancedInputComponent* EnhancedInputComponent = CastChecked<UGASCourseEnhancedInputComponent>(InputComponent))
			{
				check(EnhancedInputComponent);
				EnhancedInputComponent->BindActionByTag(InputConfig, InputTag_ConfirmTargetData, ETriggerEvent::Triggered, MyASC, &UGASCourseAbilitySystemComponent::LocalInputConfirm);
				EnhancedInputComponent->BindActionByTag(InputConfig, InputTag_CancelTargetData, ETriggerEvent::Triggered, MyASC, &UGASCourseAbilitySystemComponent::LocalInputCancel);
			}
		}
	}
}

void AGASCoursePlayerCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	const FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		if (UGASCourseAbilitySystemComponent* GASCourseASC = GetAbilitySystemComponent())
		{
			//Block any type of movement if character has tag Status.MovementInputBlocked
			if(GASCourseASC->HasMatchingGameplayTag(Status_Block_MovementInput))
			{
				return;
			}
			if(MovementVector.Length() > 0.0f)
			{
				GASCourseASC->SetLooseGameplayTagCount(Status_IsMoving, 1);
				// find out which way is forward
				const FRotator Rotation = Controller->GetControlRotation();
				const FRotator YawRotation(0, Rotation.Yaw, 0);

				// get forward vector
				const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
				// get right vector 
				const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

				// add movement 
				AddMovementInput(ForwardDirection, MovementVector.Y);
				AddMovementInput(RightDirection, MovementVector.X);
				
			}
		}
	}
}

void AGASCoursePlayerCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
	Input_RotateCameraAxis(Value);
}

void AGASCoursePlayerCharacter::Input_RotateCameraAxis(const FInputActionValue& Value)
{
	if(GetAbilitySystemComponent()->HasMatchingGameplayTag(Status_Block_RotationInput))
	{
		return;
	}
	if(AGASCoursePlayerController* PlayerControllerController = Cast<AGASCoursePlayerController>(Controller))
	{
		
		GetCameraBoom()->bEnableCameraRotationLag = false;
		const FVector2d CameraRotation = Value.Get<FVector2D>();
		const float CameraRotationX = CameraRotation.X; //* CurrentCameraRotationSpeed;
		const float CameraRotationY = CameraRotation.Y; //* CurrentCameraRotationSpeed;
		
		const FRotator NewCameraControlRotation = FRotator(FMath::ClampAngle((CameraBoom->GetComponentRotation().Pitch - CameraRotationY),
			CameraSettingsData->MinCameraPitchAngle, CameraSettingsData->MaxCameraPitchAngle),CameraBoom->GetComponentRotation().Yaw + CameraRotationX, 0.0f);

		CameraBoom->SetWorldRotation(NewCameraControlRotation);
	}
}

void AGASCoursePlayerCharacter::Input_CameraZoom(const FInputActionInstance& InputActionInstance)
{
	const float AxisValue = InputActionInstance.GetValue().Get<float>();
	
	if(USpringArmComponent* CameraRef = GetCameraBoom())
	{
		const float Step = CameraSettingsData->CameraZoomDistanceStep * AxisValue;
		const float CurrentTargetArmLength = FMath::Clamp((CameraRef->TargetArmLength - Step),
		CameraSettingsData->MinCameraBoomDistance, CameraSettingsData->MaxCameraBoomDistance);
					
		CameraRef->TargetArmLength = CurrentTargetArmLength;
	}
}