// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GameplayAbilitySystem/Tasks/AbilityTask_WaitDurationChange.h"
#include "Game/GameplayAbilitySystem/GASCourseGameplayAbility.h"

UAbilityTask_WaitDurationChange::UAbilityTask_WaitDurationChange(const FObjectInitializer& ObjectInitializer)
{
	IntervalCheck = 0.1f;
}

void UAbilityTask_WaitDurationChange::EndAction()
{
	Super::EndAction();
}


UAbilityTask_WaitDurationChange* UAbilityTask_WaitDurationChange::WaitForDurationChange(UGameplayAbility* InOwningAbility, float InIntervalCheck)
{
	UAbilityTask_WaitDurationChange* MyObj = NewObject<UAbilityTask_WaitDurationChange>(InOwningAbility);
	MyObj->OwningAbility = InOwningAbility;
	MyObj->IntervalCheck = InIntervalCheck;
	return MyObj;
}

void UAbilityTask_WaitDurationChange::Activate()
{
	Super::Activate();
	if (UGASCourseGameplayAbility* MyAbility = Cast<UGASCourseGameplayAbility>(OwningAbility.Get()))
	{
		GetWorld()->GetTimerManager().SetTimer(DurationIntervalTimerHandle, this, &ThisClass::OnDurationChanged, IntervalCheck, true);
		MyAbility->OnDurationEffectRemovedDelegate.AddDynamic(this, &UAbilityTask_WaitDurationChange::OnDurationFinished);
	}
	else
	{
		EndAction();
	}

}

void UAbilityTask_WaitDurationChange::OnDurationChanged() const
{
	float DurationRemaining = 0.0f;
	float DurationRemainingAsPercentage = 0.0f;
	if (const UGASCourseGameplayAbility* MyAbility = Cast<UGASCourseGameplayAbility>(OwningAbility.Get()))
	{
		const float TotalDuration = MyAbility->GetActiveDurationTime();
		DurationRemaining = MyAbility->GetActiveDurationTimeRemaining();
		DurationRemainingAsPercentage = FMath::Clamp((DurationRemaining/TotalDuration), 0.0f, 1.0f);
	}
	OnDurationChange.Broadcast(DurationRemaining, DurationRemainingAsPercentage);
}

void UAbilityTask_WaitDurationChange::OnDurationFinished()
{
	OnDurationDone.Broadcast();
	
	if(DurationIntervalTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(DurationIntervalTimerHandle);
	}
	EndAction();
}

