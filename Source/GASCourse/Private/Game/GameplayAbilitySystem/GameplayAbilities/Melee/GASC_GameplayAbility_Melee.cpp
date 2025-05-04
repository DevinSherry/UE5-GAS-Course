// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GameplayAbilitySystem/GameplayAbilities/Melee/GASC_GameplayAbility_Melee.h"


void UGASC_GameplayAbility_Melee::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	const FGameplayAbilitySpec* Spec = ActorInfo->AbilitySystemComponent->FindAbilitySpecFromHandle(Handle);
	if (UGASC_GameplayAbility_Melee* SourceAbilityObjectAbility_Melee = Cast<UGASC_GameplayAbility_Melee>(Spec->SourceObject))
	{
		MeleeAbilityData = SourceAbilityObjectAbility_Melee->MeleeAbilityData;
	}

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}
