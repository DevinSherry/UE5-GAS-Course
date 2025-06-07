// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Systems/Damage/Data/GASCourseDamageTypeUIData.h"

FLinearColor UGASCourseDamageTypeUIData::GetDamageTypeColor(FGameplayTag DamageType)
{
	if(DamageType.IsValid())
	{
		return *DamageTypeColors.Find(DamageType);
	}

	return FLinearColor::White;
}
