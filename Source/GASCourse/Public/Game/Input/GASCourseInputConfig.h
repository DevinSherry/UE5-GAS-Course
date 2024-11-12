// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "GASCourseInputConfig.generated.h"

class UInputAction;
struct FGameplayTag;

/**
 * Struct used to map an input action to a gameplay input tag.
 */
USTRUCT(BlueprintType)
struct FTaggedInputAction
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const UInputAction> InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (Categories = "InputTag"))
	FGameplayTag InputTag;
};

USTRUCT(BlueprintType)
struct FTaggedAbilityAction
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const UInputAction> InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (Categories = "InputTag"))
	FGameplayTag InputTag;
};


UCLASS()
class GASCOURSE_API UGASCourseInputConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	
	/**
	 * FindInputActionForTag
	 *
	 * Finds and returns the UInputAction associated with the given FGameplayTag.
	 *
	 * @param   InputTag   The FGameplayTag used to search for the UInputAction.
	 * @return             The UInputAction associated with the InputTag, or nullptr if not found.
	 */
	UFUNCTION(BlueprintPure, Category="GASCourse|Input")
	const UInputAction* FindInputActionForTag(const FGameplayTag& InputTag) const;

	/**
	 * FindTaggedAbilityActionForTag
	 *
	 * Finds and returns the UInputAction associated with the given FGameplayTag.
	 *
	 * @param   InputTag   The FGameplayTag used to search for the UInputAction.
	 *
	 * @return             The UInputAction associated with the InputTag, or nullptr if not found.
	 */
	UFUNCTION(BlueprintPure, Category="GASCourse|Input")
	const UInputAction* FindTaggedAbilityActionForTag(const FGameplayTag& InputTag) const;

	/**
	 * Finds and returns the FGameplayTag associated with the given UInputAction.
	 *
	 * @param   InputAction  The UInputAction to search for the associated FGameplayTag.
	 * @return              The FGameplayTag associated with the InputAction.
	 */
	UFUNCTION()
	const FGameplayTag& FindTagForInputAction(const UInputAction* InputAction) const;

	/**
	 * FindTagForAbilityAction
	 *
	 * Finds and returns the FGameplayTag associated with the given UInputAction.
	 *
	 * @param   InputAction   The UInputAction to search for the associated FGameplayTag.
	 * @return               The FGameplayTag associated with the InputAction.
	 */
	UFUNCTION(BlueprintPure, Category="GASCourse|Input")
	const FGameplayTag& FindTagForAbilityAction(const UInputAction* InputAction) const;


public:
	// List of input actions used by the owner. These input actions are mapped to a gameplay tag and must be manually bound.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (TitleProperty = "InputAction"))
	TArray<FTaggedInputAction> TaggedInputActions;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (TitleProperty = "InputAction"))
	TArray<FTaggedAbilityAction> TaggedAbilityActions;
};
