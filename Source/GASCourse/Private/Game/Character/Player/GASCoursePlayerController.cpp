// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Character/Player/GASCoursePlayerController.h"
#include "Net/UnrealNetwork.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Game/GameplayAbilitySystem/GASCourseNativeGameplayTags.h"

AGASCoursePlayerController::AGASCoursePlayerController(const FObjectInitializer& ObjectInitializer)
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
}

void AGASCoursePlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AGASCoursePlayerController, HitResultUnderMouseCursor);
	DOREPLIFETIME(AGASCoursePlayerController, MouseDirectionDeprojectedToWorld);
	DOREPLIFETIME(AGASCoursePlayerController, MousePositionDeprojectedToWorld);
}

void AGASCoursePlayerController::BeginPlayingState()
{
	Super::BeginPlayingState();
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

void AGASCoursePlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
}

void AGASCoursePlayerController::OnRep_Pawn()
{
	Super::OnRep_Pawn();
}

void AGASCoursePlayerController::Tick(float DeltaSeconds)
{
	GetHitResultUnderMouseCursor();
	GetMousePositionInViewport();
	Super::Tick(DeltaSeconds);
}

void AGASCoursePlayerController::OnDamageDealtCallback(const FGameplayEventData* Payload)
{
	OnDamageDealt(*Payload);
}
