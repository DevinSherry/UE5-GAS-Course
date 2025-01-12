// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/CheatManager.h"
#include "Managers/CheatManager/GASCourseCheatManagerExt.h"
#include "GASC_AbilitySystem_CheatExt.generated.h"

/**
 * @brief The UGASC_AbilitySystem_CheatExt class provides functionality to modify gameplay attributes via cheats.
 *
 * This class extends UGASCourseCheatManagerExt and allows developers to use execution commands
 * (Exec functions) to manipulate gameplay attribute values during runtime for debugging and testing purposes.
 */
UCLASS()
class GASCOURSE_API UGASC_AbilitySystem_CheatExt : public UGASCourseCheatManagerExt
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(Exec, Category = "GASCourse|CheatManager|AbiltySystem", meta=(ToolTip = "Sets the value of a gameplay attribute. [AttributeName] [Value]"))
	void SetGameplayAttribute(FName AttributeName, float Value);
};
