// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Character/NPC/GASCourseNPC_Base.h"

AGASCourseNPC_Base::AGASCourseNPC_Base(const FObjectInitializer& ObjectInitializer) :
Super(ObjectInitializer)
{
	// Create ability system component, and set it to be explicitly replicated
	AbilitySystemComponent = CreateDefaultSubobject<UGASCourseAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Full);
}

void AGASCourseNPC_Base::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

void AGASCourseNPC_Base::BeginPlay()
{
	Super::BeginPlay();
	if(AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		InitializeAbilitySystem(AbilitySystemComponent);
	}
}

void AGASCourseNPC_Base::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AGASCourseNPC_Base::OnRep_Controller()
{
	Super::OnRep_Controller();
	GetAbilitySystemComponent()->RefreshAbilityActorInfo();
}
