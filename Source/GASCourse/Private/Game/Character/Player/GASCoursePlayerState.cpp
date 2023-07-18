// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Character/Player/GASCoursePlayerState.h"
#include "Engine/ActorChannel.h"
#include "Game/Character/Player/GASCoursePlayerCharacter.h"

AGASCoursePlayerState::AGASCoursePlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UGASCourseAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
}

UGASCourseAbilitySystemComponent* AGASCoursePlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

bool AGASCoursePlayerState::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{

	check(Channel);
	check(Bunch);
	check(RepFlags);

	bool WroteSomething = true;

	for (UActorComponent* ActorComp : ReplicatedComponents)
	{
		if (ActorComp && ActorComp->GetIsReplicated())
		{
			// We replicate replicate everything but simulated proxies in ASC
			if (!ActorComp->IsA(AbilitySystemComponent->GetClass()) || RepFlags->bNetOwner || !AbilitySystemComponent->ReplicationProxyEnabled)
			{
				WroteSomething |= ActorComp->ReplicateSubobjects(Channel, Bunch, RepFlags);
				WroteSomething |= Channel->ReplicateSubobject(ActorComp, *Bunch, *RepFlags);
			}
			else
			{
				AGASCoursePlayerCharacter* MyCharacter = GetPawn<AGASCoursePlayerCharacter>();
				MyCharacter->Call_GetReplicationProxyVarList_Mutable().Copy(0, 
					AbilitySystemComponent->GetNumericAttribute(UGASCourseAttributeSet::GetOneAttributeAttribute()),
					AbilitySystemComponent->GetNumericAttribute(UGASCourseAttributeSet::GetTwoAttributeAttribute()));
			}
		}
	} 
	return WroteSomething;
}
