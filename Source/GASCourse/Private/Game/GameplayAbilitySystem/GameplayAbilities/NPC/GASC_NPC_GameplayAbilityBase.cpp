// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GameplayAbilitySystem/GameplayAbilities/NPC/GASC_NPC_GameplayAbilityBase.h"

#include "Game/Character/NPC/GASCourseNPC_Base.h"


UGASC_NPC_GameplayAbilityBase::UGASC_NPC_GameplayAbilityBase(const FObjectInitializer& ObjectInitializer)
{
	AbilitySlotType = EGASCourseAbilitySlotType::EmptySlot;
}

void UGASC_NPC_GameplayAbilityBase::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	AGASCourseNPC_Base* Character = CastChecked<AGASCourseNPC_Base>(ActorInfo->AvatarActor.Get());

	UGASC_NPC_GameplayAbilityBase* SourceAbilityObjectAbility_NPC = CastChecked<UGASC_NPC_GameplayAbilityBase>(Spec.SourceObject);
	if (!SourceAbilityObjectAbility_NPC)
	{
		UE_LOG(LOG_GASC_GameplayAbility, Error, TEXT("Invalid Source Object. No melee data found: %s"), *GetPathNameSafe(this));
		EndAbility(Spec.Handle, ActorInfo, CurrentActivationInfo, true, true);
	}

	NPCAbilityData = SourceAbilityObjectAbility_NPC->NPCAbilityData;
}
