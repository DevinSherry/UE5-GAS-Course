// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GameplayAbilitySystem/Tasks/AbilityTask_WaitForDurationEffectChange.h"

UAbilityTask_WaitOnDurationChange* UAbilityTask_WaitOnDurationChange::WaitOnDurationChange(UAbilitySystemComponent* InAbilitySystemComponent,FGameplayTagContainer InDurationTags, float InDurationInterval, bool bInUseServerCooldown)
{
	UAbilityTask_WaitOnDurationChange* MyObj = NewObject<UAbilityTask_WaitOnDurationChange>();
	MyObj->WorldContext = GEngine->GetWorldFromContextObjectChecked(InAbilitySystemComponent);
	MyObj->ASC = InAbilitySystemComponent;
	MyObj->DurationTags = InDurationTags;
	MyObj->DurationInterval = InDurationInterval;
	MyObj->bUseServerCooldown = bInUseServerCooldown;


	if(!IsValid(InAbilitySystemComponent) || InDurationTags.Num() < 1)
	{
		MyObj->EndTask();
		return nullptr;
	}

	InAbilitySystemComponent->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(MyObj, &UAbilityTask_WaitForDurationEffectChange::OnActiveGameplayEffectAddedCallback);

	TArray<FGameplayTag> DurationTagArray;
	InDurationTags.GetGameplayTagArray(DurationTagArray);

	for(const FGameplayTag DurationTag : DurationTagArray)
	{
		InAbilitySystemComponent->RegisterGameplayTagEvent(DurationTag, EGameplayTagEventType::NewOrRemoved).AddUObject(MyObj, &UAbilityTask_WaitForDurationEffectChange::DurationTagChanged);
	}
	
	return MyObj;
}

UAbilityTask_WaitOnCooldownChange* UAbilityTask_WaitOnCooldownChange::WaitOnCooldownChange(
	UAbilitySystemComponent* InAbilitySystemComponent, FGameplayTagContainer InCooldownTags, float InDurationInterval,
	bool bInUseServerCooldown)
{
	UAbilityTask_WaitOnCooldownChange* MyObj = NewObject<UAbilityTask_WaitOnCooldownChange>();
	MyObj->WorldContext = GEngine->GetWorldFromContextObjectChecked(InAbilitySystemComponent);
	MyObj->ASC = InAbilitySystemComponent;
	MyObj->DurationTags = InCooldownTags;
	MyObj->DurationInterval = InDurationInterval;
	MyObj->bUseServerCooldown = bInUseServerCooldown;


	if(!IsValid(InAbilitySystemComponent) || InCooldownTags.Num() < 1)
	{
		MyObj->EndTask();
		return nullptr;
	}

	InAbilitySystemComponent->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(MyObj, &UAbilityTask_WaitForDurationEffectChange::OnActiveGameplayEffectAddedCallback);

	TArray<FGameplayTag> DurationTagArray;
	InCooldownTags.GetGameplayTagArray(DurationTagArray);

	for(const FGameplayTag DurationTag : DurationTagArray)
	{
		InAbilitySystemComponent->RegisterGameplayTagEvent(DurationTag, EGameplayTagEventType::NewOrRemoved).AddUObject(MyObj, &UAbilityTask_WaitForDurationEffectChange::DurationTagChanged);
	}
	
	return MyObj;
}

void UAbilityTask_WaitForDurationEffectChange::EndTask()
{
	if(IsValid(ASC))
	{
		ASC->OnActiveGameplayEffectAddedDelegateToSelf.RemoveAll(this);

		TArray<FGameplayTag> DurationTagArray;
		DurationTags.GetGameplayTagArray(DurationTagArray);

		for(const FGameplayTag DurationTag :DurationTagArray)
		{
			ASC->RegisterGameplayTagEvent(DurationTag, EGameplayTagEventType::NewOrRemoved).RemoveAll(this);
		}
	}

	SetReadyToDestroy();
	MarkAsGarbage();
}

void UAbilityTask_WaitForDurationEffectChange::OnActiveGameplayEffectAddedCallback(UAbilitySystemComponent* InTargetASC,
	const FGameplayEffectSpec& InSpecApplied, FActiveGameplayEffectHandle ActiveHandle)
{
	FGameplayTagContainer AssetTags;
	InSpecApplied.GetAllAssetTags(AssetTags);

	FGameplayTagContainer GrantedTags;
	InSpecApplied.GetAllGrantedTags(GrantedTags);

	TArray<FGameplayTag> DurationTagArray;
	DurationTags.GetGameplayTagArray(DurationTagArray);

	for(FGameplayTag DurationTag : DurationTagArray)
	{
		if(AssetTags.HasTagExact(DurationTag) || GrantedTags.HasTagExact(DurationTag))
		{
			float TimeRemaining = 0.0f;
			float Duration = 0.0f;

			const FGameplayTagContainer DurationTagContainer(GrantedTags.GetByIndex(0));
			GetCooldownRemainingForTag(DurationTagContainer, TimeRemaining, Duration);

			if (ASC->GetOwnerRole() == ROLE_Authority)
			{
				// Player is Server
				OnDurationBegin.Broadcast(DurationTag, TimeRemaining, Duration);
			}
			else if (!bUseServerCooldown && InSpecApplied.GetContext().GetAbilityInstance_NotReplicated())
			{
				// Client using predicted cooldown
				OnDurationBegin.Broadcast(DurationTag, TimeRemaining, Duration);
			}
			else if (bUseServerCooldown && InSpecApplied.GetContext().GetAbilityInstance_NotReplicated() == nullptr)
			{
				// Client using Server's cooldown. This is Server's corrective cooldown GE.
				OnDurationBegin.Broadcast(DurationTag, TimeRemaining, Duration);
			}
			else if (bUseServerCooldown && InSpecApplied.GetContext().GetAbilityInstance_NotReplicated())
			{
				// Client using Server's cooldown but this is predicted cooldown GE.
				// This can be useful to gray out abilities until Server's cooldown comes in.
				OnDurationBegin.Broadcast(DurationTag, -1.0f, -1.0f);
			}
			
			if(WorldContext)
			{
				WorldContext->GetWorld()->GetTimerManager().SetTimer(DurationTimeUpdateTimerHandle, this, &UAbilityTask_WaitForDurationEffectChange::OnDurationUpdate, DurationInterval, true);
			}

		}
	}
}

void UAbilityTask_WaitForDurationEffectChange::DurationTagChanged(const FGameplayTag InDurationTag, int32 InNewCount)
{
	if(InNewCount == 0)
	{
		OnDurationEnd.Broadcast(InDurationTag, -1.0f, -1.0f);
		if(WorldContext)
		{
			WorldContext->GetWorld()->GetTimerManager().ClearTimer(DurationTimeUpdateTimerHandle);
			WorldContext->GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
		}
	}
}

bool UAbilityTask_WaitForDurationEffectChange::GetCooldownRemainingForTag(const FGameplayTagContainer& InDurationTags,
	float& TimeRemaining, float& InDuration) const
{
	if(IsValid(ASC) && InDurationTags.Num() > 0)
	{
		TimeRemaining = 0.0f;
		InDuration = 0.0f;

		FGameplayEffectQuery const Query = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(InDurationTags);
		TArray< TPair<float, float> > DurationAndTimeRemaining = ASC->GetActiveEffectsTimeRemainingAndDuration(Query);
		if(DurationAndTimeRemaining.Num() > 0)
		{
			int32 BestIndex = 0;
			float LongestTime = DurationAndTimeRemaining[0].Key;
			for(int32 Index = 1; Index < DurationAndTimeRemaining.Num(); ++Index)
			{
				if(DurationAndTimeRemaining[Index].Key >LongestTime)
				{
					LongestTime = DurationAndTimeRemaining[Index].Key;
					BestIndex = Index;
				}
			}

			TimeRemaining = DurationAndTimeRemaining[BestIndex].Key;
			InDuration = DurationAndTimeRemaining[BestIndex].Value;

			return true;
		}
	}

	return false;
}

void UAbilityTask_WaitForDurationEffectChange::OnDurationUpdate()
{
	float TimeRemaining = 0.0f;
	float Duration = 0.0f;
	GetCooldownRemainingForTag(DurationTags, TimeRemaining, Duration);
	OnDurationTimeUpdated.Broadcast(DurationTags.GetByIndex(0), TimeRemaining, Duration);
}

