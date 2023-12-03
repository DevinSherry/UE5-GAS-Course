// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GameplayAbilitySystem/GameplayTagResponseTable/GASCourseStatusEffectTable.h"
#include "AbilitySystemComponent.h"

UGASCourseStatusEffectTable::UGASCourseStatusEffectTable(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
}

void UGASCourseStatusEffectTable::PostLoad()
{
	Super::PostLoad();
}

void UGASCourseStatusEffectTable::ApplyGameplayStatusEffect(UAbilitySystemComponent* TargetASC,
	UAbilitySystemComponent* InstigatorASC, const FGameplayTagContainer& StatusEffectTags)
{
	UE_LOG(LogTemp, Warning, TEXT("Tags: %s"), *StatusEffectTags.ToString());

	FGameplayTagEventResponseTableEntry FoundStatusEffectEntry;
	FGameplayTag FoundStatusTag;
	if(HasMatchingStatusEffectTag(StatusEffectTags, FoundStatusEffectEntry, FoundStatusTag))
	{
		for(FGameplayTagEventResponsePair StatusEffectPair : FoundStatusEffectEntry.StatusEffectTypes)
		{
			if(StatusEffectPair.StatusEffectStateTag == FoundStatusTag)
			{
				FActiveGameplayEffectHandle StatusEffectHandle = InstigatorASC->ApplyGameplayEffectToTarget(StatusEffectPair.ResponseGameplayEffect.GetDefaultObject(), TargetASC);
			}
		}
	}
}

bool UGASCourseStatusEffectTable::HasMatchingStatusEffectTag(const FGameplayTagContainer& StatusEffectTags,
	FGameplayTagEventResponseTableEntry& FoundStatusEffectEntry, FGameplayTag& FoundTag)
{
	bool bHasFoundTag = false;

	for(FGameplayTagEventResponseTableEntry Entry :Entries)
	{
		if(StatusEffectTags.HasTag(Entry.StatusEffectTag))
		{
			FoundStatusEffectEntry = Entry;
			for(FGameplayTag StatusTag : StatusEffectTags.GetGameplayTagArray())
			{
				ensure(StatusTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Effect.Gameplay.Status"))));
				if(StatusTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Effect.Gameplay.Status"))))
				{
					FoundTag = StatusTag;
					bHasFoundTag = true;
					break;
				}
			}
		}
	}

	return bHasFoundTag;
}
