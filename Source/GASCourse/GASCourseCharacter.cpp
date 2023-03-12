// Copyright Epic Games, Inc. All Rights Reserved.

#include "GASCourseCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Game/GameplayAbilitySystem/GASCourseGameplayAbilitySet.h"
#include "Game/GameplayAbilitySystem/GASCourseNativeGameplayTags.h"
#include "Game/Character/Components/GASCourseMovementComponent.h"


//////////////////////////////////////////////////////////////////////////
// AGASCourseCharacter

AGASCourseCharacter::AGASCourseCharacter(const class FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass<UGASCourseMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them

	//JumpZVelocity is overridden by the base attribute set value, JumpZVelocityOverride.
	GetCharacterMovement()->JumpZVelocity = 700.f;
	//MaxWalkSpeed is overridden by the base attribute set value, MovementSpeed.
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	//Initialize AbilitySystemComponent
	AbilitySystemComponent = CreateDefaultSubobject<UGASCourseAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
}

void AGASCourseCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
}

void AGASCourseCharacter::GrantDefaultAbilitySet() const
{
	if(GetLocalRole() != ROLE_Authority || !GetAbilitySystemComponent() || !DefaultAbilities)
	{
		return;
	}
	DefaultAbilities->GiveToAbilitySystem(GetAbilitySystemComponent(), nullptr);
	
}

UGASCourseAbilitySystemComponent* AGASCourseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

float AGASCourseCharacter::GetMovementSpeed() const
{
	if (const UGASCourseCharBaseAttributeSet* BaseAttributeSet = GetAbilitySystemComponent()->GetSetChecked<UGASCourseCharBaseAttributeSet>())
	{
		return BaseAttributeSet->GetMovementSpeed();
	}
	UE_LOG(LogTemp, Warning, TEXT("NO VALID ATTRIBUTE SET FOUND"));
	return 0.0f;
}

float AGASCourseCharacter::GetCrouchSpeed() const
{
	if (const UGASCourseCharBaseAttributeSet* BaseAttributeSet = GetAbilitySystemComponent()->GetSetChecked<UGASCourseCharBaseAttributeSet>())
	{
		return BaseAttributeSet->GetCrouchSpeed();
	}
	UE_LOG(LogTemp, Warning, TEXT("NO VALID ATTRIBUTE SET FOUND"));
	return 0.0f;
}

float AGASCourseCharacter::GetJumpZVelocityOverride() const
{
	if (const UGASCourseCharBaseAttributeSet* BaseAttributeSet = GetAbilitySystemComponent()->GetSetChecked<UGASCourseCharBaseAttributeSet>())
	{
		return BaseAttributeSet->GetJumpZVelocityOverride();
	}
	UE_LOG(LogTemp, Warning, TEXT("NO VALID ATTRIBUTE SET FOUND"));
	return 0.0f;
}

void AGASCourseCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	const FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		if (UGASCourseAbilitySystemComponent* GASCourseASC = GetAbilitySystemComponent())
		{
			//Block any type of movement if character has tag Status.MovementInputBlocked
			if(GASCourseASC->HasMatchingGameplayTag(FGASCourseNativeGameplayTags::Get().Status_BlockMovementInput))
			{
				return;
			}
			if(MovementVector.Length() > 0.0f)
			{
				GASCourseASC->SetLooseGameplayTagCount(FGASCourseNativeGameplayTags::Get().Status_IsMoving, 1);
			}
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

void AGASCourseCharacter::StopMove(const FInputActionValue& Value)
{
	// input is a Vector2D
	const FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		if (UGASCourseAbilitySystemComponent* GASCourseASC = GetAbilitySystemComponent())
		{
			//Block any type of movement if character has tag Status.MovementInputBlocked
			if(GASCourseASC->HasMatchingGameplayTag(FGASCourseNativeGameplayTags::Get().Status_BlockMovementInput))
			{
				return;
			}
			if(FMath::IsNearlyZero(MovementVector.Length()))
			{
				GASCourseASC->SetLooseGameplayTagCount(FGASCourseNativeGameplayTags::Get().Status_IsMoving, 0);
			}
		}
	}
}

void AGASCourseCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AGASCourseCharacter::Input_Crouch(const FInputActionValue& Value)
{
	const UGASCourseAbilitySystemComponent* GASCourseASC = GetAbilitySystemComponent();
	//Block any type of movement if character has tag Status.MovementInputBlocked
	if(GASCourseASC->HasMatchingGameplayTag(FGASCourseNativeGameplayTags::Get().Status_BlockMovementInput))
	{
		return;
	}
	const UCharacterMovementComponent* GASCharacterMovementComponent = CastChecked<UCharacterMovementComponent>(GetCharacterMovement());
	
	if (bIsCrouched || GASCharacterMovementComponent->bWantsToCrouch)
	{
		UnCrouch();
	}
	else if (GASCharacterMovementComponent->IsMovingOnGround())
	{
		Crouch();
	}
}

void AGASCourseCharacter::OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	UGASCourseAbilitySystemComponent* GASCourseASC = GetAbilitySystemComponent();
	//Block any type of movement if character has tag Status.MovementInputBlocked
	if(GASCourseASC->HasMatchingGameplayTag(FGASCourseNativeGameplayTags::Get().Status_BlockMovementInput))
	{
		return;
	}
	
	GASCourseASC->SetLooseGameplayTagCount(FGASCourseNativeGameplayTags::Get().Status_Crouching, 1);
	Super::OnStartCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
}

void AGASCourseCharacter::OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	UGASCourseAbilitySystemComponent* GASCourseASC = GetAbilitySystemComponent();
	//Block any type of movement if character has tag Status.MovementInputBlocked
	if(GASCourseASC->HasMatchingGameplayTag(FGASCourseNativeGameplayTags::Get().Status_BlockMovementInput))
	{
		return;
	}
	
	GASCourseASC->SetLooseGameplayTagCount(FGASCourseNativeGameplayTags::Get().Status_Crouching, 0);
	Super::OnEndCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
}

bool AGASCourseCharacter::CanJumpInternal_Implementation() const
{
	const UGASCourseAbilitySystemComponent* GASCourseASC = GetAbilitySystemComponent();
	if(GASCourseASC->HasMatchingGameplayTag(FGASCourseNativeGameplayTags::Get().Status_BlockMovementInput))
	{
		return false;
	}
	const UGASCourseMovementComponent* GASCharacterMovementComponent = CastChecked<UGASCourseMovementComponent>(GetCharacterMovement());
	if(GASCharacterMovementComponent->bAllowJumpFromCrouch)
	{
		return JumpIsAllowedInternal();
	}
	
	return Super::CanJumpInternal_Implementation();
}

void AGASCourseCharacter::Jump()
{
	const UGASCourseMovementComponent* GASCharacterMovementComponent = CastChecked<UGASCourseMovementComponent>(GetCharacterMovement());
	if(GASCharacterMovementComponent->bAllowJumpFromCrouch)
	{
		UnCrouch();
	}
	
	Super::Jump();
}




