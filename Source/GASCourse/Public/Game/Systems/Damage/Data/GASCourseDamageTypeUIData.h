// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "GASCourseDamageTypeUIData.generated.h"

/**
 * @brief UGASCourseDamageTypeColor is a Data Asset used to map damage types to corresponding colors.
 *
 * This class serves as a container for associating damage types, represented by FGameplayTag objects,
 * with specific colors represented by FLinearColor objects. It is designed to be utilized for visual representation
 * of damage types within the game, such as coloring text or UI elements based on the type of damage.
 */
UCLASS(BlueprintType)
class GASCOURSE_API UGASCourseDamageTypeUIData : public UDataAsset
{
	GENERATED_BODY()

public:
	/**
	 * @brief The DamageTypeColors variable is a TMap that maps FGameplayTag keys to FColor values.
	 *
	 * The TMap<FGameplayTag, FColor> DamageTypeColors is an editable variable that can be modified in Blueprint
	 * or in the Unreal Editor. It falls under the "Some Category" category.
	 *
	 * The keys of the TMap are FGameplayTag objects, representing different types of damage.
	 * The values of the TMap are FColor objects, representing the color associated with each damage type.
	 *
	 * This variable can be used to define different colors for different types of damage in a game.
	 * For example, if there are damage types like "Physical", "Fire", and "Electric", you can assign
	 * a specific color to each of them using the DamageTypeColors variable. Later on, you can retrieve
	 * the color associated with a specific damage type by accessing the TMap using the corresponding
	 * FGameplayTag as the key.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GASCourse|Damage")
	TMap<FGameplayTag, FLinearColor> DamageTypeColors;

	UFUNCTION(BlueprintPure,  Category = "GASCourse|Damage", meta=(Categories="Damage.Type"))
	FLinearColor GetDamageTypeColor(FGameplayTag DamageType);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GASCourse|Damage")
	FLinearColor CriticalDamageColor = FLinearColor::Red;

	/**
	 * @brief Contains the localized text representing critical damage in the game.
	 *
	 * This variable is an editable and blueprint-accessible FText used to display text
	 * for scenarios involving critical damage. The default value is localized to "Critical"
	 * and can be configured or localized for different languages and regions.
	 *
	 * Designed for use in UI or gameplay feedback to indicate critical damage events,
	 * providing consistent and customizable text representation within the game.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GASCourse|Damage")
	FText CriticalDamageText = NSLOCTEXT("GASCourse", "CriticalDamageText", "Critical");
	
};
