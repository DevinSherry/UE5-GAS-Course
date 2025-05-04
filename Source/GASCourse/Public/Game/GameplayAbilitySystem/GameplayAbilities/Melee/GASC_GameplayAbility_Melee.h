// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Game/GameplayAbilitySystem/GameplayAbilities/Instant/GASCourseInstantGameplayAbility.h"
#include "GASC_GameplayAbility_Melee.generated.h"

USTRUCT(BlueprintType)
struct FMeleeAbilityData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Melee Ability Data")
	FGameplayTag MeleeAbilityAnimationTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Melee Ability Data")
	float MinWarpDistance = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Melee Ability Data")
	float NoTargetWarpDistance = 0.0f;

};

/**
 * UGASC_GameplayAbility_Melee is a gameplay ability class designed for implementing melee-specific behaviors
 * within the Gameplay Ability System (GAS).
 *
 * This class serves as a specialized extension of UGASCourseInstantGameplayAbility to provide
 * functionality specifically tailored for melee-related gameplay logic, such as melee attacks or abilities.
 *
 * Features may include handling melee effects, handling melee-specific animations, and integrating with the
 * GAS framework for activating, deactivating, and managing melee abilities. Additional logic specialized
 * for melee combat can be implemented here.
 */
UCLASS()
class GASCOURSE_API UGASC_GameplayAbility_Melee : public UGASCourseInstantGameplayAbility
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Melee Ability Data")
	FMeleeAbilityData MeleeAbilityData;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
};
