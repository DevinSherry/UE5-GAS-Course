// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GASC_UI_DamageNumber.h"
#include "Blueprint/UserWidget.h"
#include "GASC_UI_DamageNumberPanel.generated.h"

/**
 * @class UGASC_UI_DamageNumberPanel
 * @brief This class handles displaying damage numbers in the user interface for the GASCourse game.
 *
 * UGASC_UI_DamageNumberPanel is responsible for managing and displaying damage numbers,
 * including critical hits, when damage is dealt in the game. It utilizes a UCanvasPanel
 * to add and manage individual damage number widgets, which are instances of UGASC_UI_DamageNumber.
 */
UCLASS()
class GASCOURSE_API UGASC_UI_DamageNumberPanel : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category=Damage, meta=(BindWidget))
	class UCanvasPanel* DamageNumberPanel;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category=Damage)
	TMap<UGASC_UI_DamageNumber*, FVector> DamageNumbers;

	UFUNCTION()
	void OnDamageDealt(const FGameplayEventData& DamagePayloadData);

	UFUNCTION(BlueprintNativeEvent)
	void AddCriticalHitDamageText();

	UFUNCTION(BlueprintNativeEvent)
	void AddHitDamageText();

	UFUNCTION()
	void OnDamageNumberRemoved(UGASC_UI_DamageNumber* DamageNumber);

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category=Damage)
	TSubclassOf<UGASC_UI_DamageNumber> DamageNumberClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category=Damage)
	FVector2D Position;

private:

	bool bIsCriticalDamage = false;
	
	FGameplayEventData DamageData;

	FVector GetDamageNumberPosition(const FGameplayEventData& InPayload, const UGASC_UI_DamageNumber& DamageNumber) const;
	FVector2D GetDamagePositionOnScreen(const FVector &InStoredPosition, const FGameplayEventData &StoredPayload, const UGASC_UI_DamageNumber& DamageNumber);
};
