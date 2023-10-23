// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Character/Player/GASCoursePlayerCharacter.h"
#include "Game/Character/Player/GASCoursePlayerState.h"
#include "Game/Input/GASCourseEnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/TimelineComponent.h"
#include "Game/Character/Player/GASCoursePlayerController.h"
#include "Game/GameplayAbilitySystem/GASCourseNativeGameplayTags.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

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
	
	//Set camera boom arm length and socket offset Z to default max camera boom distance.
	CameraBoom->TargetArmLength = MaxCameraBoomDistance;
	CameraBoom->SocketOffset.Z = MaxCameraBoomDistance;
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
	GetCameraBoom()->TargetArmLength = MaxCameraBoomDistance/2.0f;
	GetCameraBoom()->SocketOffset = FVector(0.0f,0.0f, MaxCameraBoomDistance/2.0f);
}

void AGASCoursePlayerCharacter::OnWindowFocusChanged(bool bIsInFocus)
{
	bIsWindowFocused = bIsInFocus;
	SetMousePositionToScreenCenter();
}

void AGASCoursePlayerCharacter::UpdateCameraMovementSpeed()
{
	const float TimelineValue = MoveCameraTimeline.GetPlaybackPosition();
	const float CurveFloatValue = MoveCameraCurve->GetFloatValue(TimelineValue);
	CurrentCameraMovementSpeed = (FMath::FInterpTo(0.0f, GetCameraMovementSpeedBasedOnZoomDistance(), CurveFloatValue, MoveCameraInterpSpeed));
}

void AGASCoursePlayerCharacter::UpdateCameraMovementSpeedTimelineFinished()
{
	bCameraSpeedTimelineFinished = true;
}

void AGASCoursePlayerCharacter::UpdateCameraRotationSpeed()
{
	const float TimelineValue = RotateCameraTimeline.GetPlaybackPosition();
	const float CurveFloatValue =RotateCameraCurve->GetFloatValue(TimelineValue);

	CurrentCameraRotationSpeed = (FMath::FInterpTo(CurrentCameraRotationSpeed, CameraRotationSpeedMultiplier, CurveFloatValue, RotateCameraInterpSpeed));
}

void AGASCoursePlayerCharacter::UpdateCameraRotationSpeedTimelineFinished()
{
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
			EnhancedInputComponent->BindActionByTag(InputConfig, InputTag_MoveCamera,ETriggerEvent::Completed, this, &ThisClass::Input_MoveCameraCompleted);
			EnhancedInputComponent->BindActionByTag(InputConfig, InputTag_RecenterCamera, ETriggerEvent::Triggered, this, &ThisClass::Input_RecenterCamera);
			EnhancedInputComponent->BindActionByTag(InputConfig, InputTag_RotateCameraAxis, ETriggerEvent::Triggered, this, &ThisClass::Input_RotateCameraAxis);
			EnhancedInputComponent->BindActionByTag(InputConfig, InputTag_RotateCamera,ETriggerEvent::Completed, this, &ThisClass::Input_RotateCameraCompleted);

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
	OnCharacterMovementUpdated.AddDynamic(this, &ThisClass::OnMovementUpdated);
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
	OnCharacterMovementUpdated.AddDynamic(this, &ThisClass::OnMovementUpdated);
}

void AGASCoursePlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if(RecenterCameraCurve)
	{
		FOnTimelineFloat TimelineCallback;
		FOnTimelineEvent TimelineFinishedFunc;
		TimelineFinishedFunc.BindUFunction(this,FName("RecenterCameraBoomTimelineFinished"));
		ResetCameraOffsetTimeline.SetTimelineFinishedFunc(TimelineFinishedFunc);
		TimelineCallback.BindUFunction(this, FName("RecenterCameraBoomTargetOffset"));
		ResetCameraOffsetTimeline.AddInterpFloat(RecenterCameraCurve, TimelineCallback);
	}

	if(MoveCameraCurve)
	{
		FOnTimelineFloat TimelineCallback;
		FOnTimelineEvent TimelineFinishedFunc;
		TimelineFinishedFunc.BindUFunction(this, FName("UpdateCameraMovementSpeedTimelineFinished"));
		MoveCameraTimeline.SetTimelineFinishedFunc(TimelineFinishedFunc);
		TimelineCallback.BindUFunction(this, FName("UpdateCameraMovementSpeed"));
		MoveCameraTimeline.AddInterpFloat(MoveCameraCurve, TimelineCallback);
	}

	if(RotateCameraCurve)
	{
		FOnTimelineFloat TimelineCallback;
		FOnTimelineEvent TimelineFinishedFunc;
		TimelineFinishedFunc.BindUFunction(this, FName("UpdateCameraRotationSpeedTimelineFinished"));
		RotateCameraTimeline.SetTimelineFinishedFunc(TimelineFinishedFunc);
		TimelineCallback.BindUFunction(this, FName("UpdateCameraRotationSpeed"));
		RotateCameraTimeline.AddInterpFloat(RotateCameraCurve, TimelineCallback);
	}

	FSlateApplication::Get().OnApplicationActivationStateChanged().AddUObject(this, &ThisClass::OnWindowFocusChanged);
}

void AGASCoursePlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	ResetCameraOffsetTimeline.TickTimeline(DeltaSeconds);
	MoveCameraTimeline.TickTimeline(DeltaSeconds);
	RotateCameraTimeline.TickTimeline(DeltaSeconds);

	CameraEdgePanning();
}

void AGASCoursePlayerCharacter::Input_AbilityInputTagPressed(FGameplayTag InputTag)
{
	if(UGASCourseAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		if(ASC->HasMatchingGameplayTag(Status_Block_AbilityInput))
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
		if(ASC->HasMatchingGameplayTag(Status_Block_AbilityInput))
		{
			return;
		}
		ASC->AbilityInputTagReleased(InputTag);
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
			}
			// find out which way is forward
			const FRotator Rotation = GetCameraBoom()->GetRelativeRotation();
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

	if(CameraMovement.Length() > 0.0f)
	{
		UpdateCameraTargetOffsetZ();
	}

	if(!MoveCameraTimeline.IsPlaying() && !bCameraSpeedTimelineFinished)
	{
		if(GetCameraBoom()->IsAttachedTo(RootComponent))
		{
			FDetachmentTransformRules DetachmentRules = FDetachmentTransformRules::KeepWorldTransform;
			DetachmentRules.RotationRule = EDetachmentRule::KeepRelative;
			GetCameraBoom()->DetachFromComponent(DetachmentRules);
		}
		MoveCameraTimeline.PlayFromStart();
	}

	const FVector RotatedVector = GetCameraBoom()->GetRelativeRotation().RotateVector(FVector(CameraMovement.Y, CameraMovement.X, 0.0f));
	UpdateCameraBoomTargetOffset(RotatedVector);
}

void AGASCoursePlayerCharacter::Input_MoveCameraCompleted(const FInputActionInstance& InputActionInstance)
{
	MoveCameraTimeline.Stop();
	bCameraSpeedTimelineFinished = false;
	CurrentCameraMovementSpeed = 0.0f;
}

void AGASCoursePlayerCharacter::UpdateCameraBoomTargetOffset(const FVector& InCameraBoomTargetOffset) const
{
	const FVector NewTargetOffset = GetCameraBoom()->TargetOffset + (InCameraBoomTargetOffset.GetSafeNormal2D() * CurrentCameraMovementSpeed);
	GetCameraBoom()->TargetOffset = FVector(NewTargetOffset.X, NewTargetOffset.Y, GetCameraBoom()->TargetOffset.Z).GetClampedToSize(-CameraMaxVectorDistance, CameraMaxVectorDistance);
}

void AGASCoursePlayerCharacter::Input_RecenterCamera(const FInputActionInstance& InputActionInstance)
{
	ResetCameraOffsetTimeline.PlayFromStart();
}

void AGASCoursePlayerCharacter::Input_RotateCameraAxis(const FInputActionInstance& InputActionInstance)
{
	if(!RotateCameraTimeline.IsPlaying())
	{
		RotateCameraTimeline.PlayFromStart();
	}
	GetCameraBoom()->bEnableCameraRotationLag = false;
	const FVector2d CameraRotation = InputActionInstance.GetValue().Get<FVector2D>();
	const float CameraRotationX = CameraRotation.X * CurrentCameraRotationSpeed;
	const float CameraRotationY = CameraRotation.Y * CurrentCameraRotationSpeed;
	
	const FRotator NewCameraRelativeRotation = FRotator(FMath::ClampAngle((GetCameraBoom()->GetRelativeRotation().Pitch + CameraRotationX), MinCameraPitchAngle, MaxCameraPitchAngle),
		GetCameraBoom()->GetRelativeRotation().Yaw + CameraRotationY, 0.0f);
	
	GetCameraBoom()->SetRelativeRotation(NewCameraRelativeRotation);
}

void AGASCoursePlayerCharacter::Input_RotateCameraCompleted(const FInputActionInstance& InputActionInstance)
{
	RotateCameraTimeline.Stop();
	CurrentCameraRotationSpeed = 0.0f;
	GetCameraBoom()->bEnableCameraRotationLag = true;
	SetMousePositionToScreenCenter();
}

void AGASCoursePlayerCharacter::PointClickMovement(const FInputActionValue& Value)
{
	if(GetAbilitySystemComponent()->HasMatchingGameplayTag(Status_Block_PointClickMovementInput))
	{
		return;
	}
	MoveToMouseHitResultLocation();
}

void AGASCoursePlayerCharacter::PointClickMovementStarted(const FInputActionValue& Value)
{
	if(GetAbilitySystemComponent()->HasMatchingGameplayTag(Status_Block_PointClickMovementInput))
	{
		return;
	}
	if(AGASCoursePlayerController* PC = Cast<AGASCoursePlayerController>(Controller))
	{
		PC->StopMovement();
	}
}

void AGASCoursePlayerCharacter::PointClickMovementCompleted(const FInputActionInstance& InputActionInstance)
{
	if(GetAbilitySystemComponent()->HasMatchingGameplayTag(Status_Block_PointClickMovementInput))
	{
		return;
	}
	
	if(AGASCoursePlayerController* PC = Cast<AGASCoursePlayerController>(Controller))
	{
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(PC, PC->GetCachedDestination());
	}
}

void AGASCoursePlayerCharacter::MoveToMouseHitResultLocation()
{
	if(AGASCoursePlayerController* PC = Cast<AGASCoursePlayerController>(Controller))
	{
		SCOPED_NAMED_EVENT(AGASCourseCharacter_PointClickMovement, FColor::Blue);
		if(PC)
		{
			FHitResult HitResultUnderCursor;
			if(PC->GetHitResultUnderCursor(ECC_Visibility, true, HitResultUnderCursor))
			{
				PC->SetCachedDestination(HitResultUnderCursor.Location);
				MultithreadTask = UE::Tasks::Launch(UE_SOURCE_LOCATION, [this]
				{
					if(const AGASCoursePlayerController* InPC = Cast<AGASCoursePlayerController>(Controller))
					{
						return GetWorldDirection(InPC->GetCachedDestination());
					}
					return FVector::ZeroVector;
				});

				const FVector WorldDirection = MultithreadTask.GetResult();
				AddMovementInput(WorldDirection, 1.0f, false);
				
				if(MultithreadTask.IsCompleted())
				{
					MultithreadTask = {};
				}
			}
		}
	}
	MultithreadTask = {};
}

FVector AGASCoursePlayerCharacter::GetWorldDirection(const FVector& CachedDirection) const
{
	const FVector WorldDirection = UKismetMathLibrary::GetDirectionUnitVector(GetActorLocation(), CachedDirection);
	return WorldDirection;
}

void AGASCoursePlayerCharacter::RecenterCameraBoomTargetOffset()
{
	const float TimelineValue = ResetCameraOffsetTimeline.GetPlaybackPosition();
	const float CurveFloatValue = RecenterCameraCurve->GetFloatValue(TimelineValue);
	const FVector CurrentCameraTargetOffset = GetCameraBoom()->TargetOffset;
	const FVector CurrentCameraLocation = GetCameraBoom()->GetComponentLocation();
	
	GetCameraBoom()->TargetOffset = (FMath::VInterpTo(CurrentCameraTargetOffset, FVector(0.0f), CurveFloatValue, RecenterCameraInterpSpeed));
	GetCameraBoom()->SetWorldLocation(FMath::VInterpTo(CurrentCameraLocation, GetActorLocation(), CurveFloatValue, RecenterCameraInterpSpeed));
}

void AGASCoursePlayerCharacter::RecenterCameraBoomTimelineFinished()
{
	GetCameraBoom()->TargetOffset = FVector(0.0f);
	if(!GetCameraBoom()->IsAttachedTo(RootComponent))
	{
		FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules::KeepWorldTransform;
		AttachmentRules.RotationRule = EAttachmentRule::KeepRelative;
		AttachmentRules.LocationRule = EAttachmentRule::SnapToTarget;
		GetCameraBoom()->AttachToComponent(GetRootComponent(), AttachmentRules);
	}
}

void AGASCoursePlayerCharacter::CameraEdgePanning()
{
	SCOPED_NAMED_EVENT(AGASCourseCharacter_CameraEdgePanning, FColor::Red);
	bool bIsEnableRotateCameraAxis = false;
	bIsWindowFocused = true;

	//TODO: Move this check somewhere else?
	if(InputComponent)
	{
		if(EnableRotateCameraAxis)
		{
			if (UGASCourseEnhancedInputComponent* EnhancedInputComponent = CastChecked<UGASCourseEnhancedInputComponent>(InputComponent))
			{
				check(EnhancedInputComponent);
				const FEnhancedInputActionValueBinding* EnableRotateAxisBinding = &EnhancedInputComponent->BindActionValue(EnableRotateCameraAxis);
				bIsEnableRotateCameraAxis = EnableRotateAxisBinding->GetValue().Get<bool>();
			}
		}
	}

#if WITH_EDITOR
	const FViewport* EditorViewport = GEditor->GetPIEViewport();
	bIsWindowFocused = EditorViewport->HasMouseCapture();
#endif

	if(!GetWorld()->IsPlayInEditor())
	{
		const UGameViewportClient* GameViewport = GetWorld()->GetGameViewport();
		bIsWindowFocused = GameViewport->Viewport->HasMouseCapture();
	}
	
	const FVector2d MousePositionbyDPI = UWidgetLayoutLibrary::GetMousePositionOnViewport(this);
	const FVector2d ViewportScale2D = FVector2d(UWidgetLayoutLibrary::GetViewportScale(this));
	const FVector2d ViewportSize = UWidgetLayoutLibrary::GetViewportSize(this);
	const FVector2d MultipliedMousePosition = MousePositionbyDPI * ViewportScale2D;
		
	const float MappedNormalizedRangeX = UKismetMathLibrary::MapRangeClamped(UKismetMathLibrary::NormalizeToRange(MultipliedMousePosition.X, (ViewportSize.X * .01f), (ViewportSize.X * 0.99f)),
	0.0f, 1.0f, -1.0f, 1.0f);

	const float MappedNormalizedRangeY = UKismetMathLibrary::MapRangeClamped(UKismetMathLibrary::NormalizeToRange(MultipliedMousePosition.Y, (ViewportSize.Y * .01f), (ViewportSize.Y * 0.99f)),
	0.0f, 1.0f, 1.0f, -1.0f);
	
	if(FMath::Abs(MappedNormalizedRangeX) == 1 || FMath::Abs(MappedNormalizedRangeY) == 1)
	{
		if(!bIsEnableRotateCameraAxis && bIsWindowFocused)
		{
			const FVector OffsetDirection = GetCameraBoom()->GetRelativeRotation().RotateVector(FVector(MappedNormalizedRangeY, MappedNormalizedRangeX, 0.0f)).GetSafeNormal2D();
			const FVector NewTargetOffset = GetCameraBoom()->TargetOffset + (OffsetDirection * GetEdgePanningSpeedBasedOnZoomDistance());
			GetCameraBoom()->TargetOffset = NewTargetOffset.GetClampedToSize(-CameraMaxVectorDistance, CameraMaxVectorDistance);

			if(!MoveCameraTimeline.IsPlaying() && !bCameraSpeedTimelineFinished && !bCameraSpeedTimelineActivated)
			{
				MoveCameraTimeline.PlayFromStart();
				bCameraSpeedTimelineActivated = true;
			}

			if(GetCameraBoom()->IsAttachedTo(RootComponent))
			{
				FDetachmentTransformRules DetachmentRules = FDetachmentTransformRules::KeepWorldTransform;
				DetachmentRules.RotationRule = EDetachmentRule::KeepRelative;
				GetCameraBoom()->DetachFromComponent(DetachmentRules);
			}

			UpdateCameraTargetOffsetZ();
		}
	}
	else
	{
		if(bCameraSpeedTimelineActivated)
		{
			MoveCameraTimeline.Stop();
			bCameraSpeedTimelineFinished = false;
			bCameraSpeedTimelineActivated = false;
			CurrentCameraMovementSpeed = 0.0f;
		}

	}
}

void AGASCoursePlayerCharacter::SetMousePositionToScreenCenter()
{
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if(const ULocalPlayer* LP = PC->GetLocalPlayer())
		{
			if(UGameViewportClient* GVC = LP->ViewportClient)
			{
				FViewport* VP = GVC->Viewport;
				if(VP)
				{
					FVector2D ViewportSize;
					GVC->GetViewportSize(ViewportSize);
					const int32 X = static_cast<int32>(ViewportSize.X * 0.5f);
					const int32 Y = static_cast<int32>(ViewportSize.Y * 0.5f);

					VP->SetMouse(X, Y);
				}
			}
		}
	}
}

void AGASCoursePlayerCharacter::UpdateCameraTargetOffsetZ()
{
	if(const UWorld* World = GetWorld())
	{
		const FVector CameraBoomLocation = GetCameraBoom()->GetComponentLocation() + GetCameraBoom()->TargetOffset;
#if WITH_EDITOR
		DrawDebugSphere(GetWorld(), CameraBoomLocation, 15.f, 8, FColor::Blue);
		DrawDebugLine(GetWorld(), CameraBoomLocation, CameraBoomLocation + (GetActorUpVector() * 1000.0f), FColor::Red);
#endif

		if(GetCameraBoom()->IsAttachedTo(RootComponent))
		{
			return;
		}
		
		SCOPED_NAMED_EVENT(AGASCourseCharacter_UpdateCameraTargetOffsetZMultithread, FColor::Blue)
		HitResultMultithreadTask = UE::Tasks::Launch(UE_SOURCE_LOCATION, [this]
		{
			const UWorld* World = GetWorld();
			const FVector TraceStart = GetCameraBoom()->GetComponentLocation() + GetCameraBoom()->TargetOffset;
			const FVector TraceEnd = TraceStart + (GetActorUpVector() * CameraTargetOffsetZDownTraceLength);
			TArray<AActor*> ActorsToIgnore;
			ActorsToIgnore.Add(this);
			FHitResult OutHitResult;
			UKismetSystemLibrary::SphereTraceSingle(World, TraceStart, TraceEnd, CameraTargetOffsetZDownTraceRadius, UEngineTypes::ConvertToTraceType(ECC_Camera), true,
			ActorsToIgnore, EDrawDebugTrace::ForOneFrame, OutHitResult, true);
			return OutHitResult;
		});
		
		const FHitResult MultiThreadHitResult = HitResultMultithreadTask.GetResult();
		const float HitLocationZ = MultiThreadHitResult.ImpactPoint.Z;
		const float CharacterMeshLocationZ = GetMesh()->GetComponentLocation().Z;
		
		const float ZDifference = (HitLocationZ - CharacterMeshLocationZ) - GetCameraBoom()->TargetOffset.Z;
		GetCameraBoom()->TargetOffset.Z += ZDifference;
		
		if(HitResultMultithreadTask.IsCompleted())
		{
			HitResultMultithreadTask = {};
		}
	}
	HitResultMultithreadTask = {};
}

float AGASCoursePlayerCharacter::GetEdgePanningSpeedBasedOnZoomDistance() const
{
	return UKismetMathLibrary::MapRangeClamped(GetCameraBoom()->TargetArmLength, MinCameraBoomDistance, MaxCameraBoomDistance, EdgePanningSpeedMin, EdgePanningSpeedMax);
}

float AGASCoursePlayerCharacter::GetCameraMovementSpeedBasedOnZoomDistance() const
{
	return UKismetMathLibrary::MapRangeClamped(GetCameraBoom()->TargetArmLength, MinCameraBoomDistance, MaxCameraBoomDistance, CameraMovementSpeedMin, CameraMovementSpeedMax);
}

void AGASCoursePlayerCharacter::OnMovementUpdated(float DeltaSeconds, FVector OldLocation, FVector OldVelocity)
{
	const float MovementHeightDelta = GetActorLocation().Z - OldLocation.Z;
	const FVector CombinedCameraBoomLocation = GetCameraBoom()->TargetOffset + GetCameraBoom()->GetComponentLocation();
	if(GetCameraBoom()->IsAttachedTo(RootComponent))
	{
		return;
	}

	if((MovementHeightDelta > 0.0f && GetMesh()->GetComponentLocation().Z > CombinedCameraBoomLocation.Z) || (MovementHeightDelta < 0.0f && CombinedCameraBoomLocation.Z > GetMesh()->GetComponentLocation().Z))
	{
		GetCameraBoom()->TargetOffset.Z += MovementHeightDelta;
	}
}
