// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/BlueprintLibraries/TargetingSystem/GASCourseTargetingLibrary.h"

#include "GASCourse/GASCourseCharacter.h"


void UGASCourseTargetingLibrary::GetBestTargetFromHandle(FTargetingRequestHandle TargetingHandle, AActor* &Target, bool &bTargetFound)
{
	bTargetFound = false;
	Target = nullptr;
	
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
				//bTargetFound = false;
				//Target = nullptr;
				return;
			}
			
			bTargetFound = true;
			Target = Targets.Last();
		}
	}
}

bool UGASCourseTargetingLibrary::AreActorsOnSameTeam(AActor* InActor1, AActor* InActor2)
{
	AGASCourseCharacter* InstigatorCharacter = Cast<AGASCourseCharacter>(InActor1);
	uint8 InstigatorTeamID = InstigatorCharacter->GetGenericTeamId();

	AGASCourseCharacter* TargetCharacter = Cast<AGASCourseCharacter>(InActor2);
	uint8 TargetTeamID = TargetCharacter->GetGenericTeamId();

	return InstigatorTeamID == TargetTeamID;
}
