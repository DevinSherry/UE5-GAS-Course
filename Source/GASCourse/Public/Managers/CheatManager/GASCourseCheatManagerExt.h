// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/CheatManager.h"
#include "GASCourseCheatManagerExt.generated.h"

/**
 * @class UGASCourseCheatManagerExt
 * @brief Extension class for the GASCourse cheat manager framework.
 *
 * UGASCourseCheatManagerExt is an abstract base class derived from UCheatManagerExtension.
 * It provides a foundation for creating custom cheat manager extensions for the GASCourse ecosystem,
 * allowing developers to add functionality specific to gameplay or testing requirements.
 *
 * This class is designed to be subclassed to implement specific cheat functionalities.
 * Instances of subclasses can be registered and utilized with UGASCourseCheatManager.
 */
UCLASS()
class GASCOURSE_API UGASCourseCheatManagerExt : public UCheatManagerExtension
{
	GENERATED_BODY()
	
};
