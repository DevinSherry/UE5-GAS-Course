// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "GASC_TimeWarp_NotifyState.generated.h"

/**
 * 
 */
UCLASS()
class GASCOURSE_API UGASC_TimeWarp_NotifyState : public UAnimNotifyState
{
	GENERATED_BODY()

	UGASC_TimeWarp_NotifyState();
	
	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference);
	virtual void NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference);
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, const FAnimNotifyEventReference& EventReference);
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Curve")
	FRuntimeFloatCurve TimeWarpCurve;

private:

	float Duration = 0.0f;
	bool bTimeWarpEnabled = false;
	float DefaultPlayRate = 1.0f;

	UPROPERTY()
	UAnimInstance* AnimInstance = nullptr;

	UPROPERTY()
	UAnimMontage* AnimMontage = nullptr;
};
