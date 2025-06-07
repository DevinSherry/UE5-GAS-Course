// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/HUD/Damage/GASC_UI_DamageNumberPanel.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Game/Character/Player/GASCoursePlayerController.h"
#include "Game/GameplayAbilitySystem/GASCourseNativeGameplayTags.h"


void UGASC_UI_DamageNumberPanel::NativeConstruct()
{
	Super::NativeConstruct();
	if (AGASCoursePlayerController* PC = Cast<AGASCoursePlayerController>(GetOwningPlayer()))
	{
		PC->OnDamageDealtDelegate.AddDynamic(this, &UGASC_UI_DamageNumberPanel::OnDamageDealt);
	}
}

void UGASC_UI_DamageNumberPanel::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (DamageNumbers.IsEmpty())
	{
		return;
	}
	
	TArray<UGASC_UI_DamageNumber*> DamageNumbersToTrack;
	DamageNumbers.GetKeys(DamageNumbersToTrack);

	for (int i = DamageNumbersToTrack.Num() - 1; i >= 0; i--)
	{
		if (UGASC_UI_DamageNumber* CurrentDamageNumber = DamageNumbersToTrack[i])
		{
			if (CurrentDamageNumber->DamageData.Target)
			{
				FVector StoredDamageNumberPosition = DamageNumbers[CurrentDamageNumber];
				if (UCanvasPanelSlot* AsCanvasPanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(CurrentDamageNumber))
				{
					FVector2D StoredDamageNumberPositionOnScreen = GetDamagePositionOnScreen(StoredDamageNumberPosition, DamageData, *CurrentDamageNumber);
					Position = StoredDamageNumberPositionOnScreen;
					AsCanvasPanelSlot->SetPosition(StoredDamageNumberPositionOnScreen);
				}
			}
		}
	} 
}

FVector2D UGASC_UI_DamageNumberPanel::GetDamagePositionOnScreen(const FVector &InStoredPosition, const FGameplayEventData &StoredPayload, const UGASC_UI_DamageNumber& DamageNumber)
{
	FVector StoredTargetLocation = FVector::ZeroVector;
	FVector2D OutDamageScreenPosition = FVector2D::ZeroVector;
	if (!DamageNumber.IsValidLowLevel())
	{
		UE_LOG(LogTemp, Warning, TEXT("DamageNumber is invalid, returning 0,0"));
		return OutDamageScreenPosition;
	}
	if (const AActor* StoredTarget = StoredPayload.Target)
	{
		StoredTargetLocation = StoredTarget->GetActorLocation();	
	}
	if (APlayerController* PlayerController = GetOwningPlayer())
	{
		float WorldXPosition = StoredTargetLocation.X - InStoredPosition.X;
		float WorldYPosition = StoredTargetLocation.Y - InStoredPosition.Y;
		FVector WorldPosition = FVector(WorldXPosition, WorldYPosition, 0.0f) + InStoredPosition;
		if (UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(PlayerController, WorldPosition, OutDamageScreenPosition, true))
		{
			const FGeometry CachedWidgetGeometry = GetCachedGeometry();
			FVector2D CachedDamageNumberDesiredSize = DamageNumber.GetDesiredSize();
			if (DamageNumber.bIsCriticalHit)
			{
				CachedDamageNumberDesiredSize = CachedDamageNumberDesiredSize / 2.0f;
			}

			OutDamageScreenPosition -= CachedDamageNumberDesiredSize;
			OutDamageScreenPosition *= UWidgetLayoutLibrary::GetViewportScale(this);
			FVector2D FinalDamageOutPosition = FVector2D::ZeroVector;
			USlateBlueprintLibrary::ScreenToWidgetLocal(this, CachedWidgetGeometry, OutDamageScreenPosition, FinalDamageOutPosition, false);
			return FinalDamageOutPosition;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Failed damage number position calculation, returning 0,0"));
	return OutDamageScreenPosition;
}

void UGASC_UI_DamageNumberPanel::OnDamageDealt(const FGameplayEventData& DamagePayloadData)
{
	DamageData = DamagePayloadData;
	AddHitDamageText();
	if (DamageData.InstigatorTags.HasTagExact(DamageType_Critical))
	{
		bIsCriticalDamage = true;
		AddCriticalHitDamageText();
	}
}

void UGASC_UI_DamageNumberPanel::AddCriticalHitDamageText_Implementation()
{
	if (!DamageNumberClass.Get()->IsValidLowLevel())
	{
		UE_LOG(LogTemp, Warning, TEXT("DamageNumberClass is invalid, returning"));
		return;
	}
	if (UGASC_UI_DamageNumber* DamageNumber = CreateWidget<UGASC_UI_DamageNumber>(this, DamageNumberClass))
	{
		DamageNumber->bIsCriticalHit = bIsCriticalDamage;
		DamageNumber->DamageData = DamageData;
		DamageNumber->OnDamageNumberRemovedDelegate.AddDynamic(this, &UGASC_UI_DamageNumberPanel::OnDamageNumberRemoved);
		
		DamageNumberPanel->AddChild(DamageNumber);
		DamageNumber->ForceLayoutPrepass();
		DamageNumbers.Add(DamageNumber, GetDamageNumberPosition(DamageData, *DamageNumber));
	}
}

void UGASC_UI_DamageNumberPanel::AddHitDamageText_Implementation()
{
	if (!DamageNumberClass.Get()->IsValidLowLevel())
	{
		UE_LOG(LogTemp, Warning, TEXT("DamageNumberClass is invalid, returning"));
		return;
	}
	if (UGASC_UI_DamageNumber* DamageNumber = CreateWidget<UGASC_UI_DamageNumber>(this, DamageNumberClass))
	{
		DamageNumber->bIsCriticalHit = false;
		DamageNumber->DamageData = DamageData;
		DamageNumber->OnDamageNumberRemovedDelegate.AddDynamic(this, &UGASC_UI_DamageNumberPanel::OnDamageNumberRemoved);
		
		DamageNumberPanel->AddChild(DamageNumber);
		DamageNumber->ForceLayoutPrepass();
		DamageNumbers.Add(DamageNumber, GetDamageNumberPosition(DamageData, *DamageNumber));
	}
}

void UGASC_UI_DamageNumberPanel::OnDamageNumberRemoved(UGASC_UI_DamageNumber* DamageNumber)
{
	if (DamageNumber)
	{
		DamageNumber->RemoveFromParent();
		DamageNumber->OnDamageNumberRemovedDelegate.RemoveAll(this);
		DamageNumbers.Remove(DamageNumber);
	}
}

FVector UGASC_UI_DamageNumberPanel::GetDamageNumberPosition(const FGameplayEventData& InPayload, const UGASC_UI_DamageNumber& DamageNumber) const
{
	FVector OutDamagePosition = FVector::ZeroVector;
	const AActor* Target = InPayload.Target;

	FGameplayAbilityTargetDataHandle InTargetData = InPayload.TargetData;
	if (UAbilitySystemBlueprintLibrary::TargetDataHasHitResult(InTargetData, 0))
	{
		FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(InTargetData, 0);
		OutDamagePosition = HitResult.ImpactPoint;
	}
	else
	{
		if (Target)
		{
			OutDamagePosition = Target->GetActorLocation();
		}
	}

	if (DamageNumber.bIsCriticalHit)
	{
		FVector2D DamageNumberUISize = DamageNumber.GetDesiredSize();
		return Target->GetActorLocation() + FVector(DamageNumberUISize.X, DamageNumberUISize.Y, 100.0f);
	}
	
	return OutDamagePosition;
}
