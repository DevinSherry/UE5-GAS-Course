// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Abilities/Tasks/AbilityTask.h"
#include "UObject/ObjectMacros.h"
#include "GASCourse_WaitOverlapTask.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGASCourseWaitOverlapDelegate, const FGameplayAbilityTargetDataHandle&, TargetData);

class AActor;
class USphereComponent;

/**
 * 
 */
UCLASS()
class GASCOURSE_API UGASCourse_WaitOverlapTask : public UAbilityTask
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(BlueprintAssignable)
	FGASCourseWaitOverlapDelegate	OnOverlap;

	UFUNCTION()
	void OnComponentOverlapCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	virtual void Activate() override;

	/** Wait until an overlap occurs. This will need to be better fleshed out so we can specify game specific collision requirements */
	UFUNCTION(BlueprintCallable, Category="Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UGASCourse_WaitOverlapTask* WaitForOverlap(UGameplayAbility* OwningAbility, float InSphereRadius = 100.0f, bool bInDebugDraw = false);

private:

	virtual void OnDestroy(bool AbilityEnded) override;

	bool InitializeOverlapComponent(); 

	UPROPERTY()
	USphereComponent* OverlapComponent = nullptr;
	
	float SphereRadius = 0.0f;
	bool bDebugDraw = false;

	UPROPERTY()
	TArray<AActor*> OverlappedActors;
};
