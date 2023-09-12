// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Character/Player/GASCoursePlayerController.h"

AGASCoursePlayerController::AGASCoursePlayerController(const FObjectInitializer& ObjectInitializer)
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
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
	CreateHUD();
}

void AGASCoursePlayerController::CreateHUD_Implementation()
{
	
}

void AGASCoursePlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	//CreateHUD_Implementation();
	CreateHUD();
}
