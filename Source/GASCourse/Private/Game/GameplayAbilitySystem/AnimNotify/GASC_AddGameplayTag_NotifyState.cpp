// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GameplayAbilitySystem/AnimNotify/GASC_AddGameplayTag_NotifyState.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GASCourse/GASCourseCharacter.h"


void UGASC_AddGameplayTag_NotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                                   float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (GameplayTagsToAdd.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("Empty Gameplay Tag Container used: %s - Consider removing."), *GetPathNameSafe(this));
		return;
	}

	if(AGASCourseCharacter* OwningCharacter = Cast<AGASCourseCharacter>(MeshComp->GetOwner()))
	{
		bGameplayTagsAddedSuccessfully = UAbilitySystemBlueprintLibrary::AddLooseGameplayTags(OwningCharacter, GameplayTagsToAdd, bReplicatedTags);
		if (bGameplayTagsAddedSuccessfully)
		{
			return;
		}
		
		UE_LOG(LogTemp, Warning, TEXT("Tags failed to be added to %s, in %s."),
			*GetPathNameSafe(OwningCharacter) , *GetPathNameSafe(this));
	}
}

void UGASC_AddGameplayTag_NotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	if (bGameplayTagsAddedSuccessfully)
	{
		if (AGASCourseCharacter* OwningCharacter = Cast<AGASCourseCharacter>(MeshComp->GetOwner()))
		{
			bGameplayTagsRemovedSuccessfully = UAbilitySystemBlueprintLibrary::RemoveLooseGameplayTags(OwningCharacter, GameplayTagsToAdd, bReplicatedTags);
			if (bGameplayTagsRemovedSuccessfully)
			{
				return;
			}
			
			UE_LOG(LogTemp, Warning, TEXT("Tags failed to be removed from %s, in %s."),
				*GetPathNameSafe(OwningCharacter) , *GetPathNameSafe(this));
		}
	}
}
