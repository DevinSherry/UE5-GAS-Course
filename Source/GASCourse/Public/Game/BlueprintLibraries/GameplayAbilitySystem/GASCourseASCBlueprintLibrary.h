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
USTRUCT(Blueprintable)
struct FEffectOverTimeContext
{
	GENERATED_USTRUCT_BODY()

public:
	/** Defines the interval, in seconds, at which a periodic gameplay effect is applied.
	 *  A negative value indicates that the effect is not periodic and will not execute
	 *  at intervals. If set to a valid positive value, the effect will trigger periodically
	 *  with the specified time gap.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EffectPeriod = -1.0f;

	/** Specifies the total duration, in seconds, for which a gameplay effect remains active.
	 *  A negative value indicates that the effect has an indefinite duration or does not execute over a specific time period.
	 *  If set to a positive value, the effect will be active for the specified time span.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EffectDuration = -1.0f;

	/** Indicates whether a gameplay effect should be applied immediately upon its activation.
	 *  If true, the effect will execute its logic as soon as it is activated, in addition to any periodic or duration-based execution.
	 *  If false, the effect will only execute based on its periodic or duration settings, if applicable.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite )
	bool bApplyEffectOnApplication = true;

	/** Determines whether a value should be distributed evenly over the total duration of the effect.
	 *  If true, the value will be applied proportionally throughout the entire duration, rather than at discrete intervals.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite )
	bool bApplyValueOverTotalDuration = false;
};

/**
 * UGASCourseASCBlueprintLibrary is a blueprint library that provides utility functions for applying damage to target actors.
 */

UCLASS()
class GASCOURSE_API UGASCourseASCBlueprintLibrary : public UAbilitySystemBlueprintLibrary
{
	GENERATED_BODY()

public:

	/*
	UFUNCTION(BlueprintCallable, Category = "GASCourse|AbilitySystem|AreaOfEffect")
	static bool ApplyAreaOfEffect(AActor* Instigator, class UGASC_AreaOfEffectData* AreaOfEffectData, const FVector& AreaCenter);
	*/
	
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
	UFUNCTION(BlueprintCallable, Category = "GASCourse|AbilitySystem|Damage")
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
	UFUNCTION(BlueprintCallable, Category = "GASCourse|AbilitySystem|Damage")
	static bool ApplyDamageToTargetDataHandle(FGameplayAbilityTargetDataHandle TargetHandle, AActor* Instigator, float Damage, FDamageContext DamageContext);

	/** Applies a healing effect to a target actor internally.
	 *  This function processes the application of healing by validating the target and instigator,
	 *  retrieving and preparing the necessary healing execution settings from the gameplay mechanics,
	 *  and ensuring the proper ability system components are set for both the target and instigator.
	 *  Healing is applied through specified gameplay effects, which may include over-time contexts
	 *  defined by the execution parameters.
	 *
	 *  @param Target The actor to which healing will be applied.
	 *  @param Instigator The actor responsible for initiating the healing effect.
	 *  @param InHealing The numerical value of healing to be applied.
	 *  @param GameplayEffect The gameplay effect object that defines the specifics of the healing logic.
	 *  @param EffectOverTimeContext Context information for handling healing that occurs over a period of time.
	 *  @return True if healing was successfully applied, false otherwise.
	 */
	static bool ApplyHealingToTarget_Internal(AActor* Target, AActor* Instigator, float InHealing, UGameplayEffect* GameplayEffect, FEffectOverTimeContext EffectOverTimeContext);

	/** Applies healing to all targets specified in the given TargetDataHandle.
	 *  This function allows healing to be applied to multiple targets while including
	 *  the actor initiating the healing and the amount of healing to be applied.
	 *
	 *  @param TargetHandle The handle containing data about the targets to apply healing to.
	 *  @param Instigator The actor responsible for instigating the healing action.
	 *  @param InHealing The amount of healing to be applied to the targets.
	 *  @return True if the healing was successfully applied, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category = "GASCourse|AbilitySystem|Healing")
	static bool ApplyHealingToTargetDataHandle(FGameplayAbilityTargetDataHandle TargetHandle, AActor* Instigator, float InHealing);

	/** Applies healing to a target actor.
	 *  This method allows applying a specified amount of healing to the target actor,
	 *  originating from a specified instigator. It uses an internally constructed
	 *  healing gameplay effect to handle the application of healing.
	 *
	 *  @param Target The actor that will receive the healing.
	 *  @param Instigator The actor responsible for performing the healing.
	 *  @param InHealing The amount of healing to apply to the target.
	 *  @return Returns true if the healing was successfully applied; otherwise, false.
	 */
	UFUNCTION(BlueprintCallable, Category = "GASCourse|AbilitySystem|Healing")
	static bool ApplyHealingToTarget(AActor* Target, AActor* Instigator, float InHealing);

	/** Applies a healing effect over time to the specified target.
	 *  This function triggers a healing effect that is applied incrementally over a duration,
	 *  based on the provided healing amount and effect context.
	 *
	 *  @param Target The actor to which the healing effect will be applied.
	 *  @param Instigator The actor responsible for triggering the healing effect.
	 *  @param InHealing The total amount of healing to apply over time.
	 *  @param EffectOverTimeContext The context defining the details of the healing effect, including duration and other parameters.
	 *  @return True if the healing effect was successfully applied, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category = "GASCourse|AbilitySystem|Damage")
	static bool ApplyHealingOverTimeToTarget(AActor* Target, AActor* Instigator, float InHealing, const FEffectOverTimeContext& EffectOverTimeContext);

	/**
	 * Applies damage over time to a target actor.
	 *
	 * @param Target                      The actor to apply damage over time to.
	 * @param Instigator                  The actor that caused the damage over time.
	 * @param Damage                      The amount of damage to apply over time.
	 * @param DamageContext               The context of the damage being applied.
	 * @param EffectOverTimeContext       The context of the damage over time being applied.
	 *
	 * @return                            Returns true if the damage over time was successfully applied, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category = "GASCourse|AbilitySystem|Damage")
	static bool ApplyDamageOverTimeToTarget(AActor* Target, AActor* Instigator, float Damage, const FDamageContext& DamageContext, const FEffectOverTimeContext& EffectOverTimeContext);

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

	UFUNCTION(BlueprintCallable, meta=(Hidepin = "DamageContext"), Category = "GASCourse|AbilitySystem|Damage")
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

	static UGameplayEffect* ConstructDamageGameplayEffect(EGameplayEffectDurationType DurationType, const FEffectOverTimeContext& EffectOverTimeContext);

	static UGameplayEffect* ConstructHealingGameplayEffect(EGameplayEffectDurationType DurationType, const FEffectOverTimeContext& EffectOverTimeContext);

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

	/**
	 * Retrieves the gameplay attribute from a given modifier struct.
	 *
	 * @param ModifierInfo The gameplay modifier info struct.
	 * @return The gameplay attribute.
	 */
	UFUNCTION(BlueprintCallable, Category = "GASCourse|AbilitySystem|GameplayEffect")
	static FGameplayAttribute GetGameplayAttributeFromModifierStruct(const FGameplayModifierInfo& ModifierInfo);

	/**
	 * Calculates the magnitude of a modifier for a given gameplay effect.
	 *
	 * @param InGameplayEffect The handle to the gameplay effect.
	 * @param ModifierIdx The index of the modifier to calculate the magnitude for.
	 * @param bFactorInStackCount Specifies whether to factor in the stack count of the gameplay effect.
	 *
	 * @return The magnitude of the specified modifier. If the modifier is not found or the gameplay effect is invalid,
	 *         returns 0.0f.
	 */
	UFUNCTION(BlueprintCallable, Category = "GASCourse|AbilitySystem|GameplayEffect")
	static float GetModifierMagnitudeAtIndex(FActiveGameplayEffectHandle InGameplayEffect, int32 ModifierIdx, bool bFactorInStackCount);

	/**
	 * Retrieves the gameplay effect specification handle associated with the given active gameplay effect handle.
	 *
	 * @param InGameplayEffect The active gameplay effect handle for which to retrieve the gameplay effect specification handle.
	 * @return The gameplay effect specification handle associated with the given active gameplay effect handle.
	 */
	UFUNCTION(BlueprintCallable, Category = "GASCourse|AbilitySystem|GameplayEffect")
	static FGameplayEffectSpec GetSpecHandleFromGameplayEffect(FActiveGameplayEffectHandle InGameplayEffect);

	/**
	 * Retrieves the period of a gameplay effect.
	 *
	 * @param InGameplayEffect The handle to the active gameplay effect.
	 * @return The period of the gameplay effect.
	 */
	UFUNCTION(BlueprintPure, Category =  "GASCourse|AbilitySystem|GameplayEffect")
	static float GetPeriodFromGameplayEffect(FActiveGameplayEffectHandle InGameplayEffect);

	UFUNCTION(BlueprintCallable, Category = "GASCourse|AbilitySystem|GameplayAbility")
	static bool GrantAbilityToInputTag(UAbilitySystemComponent* InASC, TSubclassOf<UGASCourseGameplayAbility> Ability, int AbilityLevel, FGameplayTag InputTag);
};
