// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "GASCourseAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class GASCOURSE_API UGASCourseAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:

	UGASCourseAssetManager();

	// Returns the AssetManager singleton object.
	static UGASCourseAssetManager& Get();

protected:

	virtual void StartInitialLoading() override;
};
