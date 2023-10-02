// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Character/Components/GASCourseChampionComponent.h"
#include "Game/Character/Player/GASCoursePlayerState.h"
#include "GASCourse/GASCourseCharacter.h"

// Sets default values for this component's properties
UGASCourseChampionComponent::UGASCourseChampionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
}


// Called when the game starts
void UGASCourseChampionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UGASCourseChampionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UGASCourseChampionComponent::InitializeChampionComponent(UGASCourseAbilitySystemComponent* ASC)
{
	if(const AGASCourseCharacter* Champion = CastChecked<AGASCourseCharacter>(GetOwner()))
	{
		if(AGASCoursePlayerState* PS = CastChecked<AGASCoursePlayerState>(Champion->GetPlayerState()))
		{
			PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, GetOwner());
			
			if(Champion->GetLocalRole() != ROLE_Authority || !ASC)
			{
				return;
			}
			if(DefaultAbilitySet)
			{
				DefaultAbilitySet->GiveToAbilitySystem(ASC, nullptr);
			}

			ASC->SetTagRelationshipMapping(AbilityTagRelationshipMapping);
		}
	}
}
