// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tasks/TargetingSelectionTask_AOE.h"
#include "GASC_TargetingSelectionTask_AOE.generated.h"

/**
 * Represents a custom area of effect (AOE) targeting selection task used to define and handle
 * specific AOE targeting shapes such as Box, Cylinder, Sphere, or Capsule.
 * Provides methods to retrieve the AOE shape type and generate the associated collision shape.
 */
UCLASS()
class GASCOURSE_API UGASC_TargetingSelectionTask_AOE : public UTargetingSelectionTask_AOE
{
	GENERATED_BODY()
	
public:
	/**
	 * Retrieves the collision shape based on the area of effect (AOE) targeting task's shape type.
	 * The shape is determined by the stored shape type (e.g., Box, Cylinder, Sphere, Capsule)
	 * and associated dimensions such as radius, half-extent, or height.
	 *
	 * @return An FCollisionShape object representing the AOE targeting collision shape.
	 *         Returns an empty FCollisionShape if the shape type is invalid.
	 */
	FCollisionShape GetCollisionShapeFromTask() const;

	/**
	 * Retrieves the type of area of effect (AOE) shape selected for the targeting task.
	 * The shape type can determine forms such as Box, Cylinder, Sphere, or other predefined geometries.
	 *
	 * @return An ETargetingAOEShape value representing the chosen AOE shape type.
	 */
	ETargetingAOEShape GetTargetingAOEShape() const;

	FVector GetAoESourceOffset(const FTargetingRequestHandle& TargetingHandle) const;

	/**
	 * Sets the collision channel to be used for the area of effect (AOE) targeting selection task.
	 * The collision channel determines which objects the targeting task will interact with or ignore
	 * during collision detection.
	 *
	 * @param InCollisionChannel The collision channel to set for the AOE targeting task,
	 *                           represented as an ECollisionChannel enum value.
	 */
	FORCEINLINE void SetCollisionCollisionChannel(TEnumAsByte<ECollisionChannel> InCollisionChannel)
	{
		CollisionChannel = InCollisionChannel;
	};

	/**
	 * Specifies whether the source actor should be ignored during the area of effect (AOE)
	 * targeting selection process.
	 *
	 * @param bInIgnoreSourceActor A boolean value indicating whether the source actor should be ignored.
	 *                             If true, the source actor will be excluded from the targeting results.
	 */
	FORCEINLINE void SetIgnoreSourceActor(bool bInIgnoreSourceActor)
	{
		bIgnoreSourceActor = bInIgnoreSourceActor;
	}

	/**
	 * Determines whether the instigator actor should be ignored during the area of effect (AOE)
	 * targeting selection process.
	 *
	 * @param bInIgnoreInstigatorActor A boolean value indicating whether the instigator actor
	 *                                 should be ignored. If true, the instigator actor will
	 *                                 not be included in the targeting results.
	 */
	FORCEINLINE void SetIgnoreInstigatorActor(bool bInIgnoreInstigatorActor)
	{
		bIgnoreInstigatorActor = bInIgnoreInstigatorActor;
	}

	/**
	 * Adds a new collision object type to the area of effect (AOE) targeting selection task.
	 * This specifies which object types the targeting task should consider during collision detection.
	 *
	 * @param InObjectTypeToAdd The EObjectTypeQuery value representing the object type to add to
	 *                          the list of collision object types considered by this task.
	 */
	void AddCollisionObjectTypes(TEnumAsByte<EObjectTypeQuery> InObjectTypeToAdd);
	
};
