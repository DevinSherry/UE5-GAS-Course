// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayCueManager.h"
#include "GASCourseGameplayCueManager.generated.h"

/**
 * 
 */
UCLASS()
class UGASCourseGameplayCueManager : public UGameplayCueManager
{
	GENERATED_BODY()

protected:
	
	virtual bool ShouldAsyncLoadRuntimeObjectLibraries() const override;
	
};
