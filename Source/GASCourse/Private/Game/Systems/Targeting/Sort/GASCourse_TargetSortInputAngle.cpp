// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Systems/Targeting/Sort/GASCourse_TargetSortInputAngle.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/LocalPlayer.h"
#include "Kismet/KismetTextLibrary.h"

namespace GASCourse_TargetingSystemCVars
{
	static bool bEnableInputTargetingDebug = false;
	FAutoConsoleVariableRef CvarEnableInputTargetingDebugging(
		TEXT("GASCourseDebug.Targeting.EnableDebug.Sort.InputAngle"),
		bEnableInputTargetingDebug,
		TEXT("Enable on-screen debugging for input based targeting(Enabled: true, Disabled: false)"));

	static bool bDisableInputSortTargeting = false;
	FAutoConsoleVariableRef CvarDisableInputSortTargeting(
		TEXT("GASCourseDebug.Targeting.Disable.Sort.InputAngle"),
		bDisableInputSortTargeting,
		TEXT("Disable input based sorting for targeting.(Enabled: true, Disabled: false)"));
	
};

void UGASCourse_TargetSortInputAngle::Init(const FTargetingRequestHandle& TargetingHandle) const
{
	Super::Init(TargetingHandle);

	if(SourcePawn)
	{
		FRotator Delta = UKismetMathLibrary::NormalizedDeltaRotator(SourcePawn->GetActorRotation(), SourcePawn->GetControlRotation());
		Delta.Pitch = 0.0f;
		InputDirection = Delta.UnrotateVector(SourcePawn->GetLastMovementInputVector());

#if !UE_BUILD_SHIPPING
		if(GASCourse_TargetingSystemCVars::bEnableInputTargetingDebug)
		{
			FlushPersistentDebugLines(SourcePawn->GetWorld());
			FlushDebugStrings(SourcePawn->GetWorld());
			FVector LineEndInputDirection = SourcePawn->GetActorLocation() + InputDirection.GetSafeNormal() * 500.0f;
			DrawDebugSphere(SourcePawn->GetWorld(), LineEndInputDirection, 64.0f, 100, FColor::Yellow, false, 5.0f, 0, 2.0f);
			DrawDebugDirectionalArrow(SourcePawn->GetWorld(), SourcePawn->GetActorLocation(), LineEndInputDirection, 50.0f, FColor::Yellow, false, 5.0f, 0, 5.0f);
		}
#endif
	}
}

void UGASCourse_TargetSortInputAngle::Execute(const FTargetingRequestHandle& TargetingHandle) const
{
	Super::Execute(TargetingHandle);
}

float UGASCourse_TargetSortInputAngle::GetScoreForTarget(const FTargetingRequestHandle& TargetingHandle,
	const FTargetingDefaultResultData& TargetData) const
{
	if(GASCourse_TargetingSystemCVars::bDisableInputSortTargeting)
	{
		return 0.0f;
	}
	
	if(SourcePawn && SourcePlayerController)
	{
		if(TargetData.HitResult.HasValidHitObjectHandle())
		{
			if(AActor* HitActor = TargetData.HitResult.HitObjectHandle.FetchActor())
			{
				FRotator Delta = UKismetMathLibrary::NormalizedDeltaRotator(SourcePawn->GetActorRotation(), SourcePawn->GetControlRotation());
				Delta.Pitch = 0.0f;
				InputDirection = Delta.UnrotateVector(SourcePawn->GetLastMovementInputVector());
				
				FVector UnitDirection = UKismetMathLibrary::GetDirectionUnitVector(SourcePawn->GetActorLocation(), HitActor->GetActorLocation());
				float InputToTargetDotProduct = UKismetMathLibrary::Dot_VectorVector(InputDirection.GetSafeNormal(), UnitDirection);

				Score = ScoreCurve ? ScoreCurve->GetFloatValue(InputToTargetDotProduct) : InputToTargetDotProduct;
#if !UE_BUILD_SHIPPING
				if(GASCourse_TargetingSystemCVars::bEnableInputTargetingDebug)
				{
					UE_LOG(LogTemp, Warning, TEXT("CURRENT SCORE: %f"), TargetData.Score);
					UE_LOG(LogTemp, Warning, TEXT("Input ANGLE = %f"), UKismetMathLibrary::DegAcos(InputToTargetDotProduct));
					FString ScoreString = UKismetTextLibrary::Conv_DoubleToText(Score, HalfToEven, false, false, 1, 324,
						1, 2).ToString();
					FString FinalScoreString = UKismetTextLibrary::Conv_DoubleToText(Score * FinalScoreMultiplier, HalfToEven, false, false, 1, 324,
						1, 2).ToString();
					FString InputAngleString = UKismetTextLibrary::Conv_DoubleToText(UKismetMathLibrary::DegAcos(InputToTargetDotProduct), HalfToEven, false, false, 1, 324,
	1, 2).ToString();
					FString EndResults = *FString::Printf(TEXT("Score = %s | Final Score = %s | Angle = %s°" ), *ScoreString,  *FinalScoreString, *InputAngleString);

					FLinearColor ScoreColor = UKismetMathLibrary::LinearColorLerp(FLinearColor::Red, FLinearColor::Green, Score);
	
					FVector ActorOrigin(0.0f);
					FVector ActorExtents(0.0f);
					HitActor->GetActorBounds(true, ActorOrigin, ActorExtents, false);
					DrawDebugBox(HitActor->GetWorld(), HitActor->GetActorLocation(), ActorExtents, ScoreColor.ToFColor(true), false, 5.0f, 0, 2.0f);
					DrawDebugString(HitActor->GetWorld(), FVector(-ActorExtents.X, -125.0f, ActorExtents.Z), EndResults, HitActor, ScoreColor.ToFColor(true), 5.0f, true, 1.0f);
				
					UE_LOG(LogTemp, Warning, TEXT("Score = %f /n Final Score = %f"), Score,  Score * FinalScoreMultiplier);
					FVector LineEndUnitDirection = SourcePawn->GetActorLocation() + UnitDirection * 500.0f;
					DrawDebugDirectionalArrow(SourcePawn->GetWorld(), SourcePawn->GetActorLocation(), LineEndUnitDirection, 50.0f, FColor::Red, false, 5.0f, 0, 5.0f);
				}
#endif
				return Score * FinalScoreMultiplier;
			}
		}
	}
	return 0.0f;
}
