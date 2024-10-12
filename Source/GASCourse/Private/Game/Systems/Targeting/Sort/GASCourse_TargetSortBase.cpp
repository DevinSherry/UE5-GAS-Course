// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Systems/Targeting/Sort/GASCourse_TargetSortBase.h"


void UGASCourse_TargetSortBase::Execute(const FTargetingRequestHandle& TargetingHandle) const
{
	Super::Execute(TargetingHandle);
	UE_LOG(LogTemp, Warning, TEXT("Is Moving: %s"), bIsSourceActorMoving ? TEXT("TRUE"): TEXT("FALSE"));
}

void UGASCourse_TargetSortBase::Init(const FTargetingRequestHandle& TargetingHandle) const
{
	Super::Init(TargetingHandle);
	UE_LOG(LogTemp, Warning, TEXT("Init!"));
	
	if (const FTargetingSourceContext* SourceContext = FTargetingSourceContext::Find(TargetingHandle))
	{
		if (APawn* InstigatorPawn = Cast<APawn>(SourceContext->SourceActor))
		{
			bIsSourceActorMoving = InstigatorPawn->GetLastMovementInputVector().Length() > 0.0f;
		}
	}
}

float UGASCourse_TargetSortBase::GetScoreForTarget(const FTargetingRequestHandle& TargetingHandle,
	const FTargetingDefaultResultData& TargetData) const
{
	return Super::GetScoreForTarget(TargetingHandle, TargetData);
}
