// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Systems/Subsystems/TimeDilation/GASC_TimeDilation_Subsystem.h"
#include "Kismet/GameplayStatics.h"


void UGASC_TimeDilation_Subsystem::Tick(float DeltaTime)
{
	if (!GEngine || !GEngine->IsInitialized())
	{
		return;
	}
	UWorld* World = GetWorld();
	if (!World || !World->IsValidLowLevel())
	{
		UE_LOG(LogTemp, Warning, TEXT("World is invalid! Skipping tick."));
		return;
	}
	
	if (!IsInitialized() & !bWorldBeginPlay)
	{
		return;
	}

	Super::Tick(DeltaTime);
	ProcessGlobalHitStops(DeltaTime);
	ProcessLocalHitStops(DeltaTime);
}

void UGASC_TimeDilation_Subsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UGASC_TimeDilation_Subsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UGASC_TimeDilation_Subsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	bWorldBeginPlay = true;
}

void UGASC_TimeDilation_Subsystem::AddGlobalHitStopWithData(FGASC_TimeDilation_Subsystem_Data HitStopData)
{
	if (ActiveGlobalTimeDilation)
	{
		if (HitStopData.Priority >= ActiveGlobalTimeDilation->Priority)
		{
			UE_LOG(LogTemp, Warning, TEXT("HitStopData has higher priority than active global hit stop! Replacing"));
			delete ActiveGlobalTimeDilation;
			ActiveGlobalTimeDilation = new FGASC_TimeDilation_Subsystem_Data(HitStopData);
		}
	}
	else
	{
		GlobalTimeDilationQueue.Enqueue(FGASC_TimeDilation_Subsystem_Data(HitStopData));
	}
}

void UGASC_TimeDilation_Subsystem::AddLocalHitStopWithData(FGASC_TimeDilation_Subsystem_Data HitStopData, TArray<AActor*> AffectedActors)
{
	HitStopData.AffectedActors = AffectedActors;
	if (ActiveLocalTimeDilation)
	{
		if (HitStopData.Priority >= ActiveLocalTimeDilation->Priority)
		{
			UE_LOG(LogTemp, Warning, TEXT("HitStopData has higher priority than active global hit stop! Replacing"));
			delete ActiveLocalTimeDilation;
			ActiveLocalTimeDilation = new FGASC_TimeDilation_Subsystem_Data(HitStopData);
		}
	}
	else
	{
		LocalTimeDilationQueue.Enqueue(FGASC_TimeDilation_Subsystem_Data(HitStopData));
	}
}

void UGASC_TimeDilation_Subsystem::AddGlobalHitStopWithCurve(float TimeDilationDuration, float TimeDilationMinValue,
	EAlphaBlendOption AlphaBlendOption, UCurveFloat* Curve, EGASC_TimeDilation_Priority Priority)
{
	if (ActiveGlobalTimeDilation)
	{
		if (Priority >= ActiveGlobalTimeDilation->Priority)
		{
			UE_LOG(LogTemp, Warning, TEXT("HitStopData has higher priority than active global hit stop! Replacing"));
			delete ActiveGlobalTimeDilation;
			
			FGASC_TimeDilation_Subsystem_Data HitStopData;
			
			HitStopData.TimeDilationDuration = TimeDilationDuration;
			HitStopData.TimeDilation = TimeDilationMinValue;
			HitStopData.BlendMode = AlphaBlendOption;
			HitStopData.CustomTimeDilationCurve = Curve;
			HitStopData.bUseTimeDilationCurve = true;
			HitStopData.Priority = Priority;
			
			ActiveGlobalTimeDilation = new FGASC_TimeDilation_Subsystem_Data(HitStopData);
		}
	}
	else
	{
		FGASC_TimeDilation_Subsystem_Data HitStopData;
		
		HitStopData.TimeDilationDuration = TimeDilationDuration;
		HitStopData.TimeDilation = TimeDilationMinValue;
		HitStopData.BlendMode = AlphaBlendOption;
		HitStopData.CustomTimeDilationCurve = Curve;
		HitStopData.bUseTimeDilationCurve = true;
		HitStopData.Priority = Priority;
		
		GlobalTimeDilationQueue.Enqueue(FGASC_TimeDilation_Subsystem_Data(HitStopData));
	}
}

void UGASC_TimeDilation_Subsystem::AddLocalHitStopWithCurve(TArray<AActor*> AffectedActors, float TimeDilationDuration, float TimeDilationMinValue,
	EAlphaBlendOption AlphaBlendOption, UCurveFloat* Curve, EGASC_TimeDilation_Priority Priority)
{
	if (ActiveLocalTimeDilation)
	{
		if (Priority >= ActiveLocalTimeDilation->Priority)
		{
			UE_LOG(LogTemp, Warning, TEXT("HitStopData has higher priority than active global hit stop! Replacing"));
			delete ActiveLocalTimeDilation;
			
			FGASC_TimeDilation_Subsystem_Data HitStopData;
			
			HitStopData.TimeDilationDuration = TimeDilationDuration;
			HitStopData.TimeDilation = TimeDilationMinValue;
			HitStopData.BlendMode = AlphaBlendOption;
			HitStopData.CustomTimeDilationCurve = Curve;
			HitStopData.bUseTimeDilationCurve = true;
			HitStopData.Priority = Priority;
			HitStopData.AffectedActors = AffectedActors;
			
			ActiveLocalTimeDilation = new FGASC_TimeDilation_Subsystem_Data(HitStopData);
		}
	}
	else
	{
		FGASC_TimeDilation_Subsystem_Data HitStopData;
		
		HitStopData.TimeDilationDuration = TimeDilationDuration;
		HitStopData.TimeDilation = TimeDilationMinValue;
		HitStopData.BlendMode = AlphaBlendOption;
		HitStopData.CustomTimeDilationCurve = Curve;
		HitStopData.bUseTimeDilationCurve = true;
		HitStopData.Priority = Priority;
		HitStopData.AffectedActors = AffectedActors;
		
		LocalTimeDilationQueue.Enqueue(FGASC_TimeDilation_Subsystem_Data(HitStopData));
	}
}

void UGASC_TimeDilation_Subsystem::AddGlobalHitStop(float TimeDilationDuration, float TimeDilationMinValue, EGASC_TimeDilation_Priority Priority)
{
	if (ActiveGlobalTimeDilation)
	{
		if (Priority >= ActiveGlobalTimeDilation->Priority)
		{
			UE_LOG(LogTemp, Warning, TEXT("HitStopData has higher priority than active global hit stop! Replacing"));
			delete ActiveGlobalTimeDilation;
			
			FGASC_TimeDilation_Subsystem_Data HitStopData;
			
			HitStopData.TimeDilationDuration = TimeDilationDuration;
			HitStopData.TimeDilation = TimeDilationMinValue;
			HitStopData.bUseTimeDilationCurve = false;
			HitStopData.Priority = Priority;
			
			ActiveGlobalTimeDilation = new FGASC_TimeDilation_Subsystem_Data(HitStopData);
		}
	}
	else
	{
		FGASC_TimeDilation_Subsystem_Data HitStopData;
		
		HitStopData.TimeDilationDuration = TimeDilationDuration;
		HitStopData.TimeDilation = TimeDilationMinValue;
		HitStopData.bUseTimeDilationCurve = false;
		HitStopData.Priority = Priority;
		
		GlobalTimeDilationQueue.Enqueue(FGASC_TimeDilation_Subsystem_Data(HitStopData));
	}
}

void UGASC_TimeDilation_Subsystem::AddLocalHitStop(TArray<AActor*> AffectedActors, float TimeDilationDuration, float TimeDilationMinValue, EGASC_TimeDilation_Priority Priority)
{
	if (ActiveLocalTimeDilation)
	{
		if (Priority >= ActiveLocalTimeDilation->Priority)
		{
			UE_LOG(LogTemp, Warning, TEXT("HitStopData has higher priority than active global hit stop! Replacing"));
			delete ActiveLocalTimeDilation;
			
			FGASC_TimeDilation_Subsystem_Data HitStopData;
			
			HitStopData.TimeDilationDuration = TimeDilationDuration;
			HitStopData.TimeDilation = TimeDilationMinValue;
			HitStopData.bUseTimeDilationCurve = false;
			HitStopData.Priority = Priority;
			HitStopData.AffectedActors = AffectedActors;
			
			ActiveLocalTimeDilation = new FGASC_TimeDilation_Subsystem_Data(HitStopData);
		}
	}
	else
	{
		FGASC_TimeDilation_Subsystem_Data HitStopData;
		
		HitStopData.TimeDilationDuration = TimeDilationDuration;
		HitStopData.TimeDilation = TimeDilationMinValue;
		HitStopData.bUseTimeDilationCurve = false;
		HitStopData.Priority = Priority;
		HitStopData.AffectedActors = AffectedActors;
		
		LocalTimeDilationQueue.Enqueue(FGASC_TimeDilation_Subsystem_Data(HitStopData));
	}
}

void UGASC_TimeDilation_Subsystem::StopAllTimeDilationRequests()
{
	StopAllGlobalTimeDilationRequests();
	StopAllLocalTimeDilationRequests();
}

void UGASC_TimeDilation_Subsystem::StopAllGlobalTimeDilationRequests()
{
	if (ActiveGlobalTimeDilation)
	{
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
			
		delete ActiveGlobalTimeDilation;
		ActiveGlobalTimeDilation = nullptr;
	}
}

void UGASC_TimeDilation_Subsystem::StopAllLocalTimeDilationRequests()
{
	if (ActiveLocalTimeDilation)
	{
		for (AActor* AffectedActor : ActiveLocalTimeDilation->AffectedActors)
		{
			if (AffectedActor)
			{
				AffectedActor->CustomTimeDilation = 1.0f;
			}
		}
		delete ActiveLocalTimeDilation;
		ActiveLocalTimeDilation = nullptr;
	}
}

void UGASC_TimeDilation_Subsystem::ProcessLocalHitStops(float DeltaTime)
{
	if (ActiveLocalTimeDilation)
	{
		if (ActiveLocalTimeDilation->AffectedActors.Num() == 0)
		{
			delete ActiveLocalTimeDilation;
			ActiveLocalTimeDilation = nullptr;
			return;
		}
		for (int32 i = ActiveLocalTimeDilation->AffectedActors.Num() - 1; i >= 0; i--)
		{
			AActor* AffectedActor = ActiveLocalTimeDilation->AffectedActors[i];
			if (!AffectedActor)
			{
				ActiveLocalTimeDilation->AffectedActors.RemoveAt(i);
			}
		}
		
		float ScaledDeltaTime = DeltaTime; // Provided during Tick or gameplay
		float GlobalTimeDilation = UGameplayStatics::GetGlobalTimeDilation(GetWorld());
		float UnscaledDeltaTime = ScaledDeltaTime / GlobalTimeDilation;

		if (ActiveLocalTimeDilation->bUseTimeDilationCurve)
		{
			if (ActiveLocalTimeDilation->CustomTimeDilationCurve)
			{
				const TArray<FRichCurveKey>& Keys = ActiveLocalTimeDilation->CustomTimeDilationCurve->FloatCurve.GetConstRefOfKeys();
				if (Keys.Num() > 0)
				{
					ActiveLocalTimeDilation->TimeDilationDuration = Keys.Last().Time;
				}
			}

			float CurveAlpha = ActiveLocalTimeDilation->ElapsedTime / ActiveLocalTimeDilation->TimeDilationDuration;
			float CurveValue = FAlphaBlend::AlphaToBlendOption(CurveAlpha, ActiveLocalTimeDilation->BlendMode, ActiveLocalTimeDilation->CustomTimeDilationCurve);

			CurveValue = FMath::GetMappedRangeValueClamped(FVector2D(0.0f, 1.0f), FVector2D(ActiveLocalTimeDilation->TimeDilation, 1.0f), CurveValue);

			for (AActor* AffectedActor : ActiveLocalTimeDilation->AffectedActors)
			{
				if (AffectedActor)
				{
					AffectedActor->CustomTimeDilation = CurveValue;
				}
			}
		}
		else
		{
			for (AActor* AffectedActor : ActiveLocalTimeDilation->AffectedActors)
			{
				if (AffectedActor)
				{
					AffectedActor->CustomTimeDilation = ActiveLocalTimeDilation->TimeDilation;
				}
			}
		}
		
		ActiveLocalTimeDilation->ElapsedTime += UnscaledDeltaTime;
		if (ActiveLocalTimeDilation->ElapsedTime >= ActiveLocalTimeDilation->TimeDilationDuration)
		{
			for (AActor* AffectedActor : ActiveLocalTimeDilation->AffectedActors)
			{
				if (AffectedActor)
				{
					AffectedActor->CustomTimeDilation = 1.0f;
				}
			}
			
			delete ActiveLocalTimeDilation;
			ActiveLocalTimeDilation = nullptr;
		}
	}
	else
	{
		if (!LocalTimeDilationQueue.IsEmpty())
		{
			FGASC_TimeDilation_Subsystem_Data DequeuedHitStop;
			if (LocalTimeDilationQueue.Dequeue(DequeuedHitStop))
			{
				ActiveLocalTimeDilation = new FGASC_TimeDilation_Subsystem_Data(DequeuedHitStop);
			}
		}
	}
}

void UGASC_TimeDilation_Subsystem::ProcessGlobalHitStops(float DeltaTime)
{
	if (ActiveGlobalTimeDilation)
	{
		float ScaledDeltaTime = DeltaTime; // Provided during Tick or gameplay
		float GlobalTimeDilation = UGameplayStatics::GetGlobalTimeDilation(GetWorld());
		float UnscaledDeltaTime = ScaledDeltaTime / GlobalTimeDilation;
		
		if (ActiveGlobalTimeDilation->bUseTimeDilationCurve)
		{
			if (ActiveGlobalTimeDilation->CustomTimeDilationCurve)
			{
				const TArray<FRichCurveKey>& Keys = ActiveGlobalTimeDilation->CustomTimeDilationCurve->FloatCurve.GetConstRefOfKeys();
				if (Keys.Num() > 0)
				{
					ActiveGlobalTimeDilation->TimeDilationDuration = Keys.Last().Time;
				}
			}

			float CurveAlpha = ActiveGlobalTimeDilation->ElapsedTime / ActiveGlobalTimeDilation->TimeDilationDuration;
			float CurveValue = FAlphaBlend::AlphaToBlendOption(CurveAlpha, ActiveGlobalTimeDilation->BlendMode, ActiveGlobalTimeDilation->CustomTimeDilationCurve);

			CurveValue = FMath::GetMappedRangeValueClamped(FVector2D(0.0f, 1.0f), FVector2D(ActiveGlobalTimeDilation->TimeDilation, 1.0f), CurveValue);
			UGameplayStatics::SetGlobalTimeDilation(GetWorld(), CurveValue);
		}
		else
		{
			UGameplayStatics::SetGlobalTimeDilation(GetWorld(), ActiveGlobalTimeDilation->TimeDilation);
		}
		
		ActiveGlobalTimeDilation->ElapsedTime += UnscaledDeltaTime;
		if (ActiveGlobalTimeDilation->ElapsedTime >= ActiveGlobalTimeDilation->TimeDilationDuration)
		{
			UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
			
			delete ActiveGlobalTimeDilation;
			ActiveGlobalTimeDilation = nullptr;
		}
	}
	else
	{
		if (!GlobalTimeDilationQueue.IsEmpty())
		{
			FGASC_TimeDilation_Subsystem_Data DequeuedHitStop;
			if (GlobalTimeDilationQueue.Dequeue(DequeuedHitStop))
			{
				ActiveGlobalTimeDilation = new FGASC_TimeDilation_Subsystem_Data(DequeuedHitStop);
			}
		}
	}
}
