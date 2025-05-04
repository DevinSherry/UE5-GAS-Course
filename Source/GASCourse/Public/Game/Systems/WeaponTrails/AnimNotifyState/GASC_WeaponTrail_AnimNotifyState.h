// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "GASCourse/Public/Game/Systems/WeaponTrails/GASC_WeaponTrails_DataAsset.h"
#include "GASC_WeaponTrail_AnimNotifyState.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogGASCourseWeaponTrail, Log, All);

/**
 * @class UGASC_WeaponTrail_AnimNotifyState
 * @brief Handles the functionality for creating and managing weapon trail effects during animations.
 *
 * This class represents an animation notify state used to manage the lifecycle of a Niagara-based
 * weapon trail effect. It allows for specifying the socket, offsets, and additional parameters to
 * spawn and control a Niagara component attached to a skeletal mesh component.
 *
 * It is highly customizable and supports options like socket attachment, offset adjustments, and
 * whether to destroy or stop the effect at the end of the notify.
 */
UCLASS(Blueprintable, meta = (DisplayName = "Weapon Trail"))
class GASCOURSE_API UGASC_WeaponTrail_AnimNotifyState : public UAnimNotifyState
{
	GENERATED_UCLASS_BODY()

	// The socket within our mesh component to attach to when we spawn the Niagara component
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponTrailData)
	UGASC_WeaponTrails_DataAsset* WeaponTrailData = nullptr;
	
	// The socket within our mesh component to attach to when we spawn the Niagara component
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = NiagaraSystem, meta = (ToolTip = "The socket or bone to attach the system to", AnimNotifyBoneName = "true"))
	FName SocketName;

	// Offset from the socket / bone location
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = NiagaraSystem, meta = (ToolTip = "Offset from the socket or bone to place the Niagara system"))
	FVector LocationOffset;

	// Offset from the socket / bone rotation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = NiagaraSystem, meta = (ToolTip = "Rotation offset from the socket or bone for the Niagara system"))
	FRotator RotationOffset;

	// Whether or not we destroy the component at the end of the notify or instead just stop
	// the emitters.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = NiagaraSystem, meta = (DisplayName = "Destroy Immediately", ToolTip = "Whether the Niagara system should be immediately destroyed at the end of the notify state or be allowed to finish"))
	bool bDestroyAtEnd;

	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference);
	virtual void NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference);
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, const FAnimNotifyEventReference& EventReference);

	// Return FXSystemComponent created from SpawnEffect
	UFUNCTION(BlueprintCallable, Category = "AnimNotify")
	UNiagaraComponent* GetSpawnedEffect(UMeshComponent* MeshComp);

private:

	UPROPERTY()
	UNiagaraComponent* WeaponTrailNiagaraComponent = nullptr;

	void SetWeaponTrailMaterialInterface(UNiagaraComponent* InWeaponTrailNiagaraComponent) const;
	void SetWeaponTrailColorArrayAtTime(UNiagaraComponent* InWeaponTrailNiagaraComponent, float InTime) const;
	void SetWeaponTrailLifeTime(UNiagaraComponent* InWeaponTrailNiagaraComponent, float InLifeTime) const;

	void DestroyWeaponTrailVFX() const;
};
