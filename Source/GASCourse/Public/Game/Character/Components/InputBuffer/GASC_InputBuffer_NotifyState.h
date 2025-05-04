// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "GASC_InputBuffer_NotifyState.generated.h"

/**
 */
UCLASS()
class GASCOURSE_API UGASC_InputBuffer_NotifyState : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	
	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference);
	virtual void NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference);
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, const FAnimNotifyEventReference& EventReference);

	virtual FString GetNotifyName_Implementation() const override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="InputBuffer")
	FGameplayTagContainer BlockingTags;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="InputBuffer", meta=(ClampMin=0.0f, ClampMax=1.0f))
	float OpenInputBufferTimePercentage = 0.0f;
};
