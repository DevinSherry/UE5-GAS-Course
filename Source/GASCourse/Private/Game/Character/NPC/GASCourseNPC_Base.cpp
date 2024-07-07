// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Character/NPC/GASCourseNPC_Base.h"
#include "Game/Character/Components/Health/GASC_HealthComponent.h"

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
	if(AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		InitializeAbilitySystem(AbilitySystemComponent);
		//RegisterViewModels();
	}
}

void AGASCourseNPC_Base::BeginPlay()
{
	Super::BeginPlay();
}

void AGASCourseNPC_Base::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AGASCourseNPC_Base::OnRep_Controller()
{
	Super::OnRep_Controller();
	if(AbilitySystemComponent)
	{
		GetAbilitySystemComponent()->RefreshAbilityActorInfo();
	}

}
