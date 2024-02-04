// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectUIData.h"
#include "GASCourseGameplayEffectUIData.generated.h"

/**
 * 
 */
UCLASS()
class GASCOURSE_API UGASCourseGameplayEffectUIData : public UGameplayEffectUIData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Data, meta = (MultiLine = "true"))
	TSoftObjectPtr<UMaterialInterface> StatusIcon;
	
};
