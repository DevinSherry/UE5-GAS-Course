// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Character/Components/InputBuffer/GASC_InputBuffer_NotifyState.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Animation/AnimNotifyLibrary.h"
#include "Game/Character/Components/InputBuffer/GASC_InputBufferComponent.h"
#include "Game/Character/Player/GASCoursePlayerCharacter.h"


void UGASC_InputBuffer_NotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                                float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	if (AGASCoursePlayerCharacter* OwningCharacter = Cast<AGASCoursePlayerCharacter>(MeshComp->GetOwner()))
	{
		if (BlockingTags.IsEmpty())
		{
			return;
		}
		
		UAbilitySystemBlueprintLibrary::AddLooseGameplayTags(OwningCharacter, BlockingTags);
	}
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
}

void UGASC_InputBuffer_NotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	float CurrentNotifyStatePercentage = UAnimNotifyLibrary::GetCurrentAnimationNotifyStateTimeRatio(EventReference);
	if (CurrentNotifyStatePercentage >= OpenInputBufferTimePercentage)
	{
		if (AGASCoursePlayerCharacter* OwningCharacter = Cast<AGASCoursePlayerCharacter>(MeshComp->GetOwner()))
		{
			if (UGASC_InputBufferComponent* InputBufferComponent = OwningCharacter->GetInputBufferComponent())
			{
				if (!InputBufferComponent->IsInputBufferOpen())
				{
					InputBufferComponent->OpenInputBuffer();
				}
			}
		}
	}
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
}

void UGASC_InputBuffer_NotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	if (AGASCoursePlayerCharacter* OwningCharacter = Cast<AGASCoursePlayerCharacter>(MeshComp->GetOwner()))
	{
		if (BlockingTags.IsEmpty())
		{
			return;
		}
		
		UAbilitySystemBlueprintLibrary::RemoveLooseGameplayTags(OwningCharacter, BlockingTags);
		if (UGASC_InputBufferComponent* InputBufferComponent = OwningCharacter->GetInputBufferComponent())
		{
			InputBufferComponent->CloseInputBuffer();
		}
	}
	Super::NotifyEnd(MeshComp, Animation, EventReference);
}

FString UGASC_InputBuffer_NotifyState::GetNotifyName_Implementation() const
{
	FString NotifyName = TEXT("Input Buffer");

	NotifyName.ReplaceInline(TEXT("AnimNotifyState_"), TEXT(""), ESearchCase::CaseSensitive);
	
	FString Suffix = FString::Format(TEXT(" -> Open Input Buffer at: {0}%"), {FString::FromInt(OpenInputBufferTimePercentage * 100.0f)});
	NotifyName.Append(Suffix);
	
	return NotifyName;
}
