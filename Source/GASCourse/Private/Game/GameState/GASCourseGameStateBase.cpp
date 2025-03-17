// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GameState/GASCourseGameStateBase.h"



AGASCourseGameStateBase::AGASCourseGameStateBase(const FObjectInitializer& ObjectInitializer)
{
	// Enable ticking
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.SetTickFunctionEnable(true);
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void AGASCourseGameStateBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

}

void AGASCourseGameStateBase::BeginPlay()
{
	Super::BeginPlay();
}

void AGASCourseGameStateBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}
