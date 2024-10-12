// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GameState/GASCourseGameStateBase.h"


#if ENABLE_COG
#include "CogAll.h"
#include "CogDebugPlot.h"
#include "Debug/Cog/CogSampleWindow_Team.h"
#include "CogWindowManager.h"
#endif //ENABLE_COG



AGASCourseGameStateBase::AGASCourseGameStateBase(const FObjectInitializer& ObjectInitializer)
{
	// Enable ticking
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.SetTickFunctionEnable(true);
	PrimaryActorTick.bStartWithTickEnabled = true;
}


void AGASCourseGameStateBase::InitializeCog()
{
}


void AGASCourseGameStateBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	/*
#if ENABLE_COG
    
	if (CogWindowManager != nullptr)
	{
		CogWindowManager->Shutdown();
	}
#endif //ENABLE_COG

*/

}

void AGASCourseGameStateBase::BeginPlay()
{
	Super::BeginPlay();

#if ENABLE_COG
	CogWindowManager = NewObject<UCogWindowManager>(this);
	CogWindowManagerRef = CogWindowManager;

	// Add all the built-in windows
	Cog::AddAllWindows(*CogWindowManager);

	// Add a custom window 
	CogWindowManager->AddWindow<FCogSampleWindow_Team>("Gameplay.Team");
#endif //ENABLE_COG


}

void AGASCourseGameStateBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

#if ENABLE_COG

	extern ENGINE_API float GAverageFPS;
	extern ENGINE_API float GAverageMS;
	FCogDebugPlot::PlotValue(this, "Frame Rate", GAverageFPS);
	FCogDebugPlot::PlotValue(this, "Frame Time", GAverageMS);

	if (CogWindowManager != nullptr)
	{
		CogWindowManager->Tick(DeltaSeconds);
	}

#endif //ENABLE_COG


}
