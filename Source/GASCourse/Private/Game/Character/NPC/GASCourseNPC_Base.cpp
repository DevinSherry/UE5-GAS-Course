// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Character/NPC/GASCourseNPC_Base.h"

AGASCourseNPC_Base::AGASCourseNPC_Base(const FObjectInitializer& ObjectInitializer) :
Super(ObjectInitializer)
{
}

void AGASCourseNPC_Base::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

void AGASCourseNPC_Base::BeginPlay()
{
	Super::BeginPlay();
}

void AGASCourseNPC_Base::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}
