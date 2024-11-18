// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/Systems/Targeting/Filtering/GASCourse_TargetFilterBase.h"
#include "GASCourse_TargetFilterOnScreen.generated.h"

/**
 * 
 */
UCLASS()
class GASCOURSE_API UGASCourse_TargetFilterOnScreen : public UGASCourse_TargetFilterBase
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="GASCourse|Targeting|Filtering")
	float ViewThresholdMinX = 100.0f;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="GASCourse|Targeting|Filtering")
	float ViewThresholdMinY = 100.0f;

	
protected:
	
	/** Called against every target data to determine if the target should be filtered out */
	virtual bool ShouldFilterTarget(const FTargetingRequestHandle& TargetingHandle, const FTargetingDefaultResultData& TargetData) const;
	
	
};
