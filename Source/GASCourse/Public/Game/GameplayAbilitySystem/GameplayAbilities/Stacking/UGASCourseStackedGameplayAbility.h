// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Game/GameplayAbilitySystem/GASCourseGameplayAbility.h"
#include "UGASCourseStackedGameplayAbility.generated.h"

UENUM(BlueprintType)
enum class EGASCourseStackApplicationType : uint8
{
	StackSuccess,
	StackFail
};

/**
 * UGASCourseStackedGameplayAbility is a specialized gameplay ability class that extends the functionality
 * of UGASCourseGameplayAbility. This class introduces stacking mechanics for abilities that support it,
 * enabling dynamic behavior based on stack counts and related attributes.
 */
UCLASS()
class GASCOURSE_API UGASCourseStackedGameplayAbility : public UGASCourseGameplayAbility
{
	GENERATED_BODY()
	
	UGASCourseStackedGameplayAbility(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category = "GASCourse|StackedGameplayAbility", meta=(ExpandEnumAsExecs = "StackApplicationResults"))
	void TryApplyAbilityStacking(EGASCourseStackApplicationType& StackApplicationResults);

protected:
	
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

private:

	void InitializeStackCountAttributes();

	FActiveGameplayEffectHandle ActiveStackingEffectHandle;
	
};
