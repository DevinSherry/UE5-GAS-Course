// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/CheatManager/GASCourseCheatManager.h"
#include "GASCourse/GASCourseCharacter.h"
#include "Managers/CheatManager/GASCourseCheatManagerExt.h"

void UGASCourseCheatManager::InitCheatManager()
{
	// Iterate over the class types in CheatManagerExtensions
	for (TSubclassOf<UGASCourseCheatManagerExt> ExtensionClass : CheatManagerExtensionClasses)
	{
		// Check if the class type is valid
		if (ExtensionClass != nullptr)
		{
			// Create an instance of the class
			UGASCourseCheatManagerExt* Extension = NewObject<UGASCourseCheatManagerExt>(this, ExtensionClass);

			// Add the instance to the cheat manager extensions if it is valid
			if (Extension != nullptr)
			{
				AddCheatManagerExtension(Extension);
			}
		}
	}
}

void UGASCourseCheatManager::AddCheatManagerExtension(UCheatManagerExtension* CheatObject)
{
	Super::AddCheatManagerExtension(CheatObject);
}
