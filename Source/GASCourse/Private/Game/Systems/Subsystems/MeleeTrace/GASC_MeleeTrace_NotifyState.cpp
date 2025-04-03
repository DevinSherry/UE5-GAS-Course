// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Systems/Subsystems/MeleeTrace/GASC_MeleeTrace_NotifyState.h"
#include "Game/Systems/Subsystems/MeleeTrace/GASC_MeleeTrace_Subsystem.h"
#include "DrawDebugHelpers.h"


void UGASC_MeleeTrace_NotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
#if WITH_EDITOR
	if (GEditor)
	{
		UWorld* EditorWorld = GEditor->GetEditorWorldContext().World();
		UGASC_MeleeTrace_Subsystem* MeleeTrace_Subsystem = EditorWorld->GetSubsystem<UGASC_MeleeTrace_Subsystem>();
		if (MeleeTrace_Subsystem && bShouldFireInEditor)
		{
			MeleeTraceNotifyID = FGuid::NewGuid();
			MeleeTrace_Subsystem->RequestSphereMeleeTrace(MeshComp->GetOwner(), TraceSphereRadius, TraceSphereSocketName, ObjectTypes, bUseWeaponMesh, MeleeTraceNotifyID);
		}
	}
#endif
	
	if (UWorld* NotifyWorld = MeshComp->GetOwner()->GetWorld())
	{
		UGASC_MeleeTrace_Subsystem* MeleeTrace_Subsystem = NotifyWorld->GetSubsystem<UGASC_MeleeTrace_Subsystem>();
		if (MeleeTrace_Subsystem)
		{
			MeleeTraceNotifyID = FGuid::NewGuid();
			MeleeTrace_Subsystem->RequestSphereMeleeTrace(MeshComp->GetOwner(), TraceSphereRadius,
				TraceSphereSocketName, ObjectTypes, bUseWeaponMesh, MeleeTraceNotifyID);
		}
	}
	
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
}

void UGASC_MeleeTrace_NotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
#if WITH_EDITOR
	if (GEditor)
	{
		UWorld* EditorWorld = GEditor->GetEditorWorldContext().World();
		UGASC_MeleeTrace_Subsystem* MeleeTrace_Subsystem = EditorWorld->GetSubsystem<UGASC_MeleeTrace_Subsystem>();
		if (MeleeTrace_Subsystem && bShouldFireInEditor)
		{
			MeleeTrace_Subsystem->InEditor_DrawSphereMeleeTrace(MeshComp, TraceSphereRadius, TraceSphereSocketName);
		}
	}
#endif
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
}

void UGASC_MeleeTrace_NotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	if (UWorld* NotifyWorld = MeshComp->GetOwner()->GetWorld())
	{
		UGASC_MeleeTrace_Subsystem* MeleeTrace_Subsystem = NotifyWorld->GetSubsystem<UGASC_MeleeTrace_Subsystem>();
		if (MeleeTrace_Subsystem)
		{
			if (MeleeTraceNotifyID.IsValid() && MeleeTrace_Subsystem->IsMeleeTraceInProgress(MeleeTraceNotifyID))
			{
				MeleeTrace_Subsystem->CancelMeleeTrace(MeleeTraceNotifyID);
			}
		}
	}
	
	Super::NotifyEnd(MeshComp, Animation, EventReference);
}
