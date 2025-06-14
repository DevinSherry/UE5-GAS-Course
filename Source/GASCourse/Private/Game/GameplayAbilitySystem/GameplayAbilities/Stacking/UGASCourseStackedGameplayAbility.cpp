// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GameplayAbilitySystem/GameplayAbilities/Stacking/UGASCourseStackedGameplayAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemGlobals.h"
#include "Game/GameplayAbilitySystem/GASCourseNativeGameplayTags.h"
#include "Game/GameplayAbilitySystem/AttributeSets/GASC_AbilityStacksAttributeSet.h"

UGASCourseStackedGameplayAbility::UGASCourseStackedGameplayAbility(const FObjectInitializer& ObjectInitializer)
{
	bHasAbilityStacks = true;
	bAutoCommitAbilityOnActivate = false;
}

void UGASCourseStackedGameplayAbility::TryApplyAbilityStacking(EGASCourseStackApplicationType& StackApplicationResults)
{
	uint32 CurrentStackCount = UAbilitySystemBlueprintLibrary::GetActiveGameplayEffectStackCount(ActiveStackingEffectHandle);
	bool bFoundAttribute = false;
	float MaxStackCount = UAbilitySystemBlueprintLibrary::GetFloatAttributeFromAbilitySystemComponent(GetAbilitySystemComponentFromActorInfo(), MaxStackAttribute, bFoundAttribute);
	if (!bFoundAttribute)
	{
		StackApplicationResults = EGASCourseStackApplicationType::StackFail;
		UE_LOG(LogTemp, Warning, TEXT("MaxStackAttribute is invalid, please check your setup"));
		return;
	}

	if (CurrentStackCount >= MaxStackCount)
	{
		StackApplicationResults = EGASCourseStackApplicationType::StackFail;
		return;
	}
	else
	{
		UAbilitySystemGlobals& AbilitySystemGlobals = UAbilitySystemGlobals::Get();
		if (!AbilitySystemGlobals.ShouldIgnoreCooldowns())
		{
			FGameplayEffectSpecHandle StackSpecHandle = MakeOutgoingGameplayEffectSpec(StackDurationEffect, 1.0f);
			ActiveStackingEffectHandle = K2_ApplyGameplayEffectSpecToOwner(StackSpecHandle);
		}
		else
		{
			StackApplicationResults = EGASCourseStackApplicationType::StackSuccess;
		}
	}
}

void UGASCourseStackedGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo,
                                                     const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);
	InitializeStackCountAttributes();
}

bool UGASCourseStackedGameplayAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	bool bHasStackAttribute = false;
	if (!ActorInfo->AvatarActor.IsValid())
	{
		return false;
	}
	
	float CurrentStackAttributeValue = UAbilitySystemBlueprintLibrary::GetFloatAttribute(ActorInfo->AvatarActor.Get(),
		CurrentStackAttribute, bHasStackAttribute);
	if (CurrentStackAttributeValue == 0.0f)
	{
		const FGameplayTag& FailCostTag = UAbilitySystemGlobals::Get().ActivateFailCostTag;
		if (FailCostTag.IsValid())
		{
			FGameplayTag EventTag = Event_AbilityActivation_CantAffordCost;
			FGameplayEventData Payload;
			Payload.Instigator = GetAvatarActorFromActorInfo();
			Payload.OptionalObject = this;
			Payload.EventTag = AbilityActivationFail_CantAffordCost;
			Payload.Target = GetAvatarActorFromActorInfo();
						
			OptionalRelevantTags->AddTag(FailCostTag);
			GetAbilitySystemComponentFromActorInfo()->HandleGameplayEvent(EventTag, &Payload);

			//Haptic feedback for failure.
			InvokeAbilityFailHapticFeedback();
		}
	}
	
	return (bHasStackAttribute && CurrentStackAttributeValue > 0.0f &&
		Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags));
}

void UGASCourseStackedGameplayAbility::InitializeStackCountAttributes()
{
	if (!MaxStackAttribute.IsValid() || !CurrentStackAttribute.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("MaxStackAttribute OR CurrentStackAttribute are invalid, please check your setup"));
		return;
	}
	
	UGameplayEffect* GEStackChange = 
	NewObject<UGameplayEffect>(GetTransientPackage(), FName(TEXT("StackChange")));
	
	GEStackChange->DurationPolicy = EGameplayEffectDurationType::Instant;

	const int32 Index = GEStackChange->Modifiers.Num();
	GEStackChange->Modifiers.SetNum(Index + 2);

	FGameplayModifierInfo& MaxStackInfo = GEStackChange->Modifiers[Index];
	MaxStackInfo.ModifierMagnitude = FScalableFloat(MaxNumberOfStacks.GetValue());
	MaxStackInfo.ModifierOp = EGameplayModOp::Override;

	FGameplayModifierInfo& CurrentStackInfo = GEStackChange->Modifiers[Index + 1];
	CurrentStackInfo.ModifierMagnitude = FScalableFloat(MaxNumberOfStacks.GetValue());
	CurrentStackInfo.ModifierOp = EGameplayModOp::Override;

	MaxStackInfo.Attribute = MaxStackAttribute;
	CurrentStackInfo.Attribute = CurrentStackAttribute;

	FGameplayEffectContext* ContextHandle = UAbilitySystemGlobals::Get().AllocGameplayEffectContext();
	const FGameplayEffectSpecHandle StackEffectHandle = FGameplayEffectSpecHandle(new FGameplayEffectSpec(GEStackChange, FGameplayEffectContextHandle(ContextHandle), 1.0f));
	
	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToSelf(*StackEffectHandle.Data.Get());
}


