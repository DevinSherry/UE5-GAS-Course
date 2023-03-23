// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Character/Player/GASCoursePlayerCharacter.h"
#include "Game/Character/Player/GASCoursePlayerState.h"
#include "Game/Input/GASCourseEnhancedInputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Game/Character/Components/GASCourseChampionComponent.h"
#include "Game/GameplayAbilitySystem/GASCourseNativeGameplayTags.h"

//////////////////////////////////////////////////////////////////////////
// AGASCourseCharacter

AGASCoursePlayerCharacter::AGASCoursePlayerCharacter(const FObjectInitializer& ObjectInitializer) :
Super(ObjectInitializer)
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
			//Jumping
			EnhancedInputComponent->BindActionByTag(InputConfig, InputTag_Jump, ETriggerEvent::Triggered, this, &ThisClass::Jump);
			EnhancedInputComponent->BindActionByTag(InputConfig, InputTag_Jump, ETriggerEvent::Completed, this, &ThisClass::StopJumping);

			//Moving
			EnhancedInputComponent->BindActionByTag(InputConfig, InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Move);
			EnhancedInputComponent->BindActionByTag(InputConfig, InputTag_Move, ETriggerEvent::Completed, this, &ThisClass::StopMove);

			//Looking
			EnhancedInputComponent->BindActionByTag(InputConfig, InputTag_Look_Mouse, ETriggerEvent::Triggered, this, &ThisClass::Look);
			EnhancedInputComponent->BindActionByTag(InputConfig, InputTag_Look_Stick, ETriggerEvent::Triggered, this, &ThisClass::Look);

			//Crouching
			EnhancedInputComponent->BindActionByTag(InputConfig, InputTag_Crouch, ETriggerEvent::Triggered, this, &ThisClass::Input_Crouch);

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
		//GetChampionComponent()->InitializeChampionComponent(AbilitySystemComponent);

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
	}
}

void AGASCoursePlayerCharacter::OnRep_Controller()
{
	Super::OnRep_Controller();
	// Needed in case the PC wasn't valid when we Init-ed the ASC.
	if (const AGASCoursePlayerState* PS = GetPlayerState<AGASCoursePlayerState>())
	{
		PS->GetAbilitySystemComponent()->RefreshAbilityActorInfo();
	}
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
