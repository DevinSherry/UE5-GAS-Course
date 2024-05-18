// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GASCourse/GASCourseCharacter.h"
#include "Abilities/GameplayAbilityTargetActor_Trace.h"
#include "GASCourseTargetActor_Trace.generated.h"

/**
 * 
 */

USTRUCT(Blueprintable)
struct FTargetingOutline
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "GASCourse|Targeting|Outline")
	bool bEnableTargetingOutline;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "GASCourse|Targeting|Outline")
	FLinearColor OutlineColor;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "GASCourse|Targeting|Outline")
	TSubclassOf<AGASCourseCharacter> CharacterClassToOutline;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GASCourse|Targeting|Outline")
	UMaterialInterface* OutlineMaterial;

	FTargetingOutline()
	{
		bEnableTargetingOutline = true;
		OutlineColor = FColor::Red;
		CharacterClassToOutline = nullptr;
		OutlineMaterial = nullptr;
	}
	
};
UCLASS(Abstract, Blueprintable, notplaceable, config=Game, HideCategories=Trace)
class GASCOURSE_API AGASCourseTargetActor_Trace : public AGameplayAbilityTargetActor
{
	GENERATED_UCLASS_BODY()
	
public:
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void CancelTargeting() override;

	virtual void ConfirmTargeting() override;

	virtual bool IsNetRelevantFor(const AActor* RealViewer, const AActor* ViewTarget, const FVector& SrcLocation) const override;

	/** Traces as normal, but will manually filter all hit actors */
	static void LineTraceWithFilter(FHitResult& OutHitResult, const UWorld* World, const FGameplayTargetDataFilterHandle FilterHandle, const FVector& Start, const FVector& End, ECollisionChannel CollisionChannel, const FCollisionQueryParams Params);

	/** Sweeps as normal, but will manually filter all hit actors */
	static void SweepWithFilter(FHitResult& OutHitResult, const UWorld* World, const FGameplayTargetDataFilterHandle FilterHandle, const FVector& Start, const FVector& End, const FQuat& Rotation, const FCollisionShape CollisionShape, FName ProfileName, const FCollisionQueryParams Params);

	void AimWithPlayerController(const AActor* InSourceActor, FCollisionQueryParams Params, const FVector& TraceStart, FVector& OutTraceEnd, bool bIgnorePitch = false) const;

	static bool ClipCameraRayToAbilityRange(FVector CameraLocation, FVector CameraDirection, FVector AbilityCenter, float AbilityRange, FVector& ClippedPosition);

	virtual void StartTargeting(UGameplayAbility* Ability) override;

	virtual void ConfirmTargetingAndContinue() override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void ShowMouseCursor(bool bShowCursor);

	//Range of the required trace to hit the ground/landscepe
	float MaxRange;

	//Profile FName to detect the ground
	UPROPERTY(BlueprintReadOnly, meta = (ExposeOnSpawn = true), Category = Targeting)
	TEnumAsByte<ECollisionChannel> TraceChannel;

	TArray<TWeakObjectPtr<AActor>> ActorsToOutline;

protected:
	
	virtual FHitResult PerformTrace(AActor* InSourceActor) PURE_VIRTUAL(AGameplayAbilityTargetActor_Trace, return FHitResult(););

	FGameplayAbilityTargetDataHandle MakeTargetData(const FHitResult& HitResult) const;

	TWeakObjectPtr<AGameplayAbilityWorldReticle> ReticleActor;

	virtual void UpdateLooseGameplayTagsDuringTargeting(FGameplayTag InGameplayTag, int32 InCount);

	virtual void DrawTargetOutline(TArray<TWeakObjectPtr<AActor> > InHitActors,  TArray<TWeakObjectPtr<AActor>> InLatestHitActors);
	virtual void ClearTargetOutline(TArray<TWeakObjectPtr<AActor> > InHitActors);

	virtual void SendTargetDataBacktoServer(const FGameplayAbilityTargetDataHandle& InData, FGameplayTag ApplicationTag);

	virtual void SendCancelledTargetDataBackToServer(const FGameplayAbilityTargetDataHandle& InData);

protected:

	FGameplayTagContainer DefaultTargetingTagContainer;

	int StoredCustomDepthStencilValue = 0;
	bool bHasDataBeenSentToServer = false;
	
};
