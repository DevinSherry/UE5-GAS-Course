// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GameplayAbilitySystem/GameplayAbilities/Melee/GASC_GameplayAbility_Melee.h"

void UGASC_GameplayAbility_Melee::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UGASC_GameplayAbility_Melee::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);
	AGASCourseCharacter* Character = CastChecked<AGASCourseCharacter>(ActorInfo->AvatarActor.Get());
	MotionWarpingComponent = Character->FindComponentByClass<UMotionWarpingComponent>();
	if (!MotionWarpingComponent)
	{
		UE_LOG(LOG_GASC_GameplayAbility, Error, TEXT("Motion Warp Component is invalid in: %s"), *GetPathNameSafe(this));
		EndAbility(Spec.Handle, ActorInfo, CurrentActivationInfo, true, true);
	}

	UGASC_GameplayAbility_Melee* SourceAbilityObjectAbility_Melee = CastChecked<UGASC_GameplayAbility_Melee>(Spec.SourceObject);
	if (!SourceAbilityObjectAbility_Melee)
	{
		UE_LOG(LOG_GASC_GameplayAbility, Error, TEXT("Invalid Source Object. No melee data found: %s"), *GetPathNameSafe(this));
		EndAbility(Spec.Handle, ActorInfo, CurrentActivationInfo, true, true);
	}

	MeleeAbilityData = SourceAbilityObjectAbility_Melee->MeleeAbilityData;
}

void UGASC_GameplayAbility_Melee::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	RemoveMeleeMotionWarpTargets();
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGASC_GameplayAbility_Melee::RemoveMeleeMotionWarpTargets()
{
	if (MotionWarpingComponent)
	{
		MotionWarpingComponent->RemoveWarpTargets({MeleeAbilityData.MeleeWarpTranslationTargetName, MeleeAbilityData.MeleeWarpRotationTargetName});
	}
}
