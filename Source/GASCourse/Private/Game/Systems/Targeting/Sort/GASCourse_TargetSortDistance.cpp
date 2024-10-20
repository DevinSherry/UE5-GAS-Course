// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Systems/Targeting/Sort/GASCourse_TargetSortDistance.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetTextLibrary.h"

namespace GASCourse_TargetingSystemCVars
{
	static bool bEnableDistanceTargetingDebug = false;
	FAutoConsoleVariableRef CvarEnableDistanceTargetingDebugging(
		TEXT("GASCourseDebug.Targeting.EnableDebug.Sort.Distance"),
		bEnableDistanceTargetingDebug,
		TEXT("Enable on-screen debugging for distance based targeting. (Enabled: true, Disabled: false)"));

	static bool bDisableDistanceSortTargeting = false;
	FAutoConsoleVariableRef CvarDisableDistanceSortTargeting(
		TEXT("GASCourseDebug.Targeting.Disable.Sort.Distance"),
		bDisableDistanceSortTargeting,
		TEXT("Disable distance based sorting for targeting.(Enabled: true, Disabled: false)"));
	
}

void UGASCourse_TargetSortDistance::Init(const FTargetingRequestHandle& TargetingHandle) const
{
	Super::Init(TargetingHandle);

	if(GASCourse_TargetingSystemCVars::bDisableDistanceSortTargeting)
	{
		return;
	}

#if !UE_BUILD_SHIPPING
	if(GASCourse_TargetingSystemCVars::bEnableDistanceTargetingDebug)
	{
		FlushPersistentDebugLines(SourcePawn->GetWorld());
		FlushDebugStrings(SourcePawn->GetWorld());
	}
#endif
	
}

void UGASCourse_TargetSortDistance::Execute(const FTargetingRequestHandle& TargetingHandle) const
{
	Super::Execute(TargetingHandle);
}

float UGASCourse_TargetSortDistance::GetScoreForTarget(const FTargetingRequestHandle& TargetingHandle,
	const FTargetingDefaultResultData& TargetData) const
{
	if(GASCourse_TargetingSystemCVars::bDisableDistanceSortTargeting)
	{
		return 0.0f;
	}

	if(SourcePawn)
	{
		if(TargetData.HitResult.HasValidHitObjectHandle())
		{
			if(AActor* HitActor = TargetData.HitResult.HitObjectHandle.FetchActor())
			{
				float DistanceToTarget = UKismetMathLibrary::Vector_Distance(SourcePawn->GetActorLocation(), HitActor->GetActorLocation());
				Score = ScoreCurve ? ScoreCurve->GetFloatValue(DistanceToTarget) : UKismetMathLibrary::MapRangeClamped(DistanceToTarget, 0.0f, MaxDistance, 1.0f, 0.0f);

#if !UE_BUILD_SHIPPING
				if(GASCourse_TargetingSystemCVars::bEnableDistanceTargetingDebug)
				{
					UE_LOG(LogTemp, Warning, TEXT("CURRENT SCORE: %f"), TargetData.Score);
					FVector UnitDirection = UKismetMathLibrary::GetDirectionUnitVector(SourcePawn->GetActorLocation(), HitActor->GetActorLocation());
					UE_LOG(LogTemp, Warning, TEXT("Distance to %s = %f"), *HitActor->GetName(), DistanceToTarget);
					FString ScoreString = UKismetTextLibrary::Conv_DoubleToText(Score, HalfToEven, false, false, 1, 324,
						1, 2).ToString();
					FString FinalScoreString = UKismetTextLibrary::Conv_DoubleToText(Score * FinalScoreMultiplier, HalfToEven, false, false, 1, 324,
						1, 2).ToString();
					FString DistanceString = UKismetTextLibrary::Conv_DoubleToText(DistanceToTarget, HalfToEven, false, false, 1, 324,
	1, 2).ToString();
					FString EndResults = *FString::Printf(TEXT("Score = %s | Final Score = %s | Distance = %s" ), *ScoreString,  *FinalScoreString, *DistanceString);

					FLinearColor ScoreColor = UKismetMathLibrary::LinearColorLerp(FLinearColor::Red, FLinearColor::Green, Score);
	
					FVector ActorOrigin(0.0f);
					FVector ActorExtents(0.0f);
					HitActor->GetActorBounds(true, ActorOrigin, ActorExtents, false);
					DrawDebugBox(HitActor->GetWorld(), HitActor->GetActorLocation(), ActorExtents, ScoreColor.ToFColor(true), false, 5.0f, 0, 2.0f);
					DrawDebugString(HitActor->GetWorld(), FVector(-ActorExtents.X, -125.0f, ActorExtents.Z), EndResults, HitActor, ScoreColor.ToFColor(true), 5.0f, true, 1.0f);
				
					UE_LOG(LogTemp, Warning, TEXT("Score = %f /n Final Score = %f"), Score,  Score * FinalScoreMultiplier);
					FVector LineEndUnitDirection = SourcePawn->GetActorLocation() + UnitDirection * DistanceToTarget;
					FVector LineEndMaxDistanceDirection = SourcePawn->GetActorLocation() + UnitDirection * MaxDistance;
					DrawDebugDirectionalArrow(SourcePawn->GetWorld(), SourcePawn->GetActorLocation(), LineEndUnitDirection, 50.0f, ScoreColor.ToFColor(true), false, 5.0f, 0, 5.0f);
					DrawDebugDirectionalArrow(SourcePawn->GetWorld(), SourcePawn->GetActorLocation() + FVector(0.0f, 0.0f, 10.0f), LineEndMaxDistanceDirection, 50.0f, FColor::Yellow, false, 5.0f, 0, 5.0f);
				}
#endif
				return Score * FinalScoreMultiplier;
			}
		}
	}
	return 0.0f;
}
