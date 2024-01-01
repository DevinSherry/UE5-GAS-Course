// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

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

	UFUNCTION(BlueprintCallable, Category="GASCourse|PlayerState")
	UGASCourseAbilitySystemComponent* GetGASCourseAbilitySystemComponent() const {return AbilitySystemComponent;}
	
	virtual UGASCourseAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual bool ReplicateSubobjects(class UActorChannel *Channel, class FOutBunch *Bunch, FReplicationFlags *RepFlags) override;
	
	virtual void PostInitializeComponents() override;

private:

	UPROPERTY(VisibleAnywhere, Category="GASCourse|PlayerState")
	TObjectPtr<UGASCourseAbilitySystemComponent> AbilitySystemComponent;
	
};
