// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/CheatManager/Extensions/GASC_AbilitySystem_CheatExt.h"
#include "GASCourse/GASCourseCharacter.h"

void UGASC_AbilitySystem_CheatExt::SetGameplayAttribute(FName AttributeName, float Value)
{
	UE_LOG(LogTemp, Warning, TEXT("SetGameplayAttribute: %s %f"), *AttributeName.ToString(), Value);
	APawn* Pawn = GetOuterUCheatManager()->GetOuterAPlayerController()->GetPawn();
	if (Pawn != NULL)
	{
		AGASCourseCharacter* Character =  Cast<AGASCourseCharacter>(Pawn);
		if (Character)
		{
			if (UAbilitySystemComponent* AbilitySystemComponent = Character->GetAbilitySystemComponent())
			{
				TArray<FGameplayAttribute> OutAttributes;
				AbilitySystemComponent->GetAllAttributes(OutAttributes);
				for (FGameplayAttribute Attribute : OutAttributes)
				{
					if (Attribute.AttributeName == AttributeName)
					{
						AbilitySystemComponent->SetNumericAttributeBase(Attribute, Value);
					}
				}
			}
		}
	}
}
