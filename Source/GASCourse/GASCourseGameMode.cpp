// Copyright Epic Games, Inc. All Rights Reserved.

#include "GASCourseGameMode.h"
#include "GASCourseCharacter.h"
#include "UObject/ConstructorHelpers.h"

AGASCourseGameMode::AGASCourseGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/GASCourse/Game/Character/Player/BP_GASCourse_PlayerCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}