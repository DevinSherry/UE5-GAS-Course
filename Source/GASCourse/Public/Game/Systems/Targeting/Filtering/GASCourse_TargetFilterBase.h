// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tasks/TargetingFilterTask_BasicFilterTemplate.h"
#include "GASCourse_TargetFilterBase.generated.h"

/**
 * 
 */
UCLASS()
class GASCOURSE_API UGASCourse_TargetFilterBase : public UTargetingFilterTask_BasicFilterTemplate
{
	GENERATED_BODY()

protected:
	
	/** Called against every target data to determine if the target should be filtered out */
	virtual bool ShouldFilterTarget(const FTargetingRequestHandle& TargetingHandle, const FTargetingDefaultResultData& TargetData) const;
	
	
};
