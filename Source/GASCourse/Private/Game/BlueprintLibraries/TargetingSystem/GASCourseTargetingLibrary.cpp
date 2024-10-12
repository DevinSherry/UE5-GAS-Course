// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/BlueprintLibraries/TargetingSystem/GASCourseTargetingLibrary.h"


void UGASCourseTargetingLibrary::GetBestTargetFromHandle(FTargetingRequestHandle TargetingHandle, AActor* &Target, bool &bTargetFound)
{
	bTargetFound = false;
	
	if (TargetingHandle.IsValid())
	{
		if (FTargetingDefaultResultsSet* Results = FTargetingDefaultResultsSet::Find(TargetingHandle))
		{
			TArray<AActor*> Targets;
			for (const FTargetingDefaultResultData& ResultData : Results->TargetResults)
			{
				if (AActor* InTarget = ResultData.HitResult.GetActor())
				{
					Targets.Add(InTarget);
				}
			}
			if(Targets.Num() == 0)
			{
				bTargetFound = false;
				return;
			}
			
			bTargetFound = true;
			Target = Targets.Last();
		}
	}
}
