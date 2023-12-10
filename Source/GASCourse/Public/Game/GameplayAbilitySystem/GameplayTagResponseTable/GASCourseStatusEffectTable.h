// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "GameplayEffectTypes.h"
#include "GameplayEffect.h"
#include "GASCourseStatusEffectTable.generated.h"

class UAbilitySystemComponent;

/**
 * 
 */

USTRUCT()
struct FGameplayTagEventResponsePair
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category = "Response")
	FGameplayTag StatusEffectStateTag;
	
	UPROPERTY(EditAnywhere, Category = "Response")
	TSubclassOf<UGameplayEffect> ResponseGameplayEffect;
	
};

USTRUCT()
struct FGameplayTagEventResponseTableEntry
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category = "Response")
	FGameplayTag StatusEffectTag;
	
	/** Tags that count as "positive" toward to final response count. If the overall count is positive, this ResponseGameplayEffect is applied. */
	UPROPERTY(EditAnywhere, Category="Response")
	TArray<FGameplayTagEventResponsePair> StatusEffectTypes;
	
};

UCLASS()
class GASCOURSE_API UGASCourseStatusEffectTable : public UDataAsset
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(EditAnywhere, Category="Response")
	TArray<FGameplayTagEventResponseTableEntry>	Entries;

	virtual void PostLoad() override;

	/**
	 * Function to apply a gameplay status effect to the target ability system component based on the provided status effect tags.
	 *
	 * @param TargetASC The ability system component to apply the status effect to.
	 * @param InstigatorASC The ability system component that is initiating the application of the status effect.
	 * @param StatusEffectTags The gameplay tags representing the status effect to be applied.
	 */
	UFUNCTION()
	void ApplyGameplayStatusEffect(UAbilitySystemComponent* TargetASC, UAbilitySystemComponent* InstigatorASC, const FGameplayTagContainer& StatusEffectTags);

protected:
	
	/**
	 * Check if any of the given status effect tags matches the entries in the status effect table.
	 *
	 * @param StatusEffectTags The tags to check against the status effect table entries.
	 * @param FoundStatusEffectEntry The found status effect entry when a match is found.
	 * @param FoundTag The found tag when a match is found.
	 * @return True if a match is found, false otherwise.
	 */
	bool HasMatchingStatusEffectTag(const FGameplayTagContainer& StatusEffectTags, FGameplayTagEventResponseTableEntry& FoundStatusEffectEntry, FGameplayTag& FoundTag);
	
};
