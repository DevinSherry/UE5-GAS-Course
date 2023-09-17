// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GameplayAbilitySystem/GASCourseGameplayAbility.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Game/GameplayAbilitySystem/GASCourseNativeGameplayTags.h"
#include "GASCourse/GASCourse.h"
#include "GameplayEffect.h"
#include "GameplayEffectTypes.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEffectRemoved.h"


UGASCourseGameplayAbility::UGASCourseGameplayAbility(const FObjectInitializer& ObjectInitializer)
{
	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateNo;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	NetSecurityPolicy = EGameplayAbilityNetSecurityPolicy::ClientOrServer;

	ActivationPolicy = EGASCourseAbilityActivationPolicy::OnInputTriggered;
	AbilityType = EGASCourseAbilityType::Instant;

	//EGASCourseAbilityType::Duration initialized variables
	bAutoEndAbilityOnDurationEnd = true;
	bAutoCommitCooldownOnDurationEnd = true;

	bAutoCommitAbilityOnActivate = true;
}

UGASCourseAbilitySystemComponent* UGASCourseGameplayAbility::GetGASCourseAbilitySystemComponentFromActorInfo() const
{
	return (CurrentActorInfo ? Cast<UGASCourseAbilitySystemComponent>(CurrentActorInfo->AbilitySystemComponent.Get()) : nullptr);
}

AGASCoursePlayerController* UGASCourseGameplayAbility::GetGASCoursePlayerControllerFromActorInfo() const
{
	return (CurrentActorInfo ? Cast<AGASCoursePlayerController>(CurrentActorInfo->PlayerController.Get()) : nullptr);
}

AController* UGASCourseGameplayAbility::GetControllerFromActorInfo() const
{
	if (CurrentActorInfo)
	{
		if (AController* PC = CurrentActorInfo->PlayerController.Get())
		{
			return PC;
		}

		// Look for a player controller or pawn in the owner chain.
		AActor* TestActor = CurrentActorInfo->OwnerActor.Get();
		while (TestActor)
		{
			if (AController* C = Cast<AController>(TestActor))
			{
				return C;
			}

			if (const APawn* Pawn = Cast<APawn>(TestActor))
			{
				return Pawn->GetController();
			}

			TestActor = TestActor->GetOwner();
		}
	}

	return nullptr;
}

AGASCoursePlayerCharacter* UGASCourseGameplayAbility::GetGASCouresPlayerCharacterFromActorInfo() const
{
	return (CurrentActorInfo ? Cast<AGASCoursePlayerCharacter>(CurrentActorInfo->AvatarActor.Get()) : nullptr);
}

void UGASCourseGameplayAbility::TryActivateAbilityOnSpawn(const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilitySpec& Spec) const
{
}

void UGASCourseGameplayAbility::DurationEffectRemoved(const FGameplayEffectRemovalInfo& GameplayEffectRemovalInfo)
{
	if(HasAuthorityOrPredictionKey(CurrentActorInfo, &CurrentActivationInfo))
	{
		if(AbilityType == EGASCourseAbilityType::Duration)
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
}

UGameplayEffect* UGASCourseGameplayAbility::GetDurationGameplayEffect() const
{
	if ( DurationEffect )
	{
		return DurationEffect->GetDefaultObject<UGameplayEffect>();
	}
	else
	{
		return nullptr;
	}
}

bool UGASCourseGameplayAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                   const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
                                                   const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!ActorInfo || !ActorInfo->AbilitySystemComponent.IsValid())
	{
		return false;
	}

	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	return true;
}

void UGASCourseGameplayAbility::SetCanBeCanceled(bool bCanBeCanceled)
{
	Super::SetCanBeCanceled(bCanBeCanceled);
}

void UGASCourseGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);
	K2_OnAbilityAdded();
	TryActivateAbilityOnSpawn(ActorInfo, Spec);

	//Maybe here wait for tag removal of cooldown tags?
	//GetCooldownTags();
}

void UGASCourseGameplayAbility::OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilitySpec& Spec)
{
	K2_OnAbilityRemoved();
	Super::OnRemoveAbility(ActorInfo, Spec);
}

void UGASCourseGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	if(HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		if(!ActorInfo->IsNetAuthority())
		{
			return;
		}
	}

	switch (AbilityType)
	{
	case EGASCourseAbilityType::Duration:
		if(bAutoApplyDurationEffect)
		{
			if(!ApplyDurationEffect())
			{
				EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
			}
		}
	case EGASCourseAbilityType::Instant:
		{
			break;
		}
	case EGASCourseAbilityType::AimCast:
		{
			break;
		}
	default:
		break;
	}

	if(bAutoCommitAbilityOnActivate)
	{
		CommitAbility(Handle, ActorInfo, ActivationInfo);
	}
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UGASCourseGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

bool UGASCourseGameplayAbility::CheckCost(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CheckCost(Handle, ActorInfo, OptionalRelevantTags);
}

void UGASCourseGameplayAbility::ApplyCost(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	Super::ApplyCost(Handle, ActorInfo, ActivationInfo);

	check(ActorInfo);

	// Used to determine if the ability actually hit a target (as some costs are only spent on successful attempts)
	auto DetermineIfAbilityHitTarget = [&]()
	{
		if (ActorInfo->IsNetAuthority())
		{
			if (UGASCourseAbilitySystemComponent* ASC = Cast<UGASCourseAbilitySystemComponent>(ActorInfo->AbilitySystemComponent.Get()))
			{
				FGameplayAbilityTargetDataHandle TargetData;
				ASC->GetAbilityTargetData(Handle, ActivationInfo, TargetData);
				for (int32 TargetDataIdx = 0; TargetDataIdx < TargetData.Data.Num(); ++TargetDataIdx)
				{
					if (UAbilitySystemBlueprintLibrary::TargetDataHasHitResult(TargetData, TargetDataIdx))
					{
						return true;
					}
				}
			}
		}
		return false;
	};
}

FGameplayEffectContextHandle UGASCourseGameplayAbility::MakeEffectContext(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo) const
{
	return Super::MakeEffectContext(Handle, ActorInfo);
}

void UGASCourseGameplayAbility::ApplyAbilityTagsToGameplayEffectSpec(FGameplayEffectSpec& Spec,
	FGameplayAbilitySpec* AbilitySpec) const
{
	Super::ApplyAbilityTagsToGameplayEffectSpec(Spec, AbilitySpec);
}

bool UGASCourseGameplayAbility::DoesAbilitySatisfyTagRequirements(const UAbilitySystemComponent& AbilitySystemComponent,
	const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags,
	FGameplayTagContainer* OptionalRelevantTags) const
{
	// Specialized version to handle death exclusion and AbilityTags expansion via ASC

	bool bBlocked = false;
	bool bMissing = false;

	const UAbilitySystemGlobals& AbilitySystemGlobals = UAbilitySystemGlobals::Get();
	const FGameplayTag& BlockedTag = AbilitySystemGlobals.ActivateFailTagsBlockedTag;
	const FGameplayTag& MissingTag = AbilitySystemGlobals.ActivateFailTagsMissingTag;

	// Check if any of this ability's tags are currently blocked
	if (AbilitySystemComponent.AreAbilityTagsBlocked(AbilityTags))
	{
		bBlocked = true;
	}

	const UGASCourseAbilitySystemComponent* GASCourseASC = Cast<UGASCourseAbilitySystemComponent>(&AbilitySystemComponent);
	static FGameplayTagContainer AllRequiredTags;
	static FGameplayTagContainer AllBlockedTags;

	AllRequiredTags = ActivationRequiredTags;
	AllBlockedTags = ActivationBlockedTags;

	// Expand our ability tags to add additional required/blocked tags
	if (GASCourseASC)
	{
		GASCourseASC->GetAdditionalActivationTagRequirements(AbilityTags, AllRequiredTags, AllBlockedTags);
	}

	// Check to see the required/blocked tags for this ability
	if (AllBlockedTags.Num() || AllRequiredTags.Num())
	{
		static FGameplayTagContainer AbilitySystemComponentTags;
		
		AbilitySystemComponentTags.Reset();
		AbilitySystemComponent.GetOwnedGameplayTags(AbilitySystemComponentTags);

		if (AbilitySystemComponentTags.HasAny(AllBlockedTags))
		{
			if (OptionalRelevantTags && AbilitySystemComponentTags.HasTag(Status_Death))
			{
				// If player is dead and was rejected due to blocking tags, give that feedback
			}

			bBlocked = true;
		}

		if (!AbilitySystemComponentTags.HasAll(AllRequiredTags))
		{
			bMissing = true;
		}
	}

	if (SourceTags != nullptr)
	{
		if (SourceBlockedTags.Num() || SourceRequiredTags.Num())
		{
			if (SourceTags->HasAny(SourceBlockedTags))
			{
				bBlocked = true;
			}

			if (!SourceTags->HasAll(SourceRequiredTags))
			{
				bMissing = true;
			}
		}
	}

	if (TargetTags != nullptr)
	{
		if (TargetBlockedTags.Num() || TargetRequiredTags.Num())
		{
			if (TargetTags->HasAny(TargetBlockedTags))
			{
				bBlocked = true;
			}

			if (!TargetTags->HasAll(TargetRequiredTags))
			{
				bMissing = true;
			}
		}
	}

	if (bBlocked)
	{
		if (OptionalRelevantTags && BlockedTag.IsValid())
		{
			OptionalRelevantTags->AddTag(BlockedTag);
		}
		return false;
	}
	if (bMissing)
	{
		if (OptionalRelevantTags && MissingTag.IsValid())
		{
			OptionalRelevantTags->AddTag(MissingTag);
		}
		return false;
	}

	return true;
}

bool UGASCourseGameplayAbility::CommitAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	OUT FGameplayTagContainer* OptionalRelevantTags)
{
	OnAbilityCommitDelegate.Broadcast(Super::CommitAbility(Handle, ActorInfo, ActivationInfo, OptionalRelevantTags));
	return Super::CommitAbility(Handle, ActorInfo, ActivationInfo, OptionalRelevantTags);
}

void UGASCourseGameplayAbility::CommitExecute(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if(AbilityType == EGASCourseAbilityType::Duration && bAutoCommitCooldownOnDurationEnd)
	{
		//Only Apply Cost, don't apply cooldown.
		ApplyCost(Handle,ActorInfo, ActivationInfo);
	}
	else
	{
		Super::CommitExecute(Handle, ActorInfo, ActivationInfo);
	}
}

void UGASCourseGameplayAbility::ApplyCooldown(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{

	if(!GetActorInfo().IsNetAuthority() || !HasAuthorityOrPredictionKey(CurrentActorInfo, &CurrentActivationInfo))
	{
		Super::ApplyCooldown(Handle, ActorInfo, ActivationInfo);
	}
	
	if (const UGameplayEffect* CooldownGE = GetCooldownGameplayEffect())
	{
		const FActiveGameplayEffectHandle CooldownActiveGEHandle = ApplyGameplayEffectToOwner(Handle, ActorInfo, ActivationInfo, CooldownGE, GetAbilityLevel(Handle, ActorInfo));
		if(CooldownActiveGEHandle.WasSuccessfullyApplied())
		{
			if(UGASCourseAbilitySystemComponent* ASC = GetGASCourseAbilitySystemComponentFromActorInfo())
			{
				ASC->WaitForAbilityCooldownEnd(const_cast<UGASCourseGameplayAbility*>(this), CooldownActiveGEHandle);
			}
		}
	}
}

void UGASCourseGameplayAbility::GetAbilityCooldownTags(FGameplayTagContainer& CooldownTags) const
{
	CooldownTags.Reset();
	if(const UGameplayEffect* CooldownGE = GetCooldownGameplayEffect())
	{
		CooldownTags.AppendTags(CooldownGE->GetGrantedTags());
		
	}
}

void UGASCourseGameplayAbility::GetAbilityDurationTags(FGameplayTagContainer& DurationTags) const
{
	DurationTags.Reset();
	if(const UGameplayEffect* DurationGE = GetDurationGameplayEffect())
	{
		DurationTags.AppendTags(DurationGE->GetGrantedTags());
	}
}

void UGASCourseGameplayAbility::OnPawnAvatarSet()
{
}

bool UGASCourseGameplayAbility::ApplyDurationEffect()
{
	if(!GetActorInfo().IsNetAuthority() || !HasAuthorityOrPredictionKey(CurrentActorInfo, &CurrentActivationInfo))
	{
		return false;
	}
	bool bSuccess = false;
	if((DurationEffect))
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
						DurationEffectRemovalTask->OnRemoved.AddDynamic(this, &UGASCourseGameplayAbility::DurationEffectRemoved);
						DurationEffectRemovalTask->Activate();
					} 
				}
				return bSuccess;
			}
			UE_LOG(LogBlueprint, Error, TEXT("%s: SUPPLIED GAMEPLAY EFFECT {%s} HAS INVALID DURATION POLICY {%s}."),*GASCOURSE_CUR_CLASS_FUNC, *DurationEffectObject->GetName(), *UEnum::GetValueAsString(DurationEffectObject->DurationPolicy));
			EndAbility(CurrentSpecHandle, CurrentActorInfo,CurrentActivationInfo, true, true);
			return bSuccess;
		}
	}
	
	UE_LOG(LogBlueprint, Error, TEXT("%s: NO VALID DURATION EFFECT DEFINED IN DEFAULT SETTINGS"),*GASCOURSE_CUR_CLASS_FUNC);
	EndAbility(CurrentSpecHandle, CurrentActorInfo,CurrentActivationInfo, true, true);
	return bSuccess;
}
