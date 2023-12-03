// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GameplayAbilitySystem/GameplayCues/GASCourseGCNotify_Looping.h"
#include "GASCourse/GASCourseCharacter.h"
#include "GASCourse/GASCourse.h"
#include "AbilitySystemGlobals.h"
#include "GameplayCueManager.h"

AGASCourseGCNotify_Looping::AGASCourseGCNotify_Looping()
{
	
}

void AGASCourseGCNotify_Looping::HandleGameplayCue(AActor* MyTarget, EGameplayCueEvent::Type EventType,
                                                   const FGameplayCueParameters& Parameters)
{
	Super::HandleGameplayCue(MyTarget, EventType, Parameters);
}

void AGASCourseGCNotify_Looping::GameplayCueFinishedCallback()
{
	UWorld* MyWorld = GetWorld();
	if (MyWorld) // Teardown cases in PIE may cause the world to be invalid
		{
		if (FinishTimerHandle.IsValid())
		{
			MyWorld->GetTimerManager().ClearTimer(FinishTimerHandle);
			FinishTimerHandle.Invalidate();
		}

		// Make sure OnRemoved has been called at least once if WhileActive was called (for possible cleanup)
		if (bHasHandledWhileActiveEvent && !bHasHandledOnRemoveEvent)
		{
			// Force onremove to be called with null parameters
			bHasHandledOnRemoveEvent = true;
			OnRemove(GetOwner(), FGameplayCueParameters());
		}
		}
	
	UAbilitySystemGlobals::Get().GetGameplayCueManager()->NotifyGameplayCueActorFinished(this);
}

bool AGASCourseGCNotify_Looping::OnActive_Implementation(AActor* Target, const FGameplayCueParameters& Parameters)
{
	if(const AGASCourseCharacter* TargetCharacter = Cast<AGASCourseCharacter>(Target))
	{
		if(USkeletalMeshComponent* Mesh = TargetCharacter->GetComponentByClass<USkeletalMeshComponent>())
		{
			Mesh->SetRenderCustomDepth(true);
			Mesh->SetCustomDepthStencilValue(STENCIL_STATUS_BURN_OUTLINE);
		}
	}
	
	return Super::OnActive_Implementation(Target, Parameters);
}

bool AGASCourseGCNotify_Looping::OnRemove_Implementation(AActor* Target, const FGameplayCueParameters& Parameters)
{
	if(const AGASCourseCharacter* TargetCharacter = Cast<AGASCourseCharacter>(Target))
	{
		if(USkeletalMeshComponent* Mesh = TargetCharacter->GetComponentByClass<USkeletalMeshComponent>())
		{
			Mesh->SetRenderCustomDepth(false);
			Mesh->SetCustomDepthStencilValue(STENCIL_NONE);
		}
	}
	
	return Super::OnRemove_Implementation(Target, Parameters);
}

void AGASCourseGCNotify_Looping::BeginPlay()
{
	Super::BeginPlay();
}