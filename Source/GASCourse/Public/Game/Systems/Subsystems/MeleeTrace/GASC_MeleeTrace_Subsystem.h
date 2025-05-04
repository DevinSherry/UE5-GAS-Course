// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Subsystems/WorldSubsystem.h"
#include "GASCourse/Public/Game/Systems/Subsystems/MeleeTrace/Shapes/GASC_MeleeShape_Base.h"
#include "Misc/Guid.h"
#include "CollisionShape.h"
#include "Engine/Engine.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GASC_MeleeTrace_Subsystem.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LOG_GASC_MeleeTraceSubsystem, Log, All);

UENUM(BlueprintType)
enum class EGASC_MeleeTrace_TraceShape : uint8
{
	Box,
	Capsule,
	Sphere 
};

USTRUCT(BlueprintType)
struct FGASC_MeleeTrace_TraceShapeData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Melee Trace|Shape Data")
	EGASC_MeleeTrace_TraceShape TraceShape = EGASC_MeleeTrace_TraceShape::Box;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Melee Trace|Shape Data", meta=(EditCondition = "TraceShape == EGASC_MeleeTrace_TraceShape::Sphere"))
	float SphereRadius = 30.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Melee Trace|Shape Data", meta=(EditCondition = "TraceShape == EGASC_MeleeTrace_TraceShape::Capsule"))
	float CapsuleRadius = 30.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Melee Trace|Shape Data", meta=(EditCondition = "TraceShape == EGASC_MeleeTrace_TraceShape::Capsule"))
	float CapsuleHeight = 30.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Melee Trace|Shape Data", meta=(EditCondition = "TraceShape == EGASC_MeleeTrace_TraceShape::Box"))
	FVector BoxExtent = FVector::Zero();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Melee Trace|Socket Data")
	FName StartSocket = FName("WeaponTrace_Start");

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Melee Trace|Socket Data")
	FName EndSocket = FName("WeaponTrace_End");

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Melee Trace|Socket Data")
	int32 TraceDensity = 2;
	
};

/**
 * FGASC_MeleeTrace_Subsystem_Data is a data structure used for configuring and handling melee trace functionality.
 * It encapsulates configuration details required for performing melee traces, including trace shape, trace sockets,
 * trace density, and runtime metadata such as source mesh component, instigator actor, and collision data.
 *
 * This structure serves as the core data exchanged and processed by the melee trace subsystem, enabling precise
 * collision detection and multi-frame sample tracking during melee interactions.
 */
USTRUCT(BlueprintType)
struct FGASC_MeleeTrace_Subsystem_Data
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Instanced, Category = "Melee Trace")
	TObjectPtr<UGASC_MeleeShape_Base> TraceShape;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Melee Trace")
	FName TraceSocket_Start = FName("");

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Melee Trace")
	FName TraceSocket_End = FName("");

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Melee Trace")
	int32 TraceDensity = 1;

	TWeakObjectPtr<UMeshComponent> SourceMeshComponent = nullptr;
	AActor* InstigatorActor = nullptr;
	FCollisionShape TraceCollisionShape;
	TArray<FVector> PreviousFrameSamples;
	TArray<AActor*> HitActors;
	TArray<AActor*> HitActors_PreviousFrames;
	TArray<FHitResult> HitResults_PreviousFrames;
	
	FGuid TraceId;

	FGASC_MeleeTrace_Subsystem_Data() {}
};

/**
 * UGASC_MeleeTrace_Subsystem is a tickable world subsystem responsible for handling and managing melee trace requests.
 * This subsystem processes queued melee trace data and executes the necessary functionality each tick cycle.
 */
UCLASS()
class GASCOURSE_API UGASC_MeleeTrace_Subsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()
	
public:

	virtual void Tick(float DeltaTime) override;

	// USubsystem implementation Begin
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

	virtual void OnWorldBeginPlay(UWorld& InWorld) override;

	void DrawDebugMeleeTrace(const UObject* WorldContextObject, const FCollisionShape& MeleeTraceShape, const FTransform& Start, const FTransform& End,
		bool bHit, const TArray<FHitResult>& HitResults);

	void DrawDebugSphereTraceMulti(const UWorld* World,
		const FVector& Start,
		const FVector& End,
		float Radius,
		EDrawDebugTrace::Type DrawDebugType,
		bool bHit,
		const TArray<FHitResult>& Hits,
		const FLinearColor& TraceColor,
		const FLinearColor& TraceHitColor,
		float DrawTime);
	
	void DrawDebugSweptSphere(const UWorld* InWorld,
		FVector const& Start,
		FVector const& End,
		float Radius,
		FColor const& Color,
		bool bPersistentLines,
		float LifeTime,
		uint8 DepthPriority = 0);

	void DrawDebugCapsuleTraceMulti(const UWorld* World,
	const FVector& Start,
	const FVector& End,
	const FQuat& Orientation,
	float Radius,
	float HalfHeight,
	EDrawDebugTrace::Type DrawDebugType,
	bool bHit,
	const TArray<FHitResult>& HitResults,
	const FLinearColor& TraceColor,
	const FLinearColor& TraceHitColor,
	float DrawTime);

	void DrawDebugBoxTraceMulti(const UWorld* World,
	const FVector& Start,
	const FVector& End,
	const FRotator& Orientation,
	const FVector& BoxExtent,
	EDrawDebugTrace::Type DrawDebugType,
	bool bHit,
	const TArray<FHitResult>& HitResults,
	const FLinearColor& TraceColor,
	const FLinearColor& TraceHitColor,
	float DrawTime);

	void DrawDebugSweptBox(const UWorld* InWorld,
	FVector const& Start,
	FVector const& End,
	FRotator const& Orientation,
	FVector const& HalfSize,
	FColor const& Color,
	bool bPersistentLines,
	float LifeTime,
	uint8 DepthPriority = 0);

	UFUNCTION(BlueprintCallable, Category="GASCourse|MeleeTrace")
	void RequestShapeMeleeTrace(AActor* Instigator, FGASC_MeleeTrace_Subsystem_Data TraceData, FGuid TraceId);

	UFUNCTION(BlueprintCallable, Category="GASCourse|MeleeTrace")
	bool IsMeleeTraceInProgress(FGuid TraceId);

	FGASC_MeleeTrace_Subsystem_Data CreateShapeDataFromRow(FGASC_MeleeTrace_TraceShapeData RowData);

	UFUNCTION(BlueprintCallable, Category="GASCourse|MeleeTrace")
	bool CancelMeleeTrace(FGuid TraceId);

	UFUNCTION()
	TWeakObjectPtr<UMeshComponent> GetMeshComponent(AActor* Actor, const FName& StartSocketName, const FName& EndSocketName);

	virtual TStatId GetStatId() const override
	{
		return GetStatID();
	}

	static void GetTraceSamples(const UMeshComponent* MeshComponent,
	int32 TraceDensity,
	const FName& StartSocketName,
	const FName& EndSocketName,
	TArray<FVector>& OutSamples);

private:
	
	TArray<FGASC_MeleeTrace_Subsystem_Data> MeleeTraceRequests;

	void ProcessMeleeTraces(float DeltaTime);

	FCollisionObjectQueryParams ConfigureCollisionObjectParams(const TArray<TEnumAsByte<EObjectTypeQuery> > & ObjectTypes);
	
};