// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "GASC_AddGameplayTag_NotifyState.generated.h"

/**
 * UGASC_AddGameplayTag_NotifyState is a custom animation notify state
 * that allows for adding or manipulating gameplay tags on the owner
 * of an animation during specific phases of the animation's lifecycle.
 */
UCLASS()
class GASCOURSE_API UGASC_AddGameplayTag_NotifyState : public UAnimNotifyState
{
	GENERATED_BODY()
	
	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference);
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, const FAnimNotifyEventReference& EventReference);
	
public:
	/** Tags to apply to the owner of the animation */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayTag")
	FGameplayTagContainer GameplayTagsToAdd;

	/**
	 * bReplicatedTags is a boolean property that determines whether gameplay tags
	 * associated with this object are replicated across the network.
	 * When set to true, the tags will be synchronized for all clients.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayTag")
	bool bReplicatedTags = false;

private:

	bool bGameplayTagsAddedSuccessfully = false;
	bool bGameplayTagsRemovedSuccessfully = false;
};
