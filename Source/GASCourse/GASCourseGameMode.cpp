// Copyright Epic Games, Inc. All Rights Reserved.

#include "GASCourseGameMode.h"
#include "GASCourseCharacter.h"
#include "Game/HUD/GASCourseHUD.h"
#include "Game/Character/Player/GASCoursePlayerCharacter.h"
#include "Game/Character/Player/GASCoursePlayerController.h"
#include "Game/Character/Player/GASCoursePlayerState.h"
#include "Game/GameState/GASCourseGameStateBase.h"
#include "UObject/ConstructorHelpers.h"

AGASCourseGameMode::AGASCourseGameMode()
{
	/*
	static ConstructorHelpers::FClassFinder<AGASCoursePlayerCharacter> PlayerPawnBPClass(TEXT("/Game/GASCourse/Game/Character/Player/BP_GASCourse_PlayerCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	else
	{
		DefaultPawnClass = AGASCourseCharacter::StaticClass();
	}
	*/

	//For whatever reason, putting the controller BP class here will somehow load in too early for MVVM plugin and break things.
	/*
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/GASCourse/Game/Character/Player/BP_GASCourse_PlayerController"));
	if (PlayerControllerBPClass.Class != nullptr)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
	else
	{
		PlayerControllerClass = AGASCoursePlayerController::StaticClass();
	}
	*/
	PlayerControllerClass = AGASCoursePlayerController::StaticClass();

	static ConstructorHelpers::FClassFinder<AGameStateBase> GameStateBaseBPClass(TEXT("/Game/GASCourse/Game/GameStateBase/BP_GASCourse_GameStateBase"));
	if (GameStateBaseBPClass.Class != nullptr)
	{
		GameStateClass = GameStateBaseBPClass.Class;
	}
	else
	{
		GameStateClass = AGASCourseGameStateBase::StaticClass();
	}
	
	static ConstructorHelpers::FClassFinder<APlayerState> PlayerStateBPClass(TEXT("/Game/GASCourse/Game/Character/Player/BP_GASCourse_PlayerState"));
    if (PlayerStateBPClass.Class != nullptr)
    {
    	PlayerStateClass = PlayerStateBPClass.Class;
    }
    else
    {
    	PlayerStateClass = AGASCoursePlayerState::StaticClass();
    }
	
	HUDClass = AGASCourseHUD::StaticClass();
}

void AGASCourseGameMode::StartPlay()
{
	Super::StartPlay();
}
