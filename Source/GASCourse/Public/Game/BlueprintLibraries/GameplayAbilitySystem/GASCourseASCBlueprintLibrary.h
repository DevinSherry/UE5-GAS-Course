// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffect.h"
#include "Game/GameplayAbilitySystem/GASCourseNativeGameplayTags.h"
#include "Game/GameplayAbilitySystem/GASCourseGameplayAbility.h"
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
 *  @struct FDamageOverTimeContext
 *  @brief Structure representing the context for damage over time.
 *
 *  Structure that holds the parameters necessary for applying damage over time.
 *
 *  @remark This structure is blueprintable.
 */
USTRUCT(blueprintable)
struct FDamageOverTimeContext
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamagePeriod = -1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(Categories="Damage.Type"))
	float DamageDuration = -1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite )
	bool bApplyDamageOnApplication = true;
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
	 * Applies damage to multiple targets using the provided target data handle and damage context.
	 *
	 * @param TargetHandle - The gameplay ability target data handle representing the targets to apply damage to.
	 * @param Instigator - The actor that caused the damage.
	 * @param Damage - The amount of damage to apply.
	 * @param DamageContext - The context for applying the damage, containing information about the damage event.
	 *
	 * @return True if the damage was successfully applied to at least one target, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "GASCourse|AbilitySystem|Damage")
	static bool ApplyDamageToTargetDataHandle(FGameplayAbilityTargetDataHandle TargetHandle, AActor* Instigator, float Damage, const FDamageContext& DamageContext);

	/**
	 * Applies damage over time to a target actor.
	 *
	 * @param Target                      The actor to apply damage over time to.
	 * @param Instigator                  The actor that caused the damage over time.
	 * @param Damage                      The amount of damage to apply over time.
	 * @param DamageContext               The context of the damage being applied.
	 * @param DamageOverTimeContext       The context of the damage over time being applied.
	 *
	 * @return                            Returns true if the damage over time was successfully applied, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "GASCourse|AbilitySystem|Damage")
	static bool ApplyDamageOverTimeToTarget(AActor* Target, AActor* Instigator, float Damage, const FDamageContext& DamageContext, const FDamageOverTimeContext& DamageOverTimeContext);

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
	 * Applies damage to a target actor using a gameplay effect.
	 *
	 * @param Target           The actor to apply damage to.
	 * @param Instigator       The actor causing the damage.
	 * @param Damage           The amount of damage to apply.
	 * @param DamageContext    The context of the damage.
	 * @param GameplayEffect   The gameplay effect to apply.
	 *
	 * @return True if the damage is successfully applied, false otherwise.
	 */
	static bool ApplyDamageToTarget_Internal(AActor* Target, AActor* Instigator, float Damage, const FDamageContext& DamageContext, UGameplayEffect* GameplayEffect);

	static UGameplayEffect* ConstructDamageGameplayEffect(EGameplayEffectDurationType DurationType, const FDamageOverTimeContext& DamageOverTimeContext);

	UFUNCTION(BlueprintPure, Category = "GASCourse|AbilitySystem|Damage")
	static bool FindDamageTypeTagInContainer(const FGameplayTagContainer& InContainer, FGameplayTag& DamageTypeTag);

	/**
	 * Retrieves the gameplay ability slot type from the specified ability spec handle.
	 *
	 * @param AbilitySystem The ability system component to retrieve the ability spec from.
	 * @param AbilitySpecHandle The handle of the ability spec to retrieve the slot type from.
	 * @return The gameplay ability slot type associated with the specified ability spec handle.
	 */
	UFUNCTION(BlueprintPure, Category = "GASCourse|AbilitySystem|GameplayAbility")
	static EGASCourseAbilitySlotType GetGameplayAbilitySlotTypeFromHandle(const UAbilitySystemComponent* AbilitySystem, const FGameplayAbilitySpecHandle& AbilitySpecHandle);

	UFUNCTION(BlueprintCallable, Category = "GASCourse|AbilitySystem|GameplayAbility")
	static void GetAllAbilitiesofAbilitySlotType(const UAbilitySystemComponent* AbilitySystem, EGASCourseAbilitySlotType AbilitySlot, TArray<FGameplayAbilitySpecHandle>& OutAbilityHandles);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "GASCourse|AbilitySystem|Damage")
	static void SendGameplayEventToTargetDataHandle(FGameplayAbilityTargetDataHandle TargetHandle, FGameplayTag EventTag, FGameplayEventData Payload);
};
