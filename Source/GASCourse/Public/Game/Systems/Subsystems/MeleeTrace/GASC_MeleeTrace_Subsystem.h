// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Subsystems/WorldSubsystem.h"
#include "Misc/Guid.h"
#include "GASC_MeleeTrace_Subsystem.generated.h"

UENUM(BlueprintType)
enum class ETraceShape : uint8
{
	Box     UMETA(DisplayName = "Box"),
	Capsule UMETA(DisplayName = "Capsule"),
	Sphere  UMETA(DisplayName = "Sphere"),
	Line    UMETA(DisplayName = "Line")
};

USTRUCT(BlueprintType)
struct FGASC_MeleeTrace_Subsystem_Data
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MeleeTrace")
	AActor* InstigatorActor = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MeleeTrace")
	ETraceShape TraceShape = ETraceShape::Sphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MeleeTrace")
	float TraceRadius = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MeleeTrace")
	FVector TraceBoxExtent = FVector(0);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MeleeTrace")
	float CapsuleHalfHeight = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MeleeTrace")
	float CapsuleRadius = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MeleeTrace")
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MeleeTrace")
	FName TraceSocketStart = FName(TEXT(""));

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MeleeTrace")
	UStaticMeshComponent* WeaponMesh = nullptr;

	FGuid TraceId;
	FVector PreviousPosition = FVector::ZeroVector;
	FVector CurrentPosition = FVector::ZeroVector;
	int32 TraceCount = 0;

	FGASC_MeleeTrace_Subsystem_Data() {}

	FGASC_MeleeTrace_Subsystem_Data(AActor* InInstigator, ETraceShape InTraceShape, float InTraceRadius, FVector InTraceBoxExtent, float InCapsuleHalfHeight, float InCapsuleRadius, TArray<EObjectTypeQuery> InObjectTypes, FName InTraceSocketStart)
		: InstigatorActor(InInstigator)
		, TraceShape(InTraceShape)
		, TraceRadius(InTraceRadius)
		, TraceBoxExtent(InTraceBoxExtent)
		, CapsuleHalfHeight(InCapsuleHalfHeight)
		, CapsuleRadius(InCapsuleRadius)
		, ObjectTypes(InObjectTypes)
		, TraceSocketStart(InTraceSocketStart)
	{
	}
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

#if WITH_EDITOR
	UFUNCTION(BlueprintCallable, Category="GASCourse|MeleeTrace")
	void InEditor_DrawSphereMeleeTrace(USkeletalMeshComponent* SkeletalMeshComponent, float inRadius, FName inTraceSocketStart);
#endif // WITH_EDITOR

	UFUNCTION(BlueprintCallable, Category="GASCourse|MeleeTrace")
	void RequestSphereMeleeTrace(AActor* Instigator, float inRadius, FName inTraceSocketStart, TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes, bool bUseWeapon, FGuid TraceId);

	UFUNCTION(BlueprintCallable, Category="GASCourse|MeleeTrace")
	bool IsMeleeTraceInProgress(FGuid TraceId);

	UFUNCTION(BlueprintCallable, Category="GASCourse|MeleeTrace")
	bool CancelMeleeTrace(FGuid TraceId);

	virtual TStatId GetStatId() const override
	{
		return GetStatID();
	}

private:
	
	TArray<FGASC_MeleeTrace_Subsystem_Data> MeleeTraceRequests;
	FGASC_MeleeTrace_Subsystem_Data* ActiveMeleeTraceRequest;

	void ProcessMeleeTraces(float DeltaTime);

	FVector GetSocketTraceLocationFromMesh(AActor* Instigator, FName TraceStartSocketName, FName TraceEndSocketName) const;

	const float TraceGranularity = 10.0f;
};
