// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilitySystemInterface.h"
#include "Game/GameplayAbilitySystem/GASCourseAbilitySystemComponent.h"
#include "GameFramework/PlayerState.h"
#include "GASCoursePlayerState.generated.h"

/**
 * @class AGASCoursePlayerState
 * @brief Represents the player state in the GAS (Gameplay Ability System) course. It derives from APlayerState and implements the IAbilitySystemInterface.
 *
 * This class is responsible for managing the Gameplay Ability System component at the player state level.
 */
UCLASS()
class GASCOURSE_API AGASCoursePlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:

	AGASCoursePlayerState();

	UFUNCTION(BlueprintCallable, Category="GASCourse|PlayerState")
	UGASCourseAbilitySystemComponent* GetGASCourseAbilitySystemComponent() const {return AbilitySystemComponent;}
	
	virtual UGASCourseAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual bool ReplicateSubobjects(class UActorChannel *Channel, class FOutBunch *Bunch, FReplicationFlags *RepFlags) override;
	
	virtual void PostInitializeComponents() override;

private:

	UPROPERTY(VisibleAnywhere, Category="GASCourse|PlayerState")
	TObjectPtr<UGASCourseAbilitySystemComponent> AbilitySystemComponent;
	
};
