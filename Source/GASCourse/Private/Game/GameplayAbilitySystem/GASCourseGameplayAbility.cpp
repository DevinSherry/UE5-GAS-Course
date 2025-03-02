// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GameplayAbilitySystem/GASCourseGameplayAbility.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Game/GameplayAbilitySystem/GASCourseNativeGameplayTags.h"
#include "GameplayEffect.h"
#include "GameplayEffectTypes.h"

UGASCourseGameplayAbility::UGASCourseGameplayAbility(const FObjectInitializer& ObjectInitializer)
{
	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateNo;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	NetSecurityPolicy = EGameplayAbilityNetSecurityPolicy::ClientOrServer;

	ActivationPolicy = EGASCourseAbilityActivationPolicy::OnInputTriggered;
	AbilityType = EGASCourseAbilityType::Instant;
	AbilitySlotType = EGASCourseAbilitySlotType::PrimarySlot;

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
	const bool bIsPredicting = (Spec.GetPrimaryInstance()->GetCurrentActivationInfo().ActivationMode == EGameplayAbilityActivationMode::Predicting);
	
	
	// Try to activate if activation policy is on spawn.
	if (ActorInfo && !Spec.IsActive() && !bIsPredicting && (ActivationPolicy == EGASCourseAbilityActivationPolicy::OnSpawn))
	{
		FGameplayTagContainer SourceTags;
		GetGrantedByEffectContext().GetOriginalInstigatorAbilitySystemComponent()->GetOwnedGameplayTags(SourceTags);
		FGameplayTagContainer TargetTags;
		FGameplayTagContainer RelevantActivationTags;
		GetAbilitySystemComponentFromActorInfo()->GetOwnedGameplayTags(TargetTags);
		
		if(DoesAbilitySatisfyTagRequirements(*GetAbilitySystemComponentFromActorInfo(), &SourceTags, &TargetTags, &RelevantActivationTags))
		{
			UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
			const AActor* AvatarActor = ActorInfo->AvatarActor.Get();

			// If avatar actor is torn off or about to die, don't try to activate until we get the new one.
			if (ASC && AvatarActor && !AvatarActor->GetTearOff() && (AvatarActor->GetLifeSpan() <= 0.0f))
			{
				const bool bIsLocalExecution = (NetExecutionPolicy == EGameplayAbilityNetExecutionPolicy::LocalPredicted) || (NetExecutionPolicy == EGameplayAbilityNetExecutionPolicy::LocalOnly);
				const bool bIsServerExecution = (NetExecutionPolicy == EGameplayAbilityNetExecutionPolicy::ServerOnly) || (NetExecutionPolicy == EGameplayAbilityNetExecutionPolicy::ServerInitiated);

				const bool bClientShouldActivate = ActorInfo->IsLocallyControlled() && bIsLocalExecution;
				const bool bServerShouldActivate = ActorInfo->IsNetAuthority() && bIsServerExecution;

				if (bClientShouldActivate || bServerShouldActivate)
				{
					ASC->TryActivateAbility(Spec.Handle);
				}
			}
		}
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
}

void UGASCourseGameplayAbility::OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilitySpec& Spec)
{
	EndAbility(Spec.Handle, ActorInfo, Spec.GetPrimaryInstance()->GetCurrentActivationInfo(), true, false);
	K2_OnAbilityRemoved();
	Super::OnRemoveAbility(ActorInfo, Spec);
}

void UGASCourseGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
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
	UGameplayEffect* CostGE = GetCostGameplayEffect();
	if (CostGE)
	{
		UAbilitySystemComponent* AbilitySystemComponent = ActorInfo ? ActorInfo->AbilitySystemComponent.Get() : nullptr;
		if (ensure(AbilitySystemComponent))
		{
			if (!AbilitySystemComponent->CanApplyAttributeModifiers(CostGE, GetAbilityLevel(Handle, ActorInfo), MakeEffectContext(Handle, ActorInfo)))
			{
				const FGameplayTag& CostTag = UAbilitySystemGlobals::Get().ActivateFailCostTag;

				if (OptionalRelevantTags && CostTag.IsValid())
				{
					OptionalRelevantTags->AddTag(CostTag);
					
					FGameplayTag EventTag = Event_AbilityActivation_CantAffordCost;
					FGameplayEventData Payload;
					Payload.Instigator = GetAvatarActorFromActorInfo();
					Payload.OptionalObject = this;
					Payload.EventTag = AbilityActivationFail_CantAffordCost;
					Payload.Target = GetAvatarActorFromActorInfo();
					AbilitySystemComponent->HandleGameplayEvent(EventTag, &Payload);

					//Haptic feedback for failure.
					InvokeAbilityFailHapticFeedback();
				}
				return false;
			}
		}
	}
	return true;
}

void UGASCourseGameplayAbility::ApplyCost(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	UAbilitySystemGlobals& AbilitySystemGlobals = UAbilitySystemGlobals::Get();
	if (!AbilitySystemGlobals.ShouldIgnoreCosts())
	{
		Super::ApplyCost(Handle, ActorInfo, ActivationInfo);
	}
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
	if (AbilitySystemComponent.AreAbilityTagsBlocked(GetAssetTags()))
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
		GASCourseASC->GetAdditionalActivationTagRequirements(GetAssetTags(), AllRequiredTags, AllBlockedTags);
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
	bool bCanCommit = Super::CommitAbility(Handle, ActorInfo, ActivationInfo, OptionalRelevantTags);
	OnAbilityCommitDelegate.Broadcast(bCanCommit);
	return bCanCommit;
}

void UGASCourseGameplayAbility::ApplyCooldown(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	UAbilitySystemGlobals& AbilitySystemGlobals = UAbilitySystemGlobals::Get();
	if (!AbilitySystemGlobals.ShouldIgnoreCooldowns())
	{
		check(ActorInfo);
		Super::ApplyCooldown(Handle, ActorInfo, ActivationInfo);

		if(ActorInfo->IsNetAuthority() || HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
		{
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
	}
}

bool UGASCourseGameplayAbility::CheckCooldown(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!ensure(ActorInfo))
	{
		return true;
	}

	const FGameplayTagContainer* CooldownTags = GetCooldownTags();
	if (CooldownTags && !CooldownTags->IsEmpty())
	{
		if (UAbilitySystemComponent* AbilitySystemComponent = ActorInfo->AbilitySystemComponent.Get())
		{
			if (AbilitySystemComponent->HasAnyMatchingGameplayTags(*CooldownTags))
			{
				if (OptionalRelevantTags)
				{
					const FGameplayTag& FailCooldownTag = UAbilitySystemGlobals::Get().ActivateFailCooldownTag;
					if (FailCooldownTag.IsValid())
					{
						FGameplayTag EventTag;
						EventTag = Event_AbilityActivation_Fail;
						
						FGameplayEventData Payload;
						Payload.Instigator = GetAvatarActorFromActorInfo();
						Payload.OptionalObject = this;
						Payload.EventTag = AbilityActivationFail_OnCooldown;
						Payload.Target = GetAvatarActorFromActorInfo();
						
						OptionalRelevantTags->AddTag(FailCooldownTag);
						AbilitySystemComponent->HandleGameplayEvent(EventTag, &Payload);

						//Haptic feedback for failure.
						InvokeAbilityFailHapticFeedback();
					}

					// Let the caller know which tags were blocking
					OptionalRelevantTags->AppendMatchingTags(AbilitySystemComponent->GetOwnedGameplayTags(), *CooldownTags);
				}

				return false;
			}
		}
	}
	return true;
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
}

float UGASCourseGameplayAbility::GetGrantedbyEffectDuration() const
{
	check(CurrentActorInfo);
	if (CurrentActorInfo)
	{
		const UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo_Ensured();
		const FActiveGameplayEffectHandle ActiveHandle = AbilitySystemComponent->FindActiveGameplayEffectHandle(GetCurrentAbilitySpecHandle());
		if (ActiveHandle.IsValid())
		{
			return AbilitySystemComponent->GetGameplayEffectDuration(ActiveHandle);
		}
	}
	return 0.0f;
}

void UGASCourseGameplayAbility::OnPawnAvatarSet()
{
}

void UGASCourseGameplayAbility::GetStackedAbilityDurationTags(FGameplayTagContainer& DurationTags) const
{
	if (!bHasAbilityStacks)
	{
		return;
	}
	if (!StackDurationEffect.Get())
	{
		return;
	}
	DurationTags.Reset();
	
	if (const UGameplayEffect* StackDurationGE = StackDurationEffect->GetDefaultObject<UGameplayEffect>())
	{
		DurationTags.AppendTags(StackDurationGE->GetGrantedTags());
	}
}

void UGASCourseGameplayAbility::InvokeAbilityFailHapticFeedback() const
{
	if (const UGASC_AbilitySystemSettings* AbilitySystemSettings = GetDefault<UGASC_AbilitySystemSettings>())
	{
		if (AGASCoursePlayerController* PC = GetGASCoursePlayerControllerFromActorInfo())
		{
			if (PC->IsLocalPlayerController())
			{
				UForceFeedbackEffect* ForceFeedbackEffect = AbilitySystemSettings->HapticFeedback_AbilityActivationFail.Get();
				PC->ClientPlayForceFeedback(ForceFeedbackEffect);
			}
		}
	}
}
