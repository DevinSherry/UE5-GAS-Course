// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "GASCourseDamageTypeColor.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class GASCOURSE_API UGASCourseDamageTypeColor : public UDataAsset
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
	
};
