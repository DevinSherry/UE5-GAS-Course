// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Types/TargetingSystemTypes.h"
#include "GASCourseTargetingLibrary.generated.h"

/**
 * 
 */
UCLASS(meta=(ScriptName="TargetingSystem"))
class GASCOURSE_API UGASCourseTargetingLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, Category="GASCourse|Targeting")
	static void GetBestTargetFromHandle(FTargetingRequestHandle TargetingHandle, AActor* &Target, bool &bTargetFound);

	UFUNCTION(BlueprintCallable, Category="GASCourse|Targeting")
	static bool AreActorsOnSameTeam(AActor* InActor1, AActor* InActor2);
	
	
};
