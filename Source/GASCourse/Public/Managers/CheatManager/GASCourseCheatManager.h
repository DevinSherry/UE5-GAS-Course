// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/CheatManager.h"
#include "GASCourseCheatManager.generated.h"

/**
 * 
 */
UCLASS()
class GASCOURSE_API UGASCourseCheatManager : public UCheatManager
{
	GENERATED_BODY()
public:
	
	virtual void InitCheatManager() override;
	
	/** Registers a cheat manager extension with this cheat manager */
	void AddCheatManagerExtension(UCheatManagerExtension* CheatObject);
	
};
