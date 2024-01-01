// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Systems/Damage/Data/GASCourseDamageTypeColor.h"

FLinearColor UGASCourseDamageTypeColor::GetDamageTypeColor(FGameplayTag DamageType)
{
	if(DamageType.IsValid())
	{
		return *DamageTypeColors.Find(DamageType);
	}

	return FLinearColor::White;
}
