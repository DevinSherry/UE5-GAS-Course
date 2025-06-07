// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/HUD/Damage/GASC_UI_DamageNumber.h"
#include "Components/TextBlock.h"
#include "Engine/StreamableManager.h"
#include "Game/BlueprintLibraries/GameplayAbilitySystem/GASCourseASCBlueprintLibrary.h"
#include "Engine/AssetManager.h"


void UGASC_UI_DamageNumber::NativeConstruct()
{
	Super::NativeConstruct();

	
	SCOPED_NAMED_EVENT(DamageNumberConstruct, FColor::Red);
	const UGASC_AbilitySystemSettings* Settings = UGASC_AbilitySystemSettings::Get();
	
	TSoftObjectPtr<UGASCourseDamageTypeUIData> AssetRef = Settings->DamageTypeUIData;

	if (!AssetRef.IsValid() && AssetRef.IsNull() == false)
	{
		FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
		Streamable.RequestAsyncLoad(AssetRef.ToSoftObjectPath(),
			FStreamableDelegate::CreateLambda([AssetRef, this]()
			{
				if (UGASCourseDamageTypeUIData* LoadedAsset = AssetRef.Get())
				{
					UE_LOG(LogTemp, Log, TEXT("Loaded asset asynchronously: %s"), *LoadedAsset->GetName());
					DamageTypeUIData = LoadedAsset;
					
					if (bIsCriticalHit)
					{
						SetCriticalHitText();
					}
					else
					{
						SetDamageTextValue();
					}
				}
			}));
	}
	else if (AssetRef.IsValid())
	{
		// Already loaded
		DamageTypeUIData = AssetRef.Get();
		
		if (bIsCriticalHit)
		{
			SetCriticalHitText();
		}
		else
		{
			SetDamageTextValue();
		}
	}

}

void UGASC_UI_DamageNumber::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UGASC_UI_DamageNumber::SetDamageTextValue_Implementation()
{
	if (!DamageData.Instigator)
	{
		UE_LOG(LogTemp, Warning, TEXT("DamageData.Instigator is NULL"));
		return;
	}
	
	if (!DamageTypeUIData)
	{
		UE_LOG(LogTemp, Warning, TEXT("DamageTypeColorData is NULL"));
		return;
	}

	float DamageValue = DamageData.EventMagnitude;
	if (DamageData.InstigatorTags.HasTag(DamageType_Healing))
	{
		FText HealingText = FText::FromString(TEXT("+" + FString::FromInt(DamageValue) + " "));
		DamageText->SetText(HealingText);
	}
	else
	{
		DamageText->SetText(FText::AsNumber(DamageValue));
	}
	
	FLinearColor DamageTextColor = FLinearColor::Gray;
	if (DamageData.InstigatorTags.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("DamageData.InstigatorTags is EMPTY"));
	}
	FGameplayTag DamageTypeTag = FGameplayTag::EmptyTag;
	UGASCourseASCBlueprintLibrary::FindDamageTypeTagInContainer(DamageData.InstigatorTags, DamageTypeTag);
	if (!DamageTypeTag.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("DamageTypeTag is INVALID"));
		return;
	}
	DamageTextColor = DamageTypeUIData->GetDamageTypeColor(DamageTypeTag);
	DamageText->SetColorAndOpacity(DamageTextColor);
}

void UGASC_UI_DamageNumber::SetCriticalHitText_Implementation()
{
	if (!DamageTypeUIData)
	{
		UE_LOG(LogTemp, Warning, TEXT("DamageTypeColorData is NULL"));
		return;
	}
	if (DamageText)
	{
		DamageText->SetColorAndOpacity(DamageTypeUIData->CriticalDamageColor);
		DamageText->SetText(DamageTypeUIData->CriticalDamageText);
	}
}
