// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GASCourseGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class GASCOURSE_API UGASCourseGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	/** virtual function to allow custom GameInstances an opportunity to set up what it needs */
	virtual void Init() override;
	
};
