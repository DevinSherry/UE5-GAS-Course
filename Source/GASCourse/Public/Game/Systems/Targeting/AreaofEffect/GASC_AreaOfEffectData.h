// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GASC_TargetingSelectionTask_AOE.h"
#include "ScalableFloat.h"
#include "Engine/DataAsset.h"
//#include "Game/BlueprintLibraries/GameplayAbilitySystem/GASCourseASCBlueprintLibrary.h"
#include "GASC_TargetingPreset_AOE.h"
#include "Game/BlueprintLibraries/GameplayAbilitySystem/GASCourseASCBlueprintLibrary.h"
#include "TargetingSystem/TargetingPreset.h"
#include "GASC_AreaOfEffectData.generated.h"

UENUM(BlueprintType)
enum class EGASC_AreaOfEffectType : uint8
{
	Damage,
	Healing
};

/**
 * Data asset that defines the properties and configurations related to
 * the area of effect behavior in the gameplay ability system.
 * Includes targeting information, falloff calculations, and range constraints.
 */
UCLASS(Blueprintable)
class GASCOURSE_API UGASC_AreaOfEffectData : public UDataAsset
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category="GASCourse|AreaOfEffect")
	bool ProcessAreaOfEffect(AActor* Instigator, const FVector& Center);

	/**
	 * Defines the type of area of effect applied in the gameplay ability system.
	 * Determines the method in which effects are distributed, such as damage or healing,
	 * within a specified area.
	 */
	UPROPERTY(EditDefaultsOnly, Category="GASCourse|AreaOfEffect")
	EGASC_AreaOfEffectType AreaOfEffectType = EGASC_AreaOfEffectType::Damage;

	/**
	 * Context defining the damage parameters for area of effect scenarios.
	 * Used to configure the damage-related properties applied when the area of effect
	 * type is set to damage. Includes settings specific to damage calculations.
	 */
	UPROPERTY(EditDefaultsOnly, Category="GASCourse|AreaOfEffect", meta=(EditCondition="AreaOfEffectType == EGASC_AreaOfEffectType::Damage", EditConditionHides))
	FDamageContext AreaOfEffectDamageContext;

	/**
	 * The targeting preset is used to gather targets in range in which the area of effect
	 * damage or healing is applied.
	 * Editable in the editor and categorized under "GASCourse|Targeting|AreaOfEffect".
	 */
	UPROPERTY(EditDefaultsOnly, Category="GASCourse|AreaOfEffect")
	UGASC_TargetingPreset_AOE* TargetingPreset = nullptr;

	/**
	 * Defines the falloff curve that determines how the area of effect's impact
	 * (such as damage or healing) diminishes as the distance from the center increases.
	 * Editable in the editor and categorized under "GASCourse|Targeting|AreaOfEffect".
	 */
	UPROPERTY(EditAnywhere, Category="GASCourse|AreaOfEffect")
	FRuntimeFloatCurve AreaOfEffectFallOffCurve;

	/**
	 * Represents the minimum value for the area of effect range, which determines
	 * the innermost boundary where the effect (such as damage or healing) can be applied.
	 * Editable in the editor and categorized under "GASCourse|Targeting|AreaOfEffect".
	 */
	UPROPERTY(EditDefaultsOnly, Category="GASCourse|AreaOfEffect")
	FScalableFloat AreaOfEffectMinValue;

	/**
	 * Boolean property indicating whether the area of effect can cause damage
	 * to allied actors. When set to true, allies within the area will be affected
	 * by the damage calculations; otherwise, they will be excluded.
	 */
	UPROPERTY(EditDefaultsOnly, Category="GASCourse|AreaOfEffect", meta=(EditCondition="AreaOfEffectType == EGASC_AreaOfEffectType::Damage", EditConditionHides))
	bool bCanDamageAllies = false;

	UPROPERTY(EditDefaultsOnly, Category="GASCourse|AreaOfEffect", meta=(EditCondition="AreaOfEffectType == EGASC_AreaOfEffectType::Healing", EditConditionHides))
	bool bCanHealEnemies = false;
	
	/**
	 * Represents the maximum value for the area of effect range, which defines
	 * the outermost boundary where the effect (such as damage or healing) can be applied.
	 * Editable in the editor and categorized under "GASCourse|Targeting|AreaOfEffect".
	 */
	UPROPERTY(EditDefaultsOnly, Category="GASCourse|AreaOfEffect")
	FScalableFloat AreaOfEffectMaxValue;

	/**
	 * Determines whether to visually debug the area of effect in the editor or during runtime.
	 * When enabled, the area of effect will be drawn for debugging purposes, helping to visualize
	 * the range or impact zone of the effect.
	 * Configurable only in the editor and categorized under "GASCourse|AreaOfEffect|Debug".
	 */
	UPROPERTY(EditDefaultsOnly, Category="GASCourse|AreaOfEffect|Debug|Shape")
	bool bDebugDrawAreaOfEffect = false;

	/**
	 * Color used to visually represent the debug shape for the area of effect in the editor.
	 * Configurable through the editor to assist in identifying and debugging area of effect bounds.
	 */
	UPROPERTY(EditDefaultsOnly, Category="GASCourse|AreaOfEffect|Debug|Shape")
	FColor AoEDebugShapeColor = FColor::Red;

	/**
	 * Duration for which the debug shape representing the area of effect is displayed.
	 * Useful for visual debugging to validate area of effect targeting and placement.
	 * Configurable in the Editor under the specified debug category.
	 */
	UPROPERTY(EditDefaultsOnly, Category="GASCourse|AreaOfEffect|Debug|Shape")
	float AoEDebugShapeDuration = 5.0f;
	
	/**
	 * Determines whether the affected actors within the area of effect should be visually highlighted
	 * or drawn for debugging purposes. This can be useful for understanding which actors are being
	 * impacted by the area of effect behavior during development or testing.
	 * Configurable only in the editor under the "GASCourse|AreaOfEffect" category.
	 */
	UPROPERTY(EditDefaultsOnly, Category="GASCourse|AreaOfEffect|Debug")
	bool bDrawAffectedActors = false;

	/**
	 * Specifies whether the numerical value of the effect (e.g., damage or healing) applied
	 * to each actor within the area of effect should be displayed for debugging purposes.
	 * Configurable only in the editor and categorized under "GASCourse|AreaOfEffect|Debug".
	 */
	UPROPERTY(EditDefaultsOnly, Category="GASCourse|AreaOfEffect|Debug|AffectedActors")
	bool bDrawNumericalValueOfEffectOnActor = false;

	/**
	 * Debug color used to visually represent actors that have been damaged within
	 * the area of effect during gameplay debugging. Configurable for better clarity
	 * in visual debug tools.
	 */
	UPROPERTY(EditDefaultsOnly, Category="GASCourse|AreaOfEffect|Debug|AffectedActors")
	FColor DamagedActorsDebugColor = FColor::Red;

	/**
	 * Color used for debugging purposes to visually represent actors that have been healed
	 * within the area of effect during gameplay. Configurable in the editor for customization.
	 */
	UPROPERTY(EditDefaultsOnly, Category="GASCourse|AreaOfEffect|Debug|AffectedActors")
	FColor HealedActorsDebugColor = FColor::Green;

	

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif

protected:

	/**
 * Handles the completion of a targeting request for the projectile.
 * Processes the targeting results to determine the next action for the projectile, including
 * homing behavior, collision adjustments, or destruction.
 *
 * @param TargetingRequestHandle The handle associated with the completed targeting request, used to
 * retrieve and manage the targeting results.
 */
	UFUNCTION()
	void OnTargetRequestCompleted(FTargetingRequestHandle TargetingRequestHandle);

private:
	/**
	 * Handle representing the current targeting request in progress,
	 * used to manage and track targeting operations within the system.
	 * Provides a unique reference to the active targeting session.
	 */
	UPROPERTY()
	FTargetingRequestHandle CurrentTargetHandle;

	/**
	 * Reference to the actor responsible for initiating the area of effect.
	 * Used to establish the source of the effect and potentially apply associated logic
	 * such as damage, buffs, or debuffs originating from the instigator.
	 */
	UPROPERTY()
	AActor* AreaOfEffectInstigatorActor = nullptr;

	/**
	 * Draws a debug visualization of the area of effect (AoE) shape for a given targeting task.
	 * The debug shape is rendered according to the specified collision shape and parameters.
	 *
	 * @param InAoETask The targeting task object responsible for managing AoE targeting behavior.
	 * @param TargetingHandle A handle that identifies the targeting request and contains relevant data.
	 * @param InAoEShape The collision shape that defines the area of effect, determining the type and dimensions of the debug shape to be drawn.
	 */
	void DrawAoEDebugShape(const UTargetingTask* InAoETask, const FTargetingRequestHandle& TargetingHandle, FCollisionShape InAoEShape);

	/**
	 * Retrieves the UWorld object associated with the provided targeting handle.
	 * This method checks for the source context of the targeting handle and attempts
	 * to derive the world from the source actor or instigator actor. If no valid world
	 * can be resolved, it returns the world associated with the current object.
	 *
	 * @param TargetingHandle The handle associated with a targeting request, containing
	 * context information for identifying the relevant world instance.
	 * @return A pointer to the UWorld object associated with the targeting handle, or
	 * the default world of the current object if the handle does not resolve to a valid context.
	 */
	const UWorld* GetWorldFromTargetHandle(const FTargetingRequestHandle& TargetingHandle);

	/**
	 * Calculates and retrieves the source location for the area of effect (AoE)
	 * based on the provided targeting handle.
	 * The location is determined by the associated source actor, if available,
	 * or a predefined source location. Defaults to a zero vector if no valid source context is found.
	 *
	 * @param TargetingHandle The targeting request handle used to identify
	 *        the source context for determining the AoE location.
	 * @return The FVector representing the source location of the AoE.
	 */
	FVector GetSourceAoELocation(const FTargetingRequestHandle& TargetingHandle);

	/**
	 * Retrieves the rotation (in quaternion form) of the source associated with the targeting request handle.
	 * If a valid source context and source actor are found for the provided targeting handle, returns the actor's rotation.
	 * Returns an identity quaternion if no valid source context or actor is found.
	 *
	 * @param TargetingHandle The handle representing the targeting request used to retrieve the source's rotation.
	 * @return The rotation of the source actor as a quaternion, or an identity quaternion if no valid source is found.
	 */
	FQuat GetSourceAoERotation(const FTargetingRequestHandle& TargetingHandle) const;

	/**
	 * Applies the area of effect functionality on a set of target actors based on the
	 * defined area of effect type and configuration. Executes appropriate effects,
	 * such as damage, on each actor within the target list.
	 *
	 * @param InInstigator The actor responsible for initiating the area of effect.
	 * @param TargetActors Array of actors that are within the area of effect and will be affected.
	 * @param TargetingRequestHandle A handle that represents the targeting request associated with this action.
	 * @return true if the area of effect is successfully applied to all target actors, false otherwise.
	 */
	bool ApplyAreaOfEffectOnActors(AActor* InInstigator, TArray<AActor*> TargetActors, FTargetingRequestHandle TargetingRequestHandle);

	/**
	 * Calculates and processes the falloff value for a target actor within the area of effect.
	 * Determines the distance between the effect's center and the target actor's location
	 * to apply any relevant falloff adjustments to the effect's value.
	 *
	 * @param TargetingRequestHandle A handle containing data and references related to the targeting request.
	 * @param InTargetActor A pointer to the target actor within the area of effect whose value needs to be processed.
	 * @return A float representing the processed value after applying any falloff adjustments based on distance.
	 */
	float ProcessAreaOfEffectValueFallOffOnTarget(FTargetingRequestHandle TargetingRequestHandle, const AActor* InTargetActor);

	/**
	 * Retrieves the area-of-effect (AOE) targeting selection task from the specified targeting preset.
	 * Searches the targeting task set within the given preset and returns the first compatible
	 * AOE selection task if found.
	 *
	 * @param InTargetingPreset The targeting preset containing the task set to search for AOE tasks.
	 * @return A pointer to the UGASC_TargetingSelectionTask_AOE if found, or nullptr if no matching task exists.
	 */
	const UGASC_TargetingSelectionTask_AOE* GetAOESelectionTask(const UTargetingPreset* InTargetingPreset);

	/**
	 * Retrieves the maximum distance from the center of the area of effect (AoE) based on the configured
	 * targeting preset and shape properties.
	 * The method evaluates the shape of the AoE (Box, Sphere, Capsule, or Cylinder) and computes
	 * the corresponding maximum distance based on the shape's configuration.
	 *
	 * @return The maximum distance from the center of the AoE in the respective measurement unit,
	 *         or 0.0f if the targeting configuration is not valid or unknown.
	 */
	float GetMaxDistanceFromCenter();
	
};
