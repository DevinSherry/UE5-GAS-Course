// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GameplayAbilitySystem/GameplayAbilities/Duration/GASCourseDurationGameplayAbility.h"
#include "GASCourse.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEffectRemoved.h"

UGASCourseDurationGameplayAbility::UGASCourseDurationGameplayAbility(const FObjectInitializer& ObjectInitializer)
{
	//EGASCourseAbilityType::Duration initialized variables
	bAutoEndAbilityOnDurationEnd = true;
	bAutoCommitCooldownOnDurationEnd = true;
	bAutoApplyDurationEffect = true;
}

void UGASCourseDurationGameplayAbility::DurationEffectRemoved(
	const FGameplayEffectRemovalInfo& GameplayEffectRemovalInfo)
{
	if(HasAuthorityOrPredictionKey(CurrentActorInfo, &CurrentActivationInfo))
	{
		if(bAutoCommitCooldownOnDurationEnd)
		{
			CommitAbilityCooldown(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
		}
		
		if(bAutoEndAbilityOnDurationEnd)
		{
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		}
	}
}

UGameplayEffect* UGASCourseDurationGameplayAbility::GetDurationGameplayEffect() const
{
	if (DurationEffect )
	{
		return DurationEffect->GetDefaultObject<UGameplayEffect>();
	}

	return nullptr;
}

bool UGASCourseDurationGameplayAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
                                                           const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UGASCourseDurationGameplayAbility::SetCanBeCanceled(bool bCanBeCanceled)
{
	Super::SetCanBeCanceled(bCanBeCanceled);
}

void UGASCourseDurationGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);
}

void UGASCourseDurationGameplayAbility::OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilitySpec& Spec)
{
	Super::OnRemoveAbility(ActorInfo, Spec);
}

void UGASCourseDurationGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{

	if(bAutoApplyDurationEffect)
	{
		if(!ApplyDurationEffect())
		{
			EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		}
	}
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UGASCourseDurationGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

bool UGASCourseDurationGameplayAbility::CheckCost(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CheckCost(Handle, ActorInfo, OptionalRelevantTags);
}

void UGASCourseDurationGameplayAbility::ApplyCost(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	Super::ApplyCost(Handle, ActorInfo, ActivationInfo);
}

FGameplayEffectContextHandle UGASCourseDurationGameplayAbility::MakeEffectContext(
	const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo) const
{
	return Super::MakeEffectContext(Handle, ActorInfo);
}

void UGASCourseDurationGameplayAbility::ApplyAbilityTagsToGameplayEffectSpec(FGameplayEffectSpec& Spec,
	FGameplayAbilitySpec* AbilitySpec) const
{
	Super::ApplyAbilityTagsToGameplayEffectSpec(Spec, AbilitySpec);
}

bool UGASCourseDurationGameplayAbility::DoesAbilitySatisfyTagRequirements(
	const UAbilitySystemComponent& AbilitySystemComponent, const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::DoesAbilitySatisfyTagRequirements(AbilitySystemComponent, SourceTags, TargetTags,
	                                                OptionalRelevantTags);
}

bool UGASCourseDurationGameplayAbility::CommitAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	FGameplayTagContainer* OptionalRelevantTags)
{
	return Super::CommitAbility(Handle, ActorInfo, ActivationInfo, OptionalRelevantTags);
}

void UGASCourseDurationGameplayAbility::CommitExecute(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if(bAutoCommitCooldownOnDurationEnd)
	{
		//Only Apply Cost, don't apply cooldown.
		ApplyCost(Handle,ActorInfo, ActivationInfo);
	}
	else
	{
		Super::CommitExecute(Handle, ActorInfo, ActivationInfo);
	}
}

void UGASCourseDurationGameplayAbility::ApplyCooldown(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	Super::ApplyCooldown(Handle, ActorInfo, ActivationInfo);
}

void UGASCourseDurationGameplayAbility::OnPawnAvatarSet()
{
	Super::OnPawnAvatarSet();
}

bool UGASCourseDurationGameplayAbility::ApplyDurationEffect()
{
	bool bSuccess = false;
	
	if((DurationEffect) && HasAuthorityOrPredictionKey(CurrentActorInfo, &CurrentActivationInfo))
	{
		if(const UGameplayEffect* DurationEffectObject = DurationEffect.GetDefaultObject())
		{
			if(DurationEffectObject->DurationPolicy == EGameplayEffectDurationType::HasDuration)
			{
				//TODO: Apply Player Level Info Here
				DurationEffectHandle = ApplyGameplayEffectToOwner(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, DurationEffectObject, 1.0);
				if(DurationEffectHandle.WasSuccessfullyApplied())
				{
					bSuccess = true;
					
					if(UAbilityTask_WaitGameplayEffectRemoved* DurationEffectRemovalTask = UAbilityTask_WaitGameplayEffectRemoved::WaitForGameplayEffectRemoved(this, DurationEffectHandle))
					{
						DurationEffectRemovalTask->OnRemoved.AddDynamic(this, &UGASCourseDurationGameplayAbility::DurationEffectRemoved);
						DurationEffectRemovalTask->Activate();
					} 
				}
				return bSuccess;
			}
			UE_LOG(LogBlueprint, Error, TEXT("%s: SUPPLIED GAMEPLAY EFFECT {%s} HAS INVALID DURATION POLICY {%s}."),*GASCOURSE_CUR_CLASS_FUNC, *DurationEffectObject->GetName(), *UEnum::GetValueAsString(DurationEffectObject->DurationPolicy));
			return bSuccess;
		}
	}
	
	UE_LOG(LogBlueprint, Error, TEXT("%s: NO VALID DURATION EFFECT DEFINED IN DEFAULT SETTINGS"),*GASCOURSE_CUR_CLASS_FUNC);
	return bSuccess;
}

void UGASCourseDurationGameplayAbility::GetAbilityDurationTags(FGameplayTagContainer& DurationTags) const
{
	DurationTags.Reset();
	if(const UGameplayEffect* DurationGE = GetDurationGameplayEffect())
	{
		DurationTags.AppendTags(DurationGE->GetGrantedTags());
	}
}
