// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/Character/Components/GASCStatusEffectListenerComp.h"
#include "GASCourseCharacter.h"

// Sets default values for this component's properties
UGASCStatusEffectListenerComp::UGASCStatusEffectListenerComp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}
void UGASCStatusEffectListenerComp::OnStatusEffectRemoved_Implementation(const FActiveGameplayEffect& ActiveGameplayEffect)
{
	FGameplayTagContainer GameplayEffectAssetTags;
	ActiveGameplayEffect.Spec.GetAllAssetTags(GameplayEffectAssetTags);

	if(GameplayEffectAssetTags.IsEmpty())
	{
		return;
	}

	if(GameplayEffectAssetTags.HasTag(StatusEffectAssetTag))
	{
		OnStatusEffectRemovedHandle.Broadcast(ActiveGameplayEffect.Handle);
	}
}

void UGASCStatusEffectListenerComp::OnStatusEffectApplied_Implementation(UAbilitySystemComponent* Source, const FGameplayEffectSpec& GameplayEffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle)
{
	FGameplayTagContainer GameplayEffectAssetTags;
	GameplayEffectSpec.GetAllAssetTags(GameplayEffectAssetTags);

	if(GameplayEffectAssetTags.IsEmpty())
	{
		return;
	}

	if(GameplayEffectAssetTags.HasTag(StatusEffectAssetTag))
	{
		OnStatusEffectAppliedHandle.Broadcast(ActiveGameplayEffectHandle);
	}
}

void UGASCStatusEffectListenerComp::ApplyDefaultActiveStatusEffects()
{
	if(const AGASCourseCharacter* OwningCharacter = Cast<AGASCourseCharacter>(GetOwner()))
	{
		if(const UAbilitySystemComponent* ASC = OwningCharacter->GetAbilitySystemComponent())
		{	
			TArray<FActiveGameplayEffectHandle> ActiveHandles = ASC->GetActiveEffectsWithAllTags(StatusEffectAssetTag.GetSingleTagContainer());
			for(const FActiveGameplayEffectHandle InActiveHandle : ActiveHandles)
			{
				OnStatusEffectAppliedHandle.Broadcast(InActiveHandle);
			}
		}
	}
}

// Called when the game starts
void UGASCStatusEffectListenerComp::BeginPlay()
{
	Super::BeginPlay();

	if(const AGASCourseCharacter* OwningCharacter = Cast<AGASCourseCharacter>(GetOwner()))
	{
		if(UAbilitySystemComponent* ASC = OwningCharacter->GetAbilitySystemComponent())
		{
			ASC->OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &ThisClass::OnStatusEffectApplied);
			ASC->OnAnyGameplayEffectRemovedDelegate().AddUObject(this, &ThisClass::OnStatusEffectRemoved);
		}
	}
}

void UGASCStatusEffectListenerComp::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if(OnStatusEffectAppliedHandle.IsBound())
	{
		OnStatusEffectAppliedHandle.Clear();
	}
	
	Super::EndPlay(EndPlayReason);
}

void UGASCStatusEffectListenerComp::Deactivate()
{
	if(OnStatusEffectAppliedHandle.IsBound())
	{
		OnStatusEffectAppliedHandle.Clear();
	}
	
	Super::Deactivate();
}
