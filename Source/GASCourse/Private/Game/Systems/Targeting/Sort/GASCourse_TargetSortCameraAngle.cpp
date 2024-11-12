// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Systems/Targeting/Sort/GASCourse_TargetSortCameraAngle.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetTextLibrary.h"

namespace GASCourse_TargetingSystemCVars
{
	static bool bEnableCameraTargetingDebug = false;
	FAutoConsoleVariableRef CvarEnableCameraTargetingDebugging(
		TEXT("GASCourseDebug.Targeting.EnableDebug.Sort.CameraAngle"),
		bEnableCameraTargetingDebug,
		TEXT("Enable on-screen debugging for camera based targeting. (Enabled: true, Disabled: false)"));

	static bool bDisableCameraSortTargeting = false;
	FAutoConsoleVariableRef CvarDisableCameraSortTargeting(
		TEXT("GASCourseDebug.Targeting.Disable.Sort.CameraAngle"),
		bDisableCameraSortTargeting,
		TEXT("Disable camera based sorting for targeting.(Enabled: true, Disabled: false)"));
	
}

void UGASCourse_TargetSortCameraAngle::Init(const FTargetingRequestHandle& TargetingHandle) const
{
	Super::Init(TargetingHandle);

	if(GASCourse_TargetingSystemCVars::bDisableCameraSortTargeting)
	{
		return;
	}
	
	if (const FTargetingSourceContext* SourceContext = FTargetingSourceContext::Find(TargetingHandle))
	{
		if (APawn* InstigatorPawn = Cast<APawn>(SourceContext->SourceActor))
		{
			if(APlayerController* PlayerController = Cast<APlayerController>(InstigatorPawn->GetController()))
			{
				CameraManager = PlayerController->PlayerCameraManager;
			}

#if !UE_BUILD_SHIPPING
			if(GASCourse_TargetingSystemCVars::bEnableCameraTargetingDebug)
			{
				FlushPersistentDebugLines(SourcePawn->GetWorld());
				FlushDebugStrings(SourcePawn->GetWorld());
				FRotator ControlRotation = SourcePlayerController->GetControlRotation();
				ControlRotation.Pitch = 0.0f;
				FVector LineEndControlRotation = SourcePawn->GetActorLocation() + UKismetMathLibrary::Conv_RotatorToVector(ControlRotation) * 500.0f;
				DrawDebugDirectionalArrow(SourcePawn->GetWorld(), SourcePawn->GetActorLocation(), LineEndControlRotation, 50.0f, FColor::Green, false, 5.0f, 0, 5.0f);
			}
#endif
		}
	}
}

void UGASCourse_TargetSortCameraAngle::Execute(const FTargetingRequestHandle& TargetingHandle) const
{
	Super::Execute(TargetingHandle);
}

float UGASCourse_TargetSortCameraAngle::GetScoreForTarget(const FTargetingRequestHandle& TargetingHandle,
	const FTargetingDefaultResultData& TargetData) const
{
	if((GASCourse_TargetingSystemCVars::bDisableCameraSortTargeting))
	{
		return 0.0f;
	}
	
	if(CameraManager)
	{
		if(TargetData.HitResult.HasValidHitObjectHandle())
		{
			if(AActor* HitActor = TargetData.HitResult.HitObjectHandle.FetchActor())
			{
				FVector CameraForwardVector = CameraManager->GetActorForwardVector();
				FVector UnitDirection = UKismetMathLibrary::GetDirectionUnitVector(SourcePawn->GetActorLocation(), HitActor->GetActorLocation());

				float CameraToTargetDotProduct = UKismetMathLibrary::Dot_VectorVector(CameraForwardVector, UnitDirection);
				float DotToDegrees = UKismetMathLibrary::DegAcos(CameraToTargetDotProduct);
				
				Score = ScoreCurve ? ScoreCurve->GetFloatValue(DotToDegrees) : UKismetMathLibrary::MapRangeClamped(DotToDegrees, 0.0f, MaxCameraAngle, 1.0f, 0.0f);

#if !UE_BUILD_SHIPPING
				if(GASCourse_TargetingSystemCVars::bEnableCameraTargetingDebug)
				{
					UE_LOG(LogTemp, Warning, TEXT("CURRENT SCORE: %f"), TargetData.Score);
					UE_LOG(LogTemp, Warning, TEXT("Camera ANGLE = %f"), DotToDegrees);
					FString ScoreString = UKismetTextLibrary::Conv_DoubleToText(Score, HalfToEven, false, false, 1, 324,
						1, 2).ToString();
					FString FinalScoreString = UKismetTextLibrary::Conv_DoubleToText(Score * FinalScoreMultiplier, HalfToEven, false, false, 1, 324,
						1, 2).ToString();
					FString CameraAngleString = UKismetTextLibrary::Conv_DoubleToText(DotToDegrees, HalfToEven, false, false, 1, 324,
	1, 2).ToString();
					FString EndResults = *FString::Printf(TEXT("Score = %s | Final Score = %s | Angle = %s°" ), *ScoreString,  *FinalScoreString, *CameraAngleString);

					FLinearColor ScoreColor = UKismetMathLibrary::LinearColorLerp(FLinearColor::Red, FLinearColor::Green, Score);
	
					FVector ActorOrigin(0.0f);
					FVector ActorExtents(0.0f);
					HitActor->GetActorBounds(true, ActorOrigin, ActorExtents, false);
					DrawDebugBox(HitActor->GetWorld(), HitActor->GetActorLocation(), ActorExtents, ScoreColor.ToFColor(true), false, 5.0f, 0, 2.0f);
					DrawDebugString(HitActor->GetWorld(), FVector(-ActorExtents.X, -125.0f, ActorExtents.Z), EndResults, HitActor, ScoreColor.ToFColor(true), 5.0f, true, 1.0f);
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
