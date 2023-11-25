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

	UFUNCTION()
	void ApplyGameplayStatusEffect(UAbilitySystemComponent* TargetASC, UAbilitySystemComponent* InstigatorASC, const FGameplayTagContainer& StatusEffectTags);

protected:

	bool HasMatchingStatusEffectTag(const FGameplayTagContainer& StatusEffectTags, FGameplayTagEventResponseTableEntry& FoundStatusEffectEntry, FGameplayTag& FoundTag);
	
};
