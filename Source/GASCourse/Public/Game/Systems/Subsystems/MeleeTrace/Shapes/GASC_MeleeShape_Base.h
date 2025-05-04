// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CollisionQueryParams.h"
#include "CollisionShape.h"
#include "UObject/Object.h"
#include "GASC_MeleeShape_Base.generated.h"

/**
 * UGASC_MeleeShape_Base serves as a base class for defining melee shapes
 * used for collision or trace-based melee attacks in the game. This class
 * is designed to be extended to implement specific melee shape logic.
 */
UCLASS(Abstract, BlueprintType, EditInlineNew, DefaultToInstanced)
class GASCOURSE_API UGASC_MeleeShape_Base : public UObject
{
	GENERATED_BODY()
	
public:
	
	virtual FCollisionShape CreateCollisionShape() const;
	virtual FQuat GetRotationOffset() const;
	
};

UCLASS(Blueprintable)
class UGASC_MeleeShape_Sphere : public UGASC_MeleeShape_Base
{
	GENERATED_BODY()

public:
	
	virtual FCollisionShape CreateCollisionShape() const override;

	UFUNCTION(BlueprintPure, Category = "Melee Trace")
	static UGASC_MeleeShape_Sphere* MakeSphereShape(float Radius);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Melee Trace")
	float Radius = 1.0f;
	
};

UCLASS(Blueprintable)
class UGASC_MeleeShape_Capsule : public UGASC_MeleeShape_Base
{
	GENERATED_BODY()

public:
	
	virtual FCollisionShape CreateCollisionShape() const override;

	UFUNCTION(BlueprintPure, Category = "Melee Trace")
	static UGASC_MeleeShape_Capsule* MakeCapsuleShape(float HalfHeight, float Radius, const FRotator& RotationOffset);

	virtual FQuat GetRotationOffset() const override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Melee Trace")
	float HalfHeight = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Melee Trace")
	float Radius = 1.0f;
	
	FRotator RotationOffset;
};

UCLASS(Blueprintable)
class UGASC_MeleeShape_Box : public UGASC_MeleeShape_Base
{
	GENERATED_BODY()

public:
	
	virtual FCollisionShape CreateCollisionShape() const override;

	UFUNCTION(BlueprintPure, Category = "Melee Trace")
	static UGASC_MeleeShape_Box* MakeBoxShape(FVector BoxExtent, const FRotator& RotationOffset);

	virtual FQuat GetRotationOffset() const override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Melee Trace")
	FVector BoxExtent;
	
	FRotator RotationOffset;
};

