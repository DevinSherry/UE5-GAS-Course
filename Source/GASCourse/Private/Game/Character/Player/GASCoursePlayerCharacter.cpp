// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Character/Player/GASCoursePlayerCharacter.h"
#include "Game/Character/Player/GASCoursePlayerState.h"
#include "Game/Input/GASCourseEnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/TimelineComponent.h"
#include "Game/GameplayAbilitySystem/GASCourseNativeGameplayTags.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"

//////////////////////////////////////////////////////////////////////////
// AGASCourseCharacter

AGASCoursePlayerCharacter::AGASCoursePlayerCharacter(const FObjectInitializer& ObjectInitializer) :
Super(ObjectInitializer)
{
	//Set camera boom arm length and socket offset Z to default max camera boom distance.
	GetCameraBoom()->TargetArmLength = MaxCameraBoomDistance;
	GetCameraBoom()->SocketOffset.Z = MaxCameraBoomDistance;
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
	GetCameraBoom()->TargetArmLength = MaxCameraBoomDistance;
	GetCameraBoom()->SocketOffset = FVector(0.0f,0.0f, MaxCameraBoomDistance);
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
			
			EnhancedInputComponent->BindActionByTag(InputConfig, InputTag_PointClickMovement, ETriggerEvent::Triggered, this, &ThisClass::PointClickMovement);
			EnhancedInputComponent->BindActionByTag(InputConfig, InputTag_PointClickMovement, ETriggerEvent::Started, this, &ThisClass::PointClickMovementStarted);
			EnhancedInputComponent->BindActionByTag(InputConfig, InputTag_PointClickMovement, ETriggerEvent::Canceled, this, &ThisClass::PointClickMovementCompleted);
			EnhancedInputComponent->BindActionByTag(InputConfig, InputTag_PointClickMovement, ETriggerEvent::Completed, this, &ThisClass::PointClickMovementCompleted);

			if(UGASCourseAbilitySystemComponent* MyASC = CastChecked<UGASCourseAbilitySystemComponent>( GetAbilitySystemComponent()))
			{
				EnhancedInputComponent->BindActionByTag(InputConfig, InputTag_ConfirmTargetData, ETriggerEvent::Triggered, MyASC, &UGASCourseAbilitySystemComponent::LocalInputConfirm);
				EnhancedInputComponent->BindActionByTag(InputConfig, InputTag_CancelTargetData, ETriggerEvent::Triggered, MyASC, &UGASCourseAbilitySystemComponent::LocalInputCancel);
			}

			//Looking
			EnhancedInputComponent->BindActionByTag(InputConfig, InputTag_Look_Stick, ETriggerEvent::Triggered, this, &ThisClass::Look);

			//Camera Controls
			EnhancedInputComponent->BindActionByTag(InputConfig, InputTag_MoveCamera, ETriggerEvent::Triggered, this, &ThisClass::Input_MoveCamera);
			EnhancedInputComponent->BindActionByTag(InputConfig, InputTag_RecenterCamera, ETriggerEvent::Triggered, this, &ThisClass::Input_RecenterCamera);
			EnhancedInputComponent->BindActionByTag(InputConfig, InputTag_RotateCamera, ETriggerEvent::Triggered, this , &ThisClass::Input_RotateCamera);

			//Crouching
			EnhancedInputComponent->BindActionByTag(InputConfig, InputTag_Crouch, ETriggerEvent::Triggered, this, &ThisClass::Input_Crouch);

			//Camera Zoom
			EnhancedInputComponent->BindActionByTag(InputConfig, InputTag_CameraZoom,ETriggerEvent::Triggered, this, &ThisClass::Input_CameraZoom);

			TArray<uint32> BindHandles;
			EnhancedInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::Input_AbilityInputTagPressed, &ThisClass::Input_AbilityInputTagReleased, /*out*/ BindHandles);
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

		if (const APlayerController* PlayerController = Cast<APlayerController>(Controller))
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				Subsystem->AddMappingContext(DefaultMappingContextKBM, 0);
				Subsystem->AddMappingContext(DefaultMappingContextGamepad, 0);
			}
		}
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
		InitializeAbilitySystem(AbilitySystemComponent);

		if (const APlayerController* PlayerController = Cast<APlayerController>(Controller))
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				Subsystem->AddMappingContext(DefaultMappingContextKBM, 0);
				Subsystem->AddMappingContext(DefaultMappingContextGamepad, 0);
			}
		}

		UpdateCharacterAnimLayer(UnArmedAnimLayer);
		InitializeCamera();
	}
}

void AGASCoursePlayerCharacter::OnRep_Controller()
{
	Super::OnRep_Controller();

	UpdateCharacterAnimLayer(UnArmedAnimLayer);
	
	// Needed in case the PC wasn't valid when we Init-ed the ASC.
	if (const AGASCoursePlayerState* PS = GetPlayerState<AGASCoursePlayerState>())
	{
		PS->GetAbilitySystemComponent()->RefreshAbilityActorInfo();
	}
}

void AGASCoursePlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if(RecenterCameraCurve)
	{
		FOnTimelineFloat TimelineCallback;
		TimelineCallback.BindUFunction(this, FName("RecenterCameraBoomTargetOffset"));
		ResetCameraOffsetTimeline.AddInterpFloat(RecenterCameraCurve, TimelineCallback);
	}
}

void AGASCoursePlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	ResetCameraOffsetTimeline.TickTimeline(DeltaSeconds);
}

void AGASCoursePlayerCharacter::Input_AbilityInputTagPressed(FGameplayTag InputTag)
{
	if(UGASCourseAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		if(ASC->HasMatchingGameplayTag(Status_AbilityInputBlocked))
		{
			return;
		}
		ASC->AbilityInputTagPressed(InputTag);
	}
}

void AGASCoursePlayerCharacter::Input_AbilityInputTagReleased(FGameplayTag InputTag)
{
	if(UGASCourseAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		if(ASC->HasMatchingGameplayTag(Status_AbilityInputBlocked))
		{
			return;
		}
		ASC->AbilityInputTagReleased(InputTag);
	}
}

void AGASCoursePlayerCharacter::Move(const FInputActionValue& Value)
{
	if(ResetCameraOffsetTimeline.IsPlaying())
	{
		return;
	}
	Super::Move(Value);
	if(GetCameraBoom()->IsAttachedTo(RootComponent))
	{
		return;
	}

	const FVector2d Movement = Value.Get<FVector2d>();
	if(Movement.Length() <= 0)
	{
		return;
	}
	const FInputActionInstance InputActionInstance;
	Input_RecenterCamera(InputActionInstance);
}

void AGASCoursePlayerCharacter::Input_CameraZoom(const FInputActionInstance& InputActionInstance)
{
	const float AxisValue = InputActionInstance.GetValue().Get<float>();
	
	if(USpringArmComponent* CameraRef = GetCameraBoom())
	{
		const float Step = CameraZoomDistanceStep * AxisValue;
		const float CurrentTargetArmLength = FMath::Clamp((CameraRef->TargetArmLength - Step),
		MinCameraBoomDistance, MaxCameraBoomDistance);
					
		CameraRef->TargetArmLength = CurrentTargetArmLength;
		CameraRef->SocketOffset.Z = CurrentTargetArmLength;
	}
}

void AGASCoursePlayerCharacter::Input_MoveCamera(const FInputActionInstance& InputActionInstance)
{
	const FVector2d CameraMovement =  InputActionInstance.GetValue().Get<FVector2D>();
	if(CameraMovement.Length() >= 0.35f && ResetCameraOffsetTimeline.IsPlaying())
	{
		ResetCameraOffsetTimeline.Stop();
	}

	if(GetCameraBoom()->IsAttachedTo(RootComponent))
	{
		FDetachmentTransformRules& DetachmentTransformRules = FDetachmentTransformRules::KeepWorldTransform;
		DetachmentTransformRules.LocationRule = EDetachmentRule::KeepWorld;
		DetachmentTransformRules.RotationRule = EDetachmentRule::KeepRelative;
		DetachmentTransformRules.bCallModify = true;
		GetCameraBoom()->DetachFromComponent(DetachmentTransformRules);
	}
	
	const FVector CameraForward = UKismetMathLibrary::GetForwardVector(GetCameraBoom()->GetRelativeRotation()) * CameraMovement.Y;
	const FVector CameraRight = UKismetMathLibrary::GetRightVector(GetCameraBoom()->GetRelativeRotation()) * CameraMovement.X;
	UpdateCameraBoomTargetOffset(CameraForward + CameraRight);
}

void AGASCoursePlayerCharacter::UpdateCameraBoomTargetOffset(const FVector& InCameraBoomTargetOffset) const
{
	const FVector NewTargetOffset = GetCameraBoom()->TargetOffset + (InCameraBoomTargetOffset.GetSafeNormal() * CameraMovementSpeed);
	GetCameraBoom()->TargetOffset = NewTargetOffset.GetClampedToSize(-CameraMaxVectorDistance, CameraMaxVectorDistance);
}

void AGASCoursePlayerCharacter::Input_RecenterCamera(const FInputActionInstance& InputActionInstance)
{
	if(!GetCameraBoom()->IsAttachedTo(RootComponent))
	{
		FAttachmentTransformRules& AttachmentRules = FAttachmentTransformRules::KeepWorldTransform;
		AttachmentRules.LocationRule = EAttachmentRule::KeepWorld;
		AttachmentRules.RotationRule = EAttachmentRule::KeepRelative;
		GetCameraBoom()->AttachToComponent(RootComponent, AttachmentRules);
	}
	ResetCameraOffsetTimeline.PlayFromStart();
}

void AGASCoursePlayerCharacter::Input_RotateCamera(const FInputActionInstance& InputActionInstance)
{
	const FVector2d CameraRotation =  InputActionInstance.GetValue().Get<FVector2D>();
	const FRotator NewCameraRelativeRotation = FRotator(FMath::ClampAngle((GetCameraBoom()->GetRelativeRotation().Pitch + CameraRotation.X), MinCameraPitchAngle, MaxCameraPitchAngle),
		GetCameraBoom()->GetRelativeRotation().Yaw + CameraRotation.Y, 0.0f);
	
	GetCameraBoom()->SetRelativeRotation(NewCameraRelativeRotation);
}

void AGASCoursePlayerCharacter::RecenterCameraBoomTargetOffset()
{
	const float TimelineValue = ResetCameraOffsetTimeline.GetPlaybackPosition();
	const float CurveFloatValue = RecenterCameraCurve->GetFloatValue(TimelineValue);
	const FVector CurrentCameraTargetOffset = GetCameraBoom()->TargetOffset;
	
	GetCameraBoom()->TargetOffset = (FMath::VInterpTo(CurrentCameraTargetOffset, FVector(0.0f), CurveFloatValue, RecenterCameraInterpSpeed));
}
