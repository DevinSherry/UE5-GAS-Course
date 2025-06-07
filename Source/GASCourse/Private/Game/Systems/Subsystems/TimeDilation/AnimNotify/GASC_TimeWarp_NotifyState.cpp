// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Systems/Subsystems/TimeDilation/AnimNotify/GASC_TimeWarp_NotifyState.h"
#include "Animation/AnimNotifyLibrary.h"
#include "Kismet/KismetMathLibrary.h"


UGASC_TimeWarp_NotifyState::UGASC_TimeWarp_NotifyState()
{
	TimeWarpCurve.EditorCurveData.AddKey(0.0f, 1.0f);
	TimeWarpCurve.EditorCurveData.AddKey(1.0f, 1.0f);

#if WITH_EDITORONLY_DATA
	NotifyColor = FColor::Red;
#endif 
}

void UGASC_TimeWarp_NotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	if (TimeWarpCurve.EditorCurveData.Keys.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Empty TimeWarp Curve used: %s - Consider removing. Will not process"), *GetPathNameSafe(this));
		return;
	}

	if (MeshComp)
	{
		AnimInstance = MeshComp->GetAnimInstance();
		if (AnimInstance)
		{
			AnimMontage = Cast<UAnimMontage>(Animation);
			if (AnimMontage)
			{
				Duration = TotalDuration;
				bTimeWarpEnabled = true;
				DefaultPlayRate = AnimInstance->Montage_GetPlayRate(AnimMontage);
			}
		}
	}
}

void UGASC_TimeWarp_NotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	if (bTimeWarpEnabled && AnimInstance)
	{
		float CurveMinTime = 0.0f;
		float CurveMaxTime = 0.0f;
		TimeWarpCurve.GetRichCurve()->GetTimeRange(CurveMinTime, CurveMaxTime);

		float CurrentAnimationStateTime = UAnimNotifyLibrary::GetCurrentAnimationNotifyStateTime(EventReference);
		float AdjustedAnimationStateTime = UKismetMathLibrary::MapRangeClamped(CurrentAnimationStateTime, 0.0f, Duration, 0.0f, CurveMaxTime);
		float CurveValue = TimeWarpCurve.EditorCurveData.Eval(AdjustedAnimationStateTime);
		
		AnimInstance->Montage_SetPlayRate(AnimMontage, CurveValue);
	}
}

void UGASC_TimeWarp_NotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	if (bTimeWarpEnabled && AnimInstance)
	{
		AnimInstance->Montage_SetPlayRate(AnimMontage, DefaultPlayRate);
	}
}
