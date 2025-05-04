// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Systems/WeaponTrails/AnimNotifyState/GASC_WeaponTrail_AnimNotifyState.h"
#include "NiagaraComponent.h"
#include "NiagaraDataInterfaceArrayFunctionLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "Animation/AnimNotifyLibrary.h"
#include "Curves/CurveLinearColor.h"
#include "Game/Systems/WeaponTrails/GASC_WeaponTrails_DataAsset.h"

DEFINE_LOG_CATEGORY(LogGASCourseWeaponTrail);

UGASC_WeaponTrail_AnimNotifyState::UGASC_WeaponTrail_AnimNotifyState(const FObjectInitializer& ObjectInitializer)
{
	LocationOffset.Set(0.0f, 0.0f, 0.0f);
	RotationOffset = FRotator(0.0f, 0.0f, 0.0f);
	bDestroyAtEnd = false;
}

void UGASC_WeaponTrail_AnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                                    float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	if (!WeaponTrailData->IsValidLowLevelFast())
	{
		UE_LOG(LogGASCourseWeaponTrail, Warning, TEXT("Invalid Weapon Trail data found in %s. Please fix."), *GetPathNameSafe(this));
		return;
	}
	
	WeaponTrailNiagaraComponent = GetSpawnedEffect(MeshComp);
	if (WeaponTrailNiagaraComponent)
	{
		SetWeaponTrailMaterialInterface(WeaponTrailNiagaraComponent);
		SetWeaponTrailColorArrayAtTime(WeaponTrailNiagaraComponent, 0.0f);
		SetWeaponTrailLifeTime(WeaponTrailNiagaraComponent, WeaponTrailData->TrailLifeTime);
		
	}
	
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
}

void UGASC_WeaponTrail_AnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	if (!WeaponTrailData->IsValidLowLevelFast())
	{
		return;
	}

	if (WeaponTrailNiagaraComponent)
	{
		float CurrentTimeRatio =  UAnimNotifyLibrary::GetCurrentAnimationNotifyStateTimeRatio(EventReference);
		SetWeaponTrailColorArrayAtTime(WeaponTrailNiagaraComponent, CurrentTimeRatio);
	}
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
}

void UGASC_WeaponTrail_AnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	DestroyWeaponTrailVFX();
	Super::NotifyEnd(MeshComp, Animation, EventReference);
}

UNiagaraComponent* UGASC_WeaponTrail_AnimNotifyState::GetSpawnedEffect(UMeshComponent* MeshComp)
{
	if (WeaponTrailData->IsValidLowLevelFast())
	{
		if (UNiagaraSystem* WeaponTrail = WeaponTrailData->WeaponTrailNiagaraSystem.Get())
		{
			return UNiagaraFunctionLibrary::SpawnSystemAttached(WeaponTrail, MeshComp, SocketName, LocationOffset, RotationOffset, EAttachLocation::KeepRelativeOffset, !bDestroyAtEnd,
				true, ENCPoolMethod::AutoRelease, true);
		}
	}
	
	UE_LOG(LogGASCourseWeaponTrail, Warning, TEXT("Invalid Weapon Trail data found in %s. Please fix."), *GetPathNameSafe(this));
	return nullptr;
}

void UGASC_WeaponTrail_AnimNotifyState::SetWeaponTrailMaterialInterface(UNiagaraComponent* InWeaponTrailNiagaraComponent) const
{
	if (!WeaponTrailData->WeaponTrailMaterialInterface.Get())
	{
		UE_LOG(LogGASCourseWeaponTrail, Warning, TEXT("Invalid Weapon Trail Material Interface in Weapon Trail Data: %s found in %s. Please fix."), *GetPathNameSafe(WeaponTrailData),
			*GetPathNameSafe(this));
		return;
	}
	
	InWeaponTrailNiagaraComponent->SetVariableMaterial(FName("Material Interface") ,WeaponTrailData->WeaponTrailMaterialInterface.Get());
}

void UGASC_WeaponTrail_AnimNotifyState::SetWeaponTrailColorArrayAtTime(UNiagaraComponent* InWeaponTrailNiagaraComponent,
	float InTime) const
{
	if (!WeaponTrailData->ColorCurve.Get())
	{
		UE_LOG(LogGASCourseWeaponTrail, Warning, TEXT("Invalid Weapon Trail Color curve in Weapon Trail Data: %s found in %s. Please fix."), *GetPathNameSafe(WeaponTrailData),
			*GetPathNameSafe(this));
		return;
	}

	UCurveLinearColor* ColorCurve = WeaponTrailData->ColorCurve.Get();
	FLinearColor ColorData = ColorCurve->GetLinearColorValue(InTime);
	TArray<FLinearColor> ArrayData = {ColorData};
	
	
	UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayColor(InWeaponTrailNiagaraComponent, "Color Selection Array_Color", ArrayData);
}

void UGASC_WeaponTrail_AnimNotifyState::SetWeaponTrailLifeTime(UNiagaraComponent* InWeaponTrailNiagaraComponent,
	float InLifeTime) const
{
	if (!WeaponTrailData)
	{
		UE_LOG(LogGASCourseWeaponTrail, Warning, TEXT("Invalid Weapon Trail Data found in %s. Please fix."), *GetPathNameSafe(this));
		return;
	}

	InWeaponTrailNiagaraComponent->SetVariableFloat(FName("LifeTime"), InLifeTime);
}

void UGASC_WeaponTrail_AnimNotifyState::DestroyWeaponTrailVFX() const
{
	if (WeaponTrailNiagaraComponent)
	{
		if (bDestroyAtEnd)
		{
			WeaponTrailNiagaraComponent->DestroyComponent();
		}
		else
		{
			WeaponTrailNiagaraComponent->Deactivate();
		}
	}
}
