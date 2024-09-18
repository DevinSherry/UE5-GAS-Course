// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Character/Player/GASCoursePlayerController.h"
#include "Net/UnrealNetwork.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Game/GameplayAbilitySystem/GASCourseNativeGameplayTags.h"
#include "GameFramework/PlayerInput.h"
#include "CogCommon.h"
#include "Debug/Cog/CogSampleDefines.h"
#include "Debug/Cog/CogSampleLogCategories.h"
#include "GASCourse/GASCourseCharacter.h"

#if ENABLE_COG
#include "CogAbilityReplicator.h"
#include "CogDebugReplicator.h"
#include "CogEngineReplicator.h"
#include "Framework/Application/SlateApplication.h"
#endif //ENABLE_COG

AGASCoursePlayerController::AGASCoursePlayerController(const FObjectInitializer& ObjectInitializer)
{
	bEnableMouseOverEvents = true;
}

void AGASCoursePlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AGASCoursePlayerController, HitResultUnderMouseCursor);
	DOREPLIFETIME(AGASCoursePlayerController, MouseDirectionDeprojectedToWorld);
	DOREPLIFETIME(AGASCoursePlayerController, MousePositionDeprojectedToWorld);
	DOREPLIFETIME(AGASCoursePlayerController, CameraRotation);
	DOREPLIFETIME(AGASCoursePlayerController, bUsingGamepad);
}

void AGASCoursePlayerController::BeginPlayingState()
{
	Super::BeginPlayingState();
}

void AGASCoursePlayerController::BeginPlay()
{
	Super::BeginPlay();

#if ENABLE_COG
	// Spawn the Replicator of each plugin
	ACogDebugReplicator::Spawn(this);
	ACogAbilityReplicator::Spawn(this);
	ACogEngineReplicator::Spawn(this);
#endif //ENABLE_COG
}

void AGASCoursePlayerController::SetPossession(APawn* NewPawn)
{
	    if (NewPawn == nullptr || GetPawn() == NewPawn)
    {
        return;
    }

    //-------------------------------------------------------------------------------------------
    // Unplug the current controller so it doesn't conflict with the newly assigned controller
    //-------------------------------------------------------------------------------------------
    AController* OldController = NewPawn->GetController();
    if (OldController != nullptr)
    {
        COG_LOG_OBJECT(LogCogPossession, ELogVerbosity::Verbose, this, TEXT("%s unpossess %s"), *GetNameSafe(OldController), *GetNameSafe(NewPawn));
        OldController->UnPossess();
    }

    //-------------------------------------------------------------------------------------------
    // We will need to replug the initial controller of the character we currently control
    //-------------------------------------------------------------------------------------------
    AGASCourseCharacter* OldCharacter = Cast<AGASCourseCharacter>(GetPawn());

    //-------------------------------------------------------------------------------------------
    // Unpossess before possession to prevent a warning
    //-------------------------------------------------------------------------------------------
    COG_LOG_OBJECT(LogCogPossession, ELogVerbosity::Verbose, this, TEXT("%s unpossess %s"), *GetNameSafe(this), *GetNameSafe(GetPawn()));
    UnPossess();

    COG_LOG_OBJECT(LogCogPossession, ELogVerbosity::Verbose, this, TEXT("%s possess %s"), *GetNameSafe(this), *GetNameSafe(NewPawn));
    Possess(NewPawn);

    //-------------------------------------------------------------------------------------------
    // Replug the initial controller on the old character. For example, replug the initial
    // AI controller of an AI when the player finishes controlling it. This needs to be done
    // after the Possess call.
    //-------------------------------------------------------------------------------------------
    if (OldCharacter != nullptr && OldCharacter->InitialController != nullptr && OldCharacter->InitialController != this)
    {
        COG_LOG_OBJECT(LogCogPossession, ELogVerbosity::Verbose, this, TEXT("%s possess %s"), *GetNameSafe(OldCharacter->InitialController), *GetNameSafe(OldCharacter));
        OldCharacter->InitialController->Possess(OldCharacter);
    }
}

void AGASCoursePlayerController::ResetPossession()
{
	SetPossession(InitialPossessedCharacter.Get());
}

AGASCoursePlayerState* AGASCoursePlayerController::GetGASCoursePlayerState() const
{
	return CastChecked<AGASCoursePlayerState>(PlayerState, ECastCheckedType::NullAllowed);
}

UGASCourseAbilitySystemComponent* AGASCoursePlayerController::GetGASCourseAbilitySystemComponent() const
{
	const AGASCoursePlayerState* PS = GetGASCoursePlayerState();
	return (PS ? PS->GetAbilitySystemComponent() : nullptr);
}

void AGASCoursePlayerController::PreProcessInput(const float DeltaTime, const bool bGamePaused)
{
	Super::PreProcessInput(DeltaTime, bGamePaused);
}

void AGASCoursePlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	if (UGASCourseAbilitySystemComponent* ASC = GetGASCourseAbilitySystemComponent())
	{
		ASC->ProcessAbilityInput(DeltaTime, bGamePaused);
	}
	
	Super::PostProcessInput(DeltaTime, bGamePaused);
}

void AGASCoursePlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AGASCoursePlayerState* PS = GetPlayerState<AGASCoursePlayerState>();
	if (PS)
	{
		// Init ASC with PS (Owner) and our new Pawn (AvatarActor)
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, InPawn);
	}

	if(UGASCourseAbilitySystemComponent* ASC = GetGASCourseAbilitySystemComponent())
	{
		ASC->GenericGameplayEventCallbacks.FindOrAdd(Event_Gameplay_OnDamageDealt).AddUObject(this, &AGASCoursePlayerController::OnDamageDealtCallback);
	}
}

void AGASCoursePlayerController::CreateHUD_Implementation()
{

}

void AGASCoursePlayerController::GetHitResultUnderMouseCursor_Implementation()
{
	GetHitResultUnderCursorForObjects(HitResultUnderMouseCursorObjectTypes, true, HitResultUnderMouseCursor);
	if(GetLocalRole() != ROLE_Authority)
	{
		UpdateHitResultOnServer(HitResultUnderMouseCursor);
	}
}

void AGASCoursePlayerController::UpdateHitResultOnServer_Implementation(FHitResult InHitResult)
{
	HitResultUnderMouseCursor = InHitResult;
}

void AGASCoursePlayerController::GetMousePositionInViewport_Implementation()
{
	const FVector2D MousePositionInViewport = UWidgetLayoutLibrary::GetMousePositionOnViewport(this);
	UGameplayStatics::DeprojectScreenToWorld(this, MousePositionInViewport, MousePositionDeprojectedToWorld, MouseDirectionDeprojectedToWorld);
	if(GetLocalRole() != ROLE_Authority)
	{
		UpdateMousePositionInViewport(MousePositionDeprojectedToWorld, MouseDirectionDeprojectedToWorld);
	}
}

void AGASCoursePlayerController::UpdateMousePositionInViewport_Implementation(FVector InMousePosition, FVector InMouseDirection)
{
	MouseDirectionDeprojectedToWorld = InMouseDirection;
	MousePositionDeprojectedToWorld = InMousePosition;
}

void AGASCoursePlayerController::GetCameraRotation_Implementation(FRotator InCameraRotation)
{
	CameraRotation = InCameraRotation;
}

void AGASCoursePlayerController::Client_GetCameraRotation_Implementation()
{
	if(PlayerCameraManager)
	{
		CameraRotation = PlayerCameraManager->GetCameraRotation();
		if(GetLocalRole() != ROLE_Authority)
		{
			GetCameraRotation(CameraRotation);
		}
	}
}

void AGASCoursePlayerController::GetIsUsingGamepad_Implementation(bool bInUsingGamepad)
{
	bUsingGamepad = bInUsingGamepad;
}

void AGASCoursePlayerController::Client_GetIsUsingGamepad_Implementation()
{
	if(GetLocalRole() != ROLE_Authority)
	{
		GetIsUsingGamepad(bUsingGamepad);
	}
}

void AGASCoursePlayerController::StopMovement_Client_Implementation()
{
	StopMovement();
	if(HasAuthority())
	{
		StopMovement();
	}
	else
	{
		StopMovement_Server();
	}
}

void AGASCoursePlayerController::StopMovement_Server_Implementation()
{
	StopMovement();
	StopMovement_Multicast();
}

void AGASCoursePlayerController::StopMovement_Multicast_Implementation()
{
	StopMovement();
	ForceNetUpdate();
}

bool AGASCoursePlayerController::InputKey(const FInputKeyParams& Params)
{
	bUsingGamepad = Params.IsGamepad();
	Client_GetIsUsingGamepad();
	return Super::InputKey(Params);
}

void AGASCoursePlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	// Needed in case the PC wasn't valid when we Init-ed the ASC.
	if (AGASCoursePlayerState* PS = GetPlayerState<AGASCoursePlayerState>())
	{
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, GetPawn());
	}
	CreateHUD();
}

void AGASCoursePlayerController::OnRep_Pawn()
{
	Super::OnRep_Pawn();
}

void AGASCoursePlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	//GetHitResultUnderMouseCursor();
	GetMousePositionInViewport();
	Client_GetCameraRotation();
}

void AGASCoursePlayerController::OnDamageDealtCallback(const FGameplayEventData* Payload)
{
	OnDamageDealt(*Payload);
}
