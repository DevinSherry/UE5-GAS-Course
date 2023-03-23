// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "Game/GameplayAbilitySystem/GASCourseAbilitySystemComponent.h"
#include "GameFramework/PlayerState.h"
#include "GASCoursePlayerState.generated.h"

/**
 * 
 */
UCLASS()
class GASCOURSE_API AGASCoursePlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:

	AGASCoursePlayerState();
	
	virtual UGASCourseAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual bool ReplicateSubobjects(class UActorChannel *Channel, class FOutBunch *Bunch, FReplicationFlags *RepFlags) override;

protected:
	
	UGASCourseAbilitySystemComponent* AbilitySystemComponent = nullptr;
	
};
