// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GASCourseCheatManagerExt.h"
#include "GameFramework/CheatManager.h"
#include "GASCourseCheatManager.generated.h"

/**
 * A custom cheat manager class designed for the GAS Course.
 * Extends functionality by allowing the addition of cheat manager extensions.
 */
UCLASS()
class GASCOURSE_API UGASCourseCheatManager : public UCheatManager
{
	GENERATED_BODY()
public:
	
	virtual void InitCheatManager() override;
	
	/** Registers a cheat manager extension with this cheat manager */
	void AddCheatManagerExtension(UCheatManagerExtension* CheatObject);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GASCourse|CheatManager")
	TArray<TSubclassOf<UGASCourseCheatManagerExt>> CheatManagerExtensionClasses;
};
