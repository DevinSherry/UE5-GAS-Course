// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActiveGameplayEffectHandle.h"
#include "GameplayEffectTypes.h"
#include "GameplayEffect.h"
#include "Components/ActorComponent.h"
#include "GASCStatusEffectListenerComp.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStatusEffectApplied, FActiveGameplayEffectHandle, StatusEffectSpec);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable )
class GASCOURSE_API UGASCStatusEffectListenerComp : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGASCStatusEffectListenerComp();

	UFUNCTION(NetMulticast, Reliable)
	void OnStatusEffectApplied(UAbilitySystemComponent* Source, const FGameplayEffectSpec& GameplayEffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle);

	UFUNCTION(Server, Reliable)
	void OnStatusEffectRemoved(const FActiveGameplayEffect& ActiveGameplayEffect);
	
	UPROPERTY(BlueprintAssignable)
	FStatusEffectApplied OnStatusEffectAppliedHandle;

	UPROPERTY(BlueprintAssignable)
	FStatusEffectApplied OnStatusEffectRemovedHandle;

	UPROPERTY(EditAnywhere, Category = "GASCourse|StatusEffect|Tags")
	FGameplayTag StatusEffectAssetTag;

	UFUNCTION()
	void ApplyDefaultActiveStatusEffects();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void Deactivate() override;
		
};
