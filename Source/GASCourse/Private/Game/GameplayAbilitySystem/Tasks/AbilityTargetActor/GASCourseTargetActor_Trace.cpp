// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GameplayAbilitySystem/Tasks/AbilityTargetActor/GASCourseTargetActor_Trace.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"
#include "Game/GameplayAbilitySystem/GASCourseNativeGameplayTags.h"

AGASCourseTargetActor_Trace::AGASCourseTargetActor_Trace(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	
}
void AGASCourseTargetActor_Trace::StartTargeting(UGameplayAbility* InAbility)
{
	Super::StartTargeting(InAbility);
	UpdateLooseGameplayTagsDuringTargeting(Status_Block_PointClickMovementInput, 1);
}

void AGASCourseTargetActor_Trace::ConfirmTargetingAndContinue()
{
	Super::ConfirmTargetingAndContinue();
	UpdateLooseGameplayTagsDuringTargeting(Status_Block_PointClickMovementInput, 0);
}

void AGASCourseTargetActor_Trace::CancelTargeting()
{
	Super::CancelTargeting();
	UpdateLooseGameplayTagsDuringTargeting(Status_Block_PointClickMovementInput, 0);
}

void AGASCourseTargetActor_Trace::UpdateLooseGameplayTagsDuringTargeting(FGameplayTag InGameplayTag, int32 InCount)
{
	if(UAbilitySystemComponent* ASC = OwningAbility->GetCurrentActorInfo()->AbilitySystemComponent.Get())
	{
		ASC->SetLooseGameplayTagCount(InGameplayTag, InCount);
	}
}
