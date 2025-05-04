// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GASC_MeleeTrace_Subsystem.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Shapes/GASC_MeleeShape_Base.h"
#include "GASC_MeleeTrace_NotifyState.generated.h"

/**
 * UGASC_MeleeTrace_NotifyState is a custom animation notify state that facilitates melee tracing during animation notifications.
 * It is designed to interact with the melee trace subsystem for handling melee hit detection using customizable trace shapes.
 */
UCLASS()
class GASCOURSE_API UGASC_MeleeTrace_NotifyState : public UAnimNotifyState
{
	GENERATED_BODY()
	
	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference);
	virtual void NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference);
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, const FAnimNotifyEventReference& EventReference);

public:

	/*
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GASC_MeleeTrace_NotifyState")
	FGASC_MeleeTrace_Subsystem_Data MeleeTraceSubsystemData;
	*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee Trace", meta = (DataTable = "FGASC_MeleeTrace_TraceShapeData"))
	FDataTableRowHandle MeleeTraceRowHandle;
private:

	FGuid MeleeTraceNotifyID;
	FGASC_MeleeTrace_Subsystem_Data MeleeTraceSubsystemData;

	FGASC_MeleeTrace_Subsystem_Data CreateShapeDataFromRow(FGASC_MeleeTrace_TraceShapeData RowData);

#if WITH_EDITORONLY_DATA
	TWeakObjectPtr<UMeshComponent> DebugMeshComponent = nullptr;
	TArray<FVector> PreviousFrameSamples;
#endif
};
