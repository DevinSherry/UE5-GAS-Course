// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Types/TargetingSystemTypes.h"
#include "Tasks/TargetingFilterTask_BasicFilterTemplate.h"
#include "GASC_TargetFilter_ActorClass.generated.h"

/**
 * 
 */
UCLASS()
class GASCOURSE_API UGASC_TargetFilter_ActorClass : public UTargetingFilterTask_BasicFilterTemplate
{
	GENERATED_BODY()

public:
	UGASC_TargetFilter_ActorClass(const FObjectInitializer& ObjectInitializer);

	/** Evaluation function called by derived classes to process the targeting request */
	virtual bool ShouldFilterTarget(const FTargetingRequestHandle& TargetingHandle, const FTargetingDefaultResultData& TargetData) const override;

	void AddRequiredActorClassFilter(const TObjectPtr<UClass>& ActorClassToAdd);
	void AddIgnoredActorClassFilter(const TObjectPtr<UClass>& ActorClassIgnored);

protected:
	/** The set of required actor classes (must be one of these types to not be filtered out) */
	UPROPERTY(EditAnywhere, Category = "Targeting Filter | Class", Meta = (AllowAbstract=true))
	TArray<TObjectPtr<UClass>> RequiredActorClassFilters;

	/** The set of ignored actor classes (must NOT be one of these types) */
	UPROPERTY(EditAnywhere, Category = "Targeting Filter | Class", Meta = (AllowAbstract = true))
	TArray<TObjectPtr<UClass>> IgnoredActorClassFilters;
};
