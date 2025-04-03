// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "GASC_MeleeTrace_NotifyState.generated.h"

/**
 * 
 */
UCLASS()
class GASCOURSE_API UGASC_MeleeTrace_NotifyState : public UAnimNotifyState
{
	GENERATED_BODY()
	
	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference);
	virtual void NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference);
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, const FAnimNotifyEventReference& EventReference);

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GASC_MeleeTrace_NotifyState")
	float TraceSphereRadius = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GASC_MeleeTrace_NotifyState")
	FName TraceSphereSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GASC_MeleeTrace_NotifyState")
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GASC_MeleeTrace_NotifyState")
	bool bUseWeaponMesh = false;
	
private:

	FGuid MeleeTraceNotifyID;
};
