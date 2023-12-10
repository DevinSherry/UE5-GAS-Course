// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilitySystemBlueprintLibrary.h"
#include "Game/GameplayAbilitySystem/GASCourseNativeGameplayTags.h"
#include "GASCourseASCBlueprintLibrary.generated.h"

/** Represents a context for applying damage to an object or character.
 *  Contains information related to the damage event such as the hit result,
 *  damage type, and additional gameplay tags associated with the damage.
 */

USTRUCT(blueprintable)
struct FDamageContext
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite )
	FHitResult HitResult;

	UPROPERTY(EditAnywhere, BlueprintReadWrite , meta=(Categories="Damage.Type"))
	FGameplayTag DamageType = DamageType_Physical;

	UPROPERTY(EditAnywhere, BlueprintReadWrite )
	FGameplayTagContainer GrantedTags;
};

/**
 * UGASCourseASCBlueprintLibrary is a blueprint library that provides utility functions for applying damage to target actors.
 */

UCLASS()
class GASCOURSE_API UGASCourseASCBlueprintLibrary : public UAbilitySystemBlueprintLibrary
{
	GENERATED_BODY()

public:
	
	/**
	 * Applies damage to the specified target actor using the specified instigator actor, damage amount, and damage context.
	 * This method is blueprint callable and can only be executed by an authority.
	 *
	 * @param Target The actor to apply the damage to.
	 * @param Instigator The actor initiating the damage.
	 * @param Damage The amount of damage to apply.
	 * @param DamageContext The context of the damage being applied.
	 * @return True if the damage was successfully applied, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "GASCourse|AbilitySystem|Damage")
	static bool ApplyDamageToTarget(AActor* Target, AActor* Instigator, float Damage, const FDamageContext& DamageContext);

	/**
	 * Applies physical damage to the specified target.
	 *
	 * @param Target    The actor to apply damage to.
	 * @param Instigator    The actor responsible for the damage.
	 * @param Damage    The amount of physical damage to apply.
	 * @param HitResult    The hit result information of the damage.
	 * @param DamageContext    The damage context information. Context is hidden and initialized in the function to pass DamageType_Physical by default.
	 * @return    True if the damage was successfully applied, false otherwise.
	 */

	UFUNCTION(BlueprintCallable, meta=(Hidepin = "DamageContext"), BlueprintAuthorityOnly, Category = "GASCourse|AbilitySystem|Damage")
	static bool ApplyPhysicalDamageToTarget(AActor* Target, AActor* Instigator, float Damage, const FHitResult& HitResult, FDamageContext& DamageContext);

	/**
	 * Apply fire damage to a target actor.
	 *
	 * @param Target The actor to apply the fire damage to.
	 * @param Instigator The actor that initiated the fire damage.
	 * @param Damage The amount of damage to apply.
	 * @param HitResult The hit result of the fire damage.
	 * @param DamageContext The damage context containing additional information about the fire damage. Context is hidden and initialized in the function to pass DamageType_Elemental_Fire by default.
	 * @param bApplyBurnStack Whether to apply a burn stack effect.
	 *
	 * @return True if the fire damage was successfully applied, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, meta=(Hidepin = "DamageContext"), BlueprintAuthorityOnly, Category = "GASCourse|AbilitySystem|Damage")
	static bool ApplyFireDamageToTarget(AActor* Target, AActor* Instigator, float Damage, const FHitResult& HitResult, FDamageContext& DamageContext, bool bApplyBurnStack = true);

	/**
	 * Applies damage to a target actor.
	 *
	 * @param Target The actor being damaged.
	 * @param Instigator The actor causing the damage.
	 * @param Damage The amount of damage to be applied.
	 * @param DamageContext Additional context information for the damage.
	 *
	 * @return True if the damage was successfully applied, false otherwise.
	 */
	static bool ApplyDamageToTarget_Internal(AActor* Target, AActor* Instigator, float Damage, const FDamageContext& DamageContext);
	
};
