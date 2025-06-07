// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Game/GameplayAbilitySystem/GASCourseGameplayAbility.h"
#include "GASC_NPC_GameplayAbilityBase.generated.h"

USTRUCT(BlueprintType)
struct FNPCAbilityData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "NPC Ability Data")
	AActor* Target;
	
};

/**
 * 
 */
UCLASS()
class GASCOURSE_API UGASC_NPC_GameplayAbilityBase : public UGASCourseGameplayAbility
{
	GENERATED_BODY()
	
public:
	
	UGASC_NPC_GameplayAbilityBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "NPC Ability Data")
	FNPCAbilityData NPCAbilityData;

	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	
};
