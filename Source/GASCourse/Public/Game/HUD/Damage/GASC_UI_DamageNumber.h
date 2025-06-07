// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Abilities/GameplayAbilityTypes.h"
#include "Blueprint/UserWidget.h"
#include "Game/Systems/Damage/Data/GASCourseDamageTypeUIData.h"
#include "GASC_UI_DamageNumber.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDamageNumberRemoved, UGASC_UI_DamageNumber*, DamageNumber);

/**
 * @class UGASC_UI_DamageNumber
 * @brief A User Interface widget class responsible for displaying dynamic damage numbers in the game.
 *
 * This class extends UUserWidget to handle the visual representation of damage numbers displayed
 * when damage is applied to a target. It supports both regular and critical hit damage numbers,
 * with customizable text and colors based on the type of damage dealt.
 *
 * The widget can be bound to a text block to display damage values and includes functionality
 * for dynamic text changes and critical hit effects.
 */
UCLASS()
class GASCOURSE_API UGASC_UI_DamageNumber : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;
	
public:

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta=(BindWidget))
	class UTextBlock* DamageText;

	UPROPERTY(BlueprintReadOnly, Category = "DamageNumber", meta=(ExposeOnSpawn=true))
	FGameplayEventData DamageData;

	UPROPERTY(BlueprintReadOnly, Category = "DamageNumber", meta=(ExposeOnSpawn=true))
	bool bIsCriticalHit = false;

	UPROPERTY()
	UGASCourseDamageTypeUIData* DamageTypeUIData;

	UFUNCTION(BlueprintNativeEvent, Category = "DamageNumber")
	void SetDamageTextValue();

	UFUNCTION(BlueprintNativeEvent, Category = "DamageNumber")
	void SetCriticalHitText();
	
	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnDamageNumberRemoved OnDamageNumberRemovedDelegate;
	
};
