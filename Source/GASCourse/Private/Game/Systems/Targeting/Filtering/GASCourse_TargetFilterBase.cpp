// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Systems/Targeting/Filtering/GASCourse_TargetFilterBase.h"

bool UGASCourse_TargetFilterBase::ShouldFilterTarget(const FTargetingRequestHandle& TargetingHandle,
	const FTargetingDefaultResultData& TargetData) const
{
	return Super::ShouldFilterTarget(TargetingHandle, TargetData);
}
