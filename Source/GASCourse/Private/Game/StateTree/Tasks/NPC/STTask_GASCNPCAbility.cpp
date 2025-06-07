// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/StateTree/Tasks/NPC/STTask_GASCNPCAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AIController.h"
#include "StateTreeExecutionContext.h"
#include "Game/Character/NPC/GASCourseNPC_Base.h"


UStateTreeTask_GASCNPCAbility_InstanceData::~UStateTreeTask_GASCNPCAbility_InstanceData()
{
	if (NPCAbilityRef)
	{
		NPCAbilityRef = nullptr;
	}
}

void UStateTreeTask_GASCNPCAbility_InstanceData::OnNPCAbilityEnd(const FAbilityEndedData& EndData)
{
	if (EndData.AbilitySpecHandle == NPCAbilitySpecHandle)
	{
		bNPCAbilityActive = false;
	}
}

void UStateTreeTask_GASCNPCAbility_InstanceData::OnNPCAbilityFailed(const UGameplayAbility* InAbility,
	const FGameplayTagContainer& FailTags)
{
	if (InAbility->GetCurrentAbilitySpec()->Handle == NPCAbilitySpec.Handle)
	{
		bNPCAbilityActive = false;
	}
}

void UStateTreeTask_GASCNPCAbility_InstanceData::OnNPCAbilityActivated(UGameplayAbility* InAbility)
{
	if (InAbility->GetCurrentAbilitySpec()->Handle == NPCAbilitySpec.Handle)
	{
		InAbility->GetAbilitySystemComponentFromActorInfo_Ensured()->OnAbilityEnded.AddUObject(this, &UStateTreeTask_GASCNPCAbility_InstanceData::OnNPCAbilityEnd);
		bNPCAbilityActive = true;
	}
}

EStateTreeRunStatus UStateTreeTask_GASCNPCAbility_InstanceData::OnEnterState(
	const FStateTreeExecutionContext& Context)
{
	if (NPCAbilityData.NPCGameplayAbilityClass == nullptr)
	{
		return EStateTreeRunStatus::Failed;
	}
	NPCAbilityRef = NewObject<UGASC_NPC_GameplayAbilityBase>(Context.GetWorld(), NPCAbilityData.NPCGameplayAbilityClass);
	if (NPCAbilityRef)
	{
		if (AAIController* AI = Cast<AAIController>(Context.GetOwner()))
		{
			if (AGASCourseNPC_Base* NPCCharacter = Cast<AGASCourseNPC_Base>(AI->GetPawn()))
			{
				if (UAbilitySystemComponent* ASC = NPCCharacter->GetAbilitySystemComponent())
				{
					NPCAbilityRef->NPCAbilityData = NPCAbilityData.NPCAbilityData;
					
					//ASC->OnAbilityEnded.AddUObject(this, &UStateTreeTask_GASCNPCAbility_InstanceData::OnNPCAbilityEnd);
					ASC->AbilityFailedCallbacks.AddUObject(this, &UStateTreeTask_GASCNPCAbility_InstanceData::OnNPCAbilityFailed);
					ASC->AbilityActivatedCallbacks.AddUObject(this, &UStateTreeTask_GASCNPCAbility_InstanceData::OnNPCAbilityActivated);
					NPCAbilitySpec = FGameplayAbilitySpec(
					NPCAbilityData.NPCGameplayAbilityClass,
					1,
					INDEX_NONE,
					NPCAbilityRef);
					
					NPCAbilitySpecHandle = ASC->GiveAbilityAndActivateOnce(NPCAbilitySpec);
					//ASC->OnAbilityEnded.AddUObject(this, &UStateTreeTask_GASCNPCAbility_InstanceData::OnNPCAbilityEnd);
					
					bNPCAbilityGranted = true;
					return EStateTreeRunStatus::Running;
				}
			}
		}
	}
	return EStateTreeRunStatus::Failed;
}

void UStateTreeTask_GASCNPCAbility_InstanceData::OnExitState(const FStateTreeExecutionContext& Context)
{
	if (bNPCAbilityGranted)  // Only cleanup if we actually granted the ability
	{
		if (AAIController* AI = Cast<AAIController>(Context.GetOwner()))
		{
			if (AGASCourseNPC_Base* NPCCharacter = Cast<AGASCourseNPC_Base>(AI->GetPawn()))
			{
				if (UAbilitySystemComponent* ASC = NPCCharacter->GetAbilitySystemComponent())
				{
					if (NPCAbilitySpecHandle.IsValid())
					{
						ASC->ClearAbility(NPCAbilitySpecHandle);
					}
				}
			}
		}
        
		if (NPCAbilityRef)
		{
			//NPCAbilityRef->MarkAsGarbage();
			NPCAbilityRef = nullptr;
		}
        
		bNPCAbilityGranted = false;
	}
}

EStateTreeRunStatus UStateTreeTask_GASCNPCAbility_InstanceData::OnTick(const FStateTreeExecutionContext& Context,
	const float DeltaTime) const
{
	if (bNPCAbilityGranted && !bNPCAbilityActive)
	{
		return EStateTreeRunStatus::Failed;
	}
	
	return EStateTreeRunStatus::Running;
}

FStateTreeTask_GASCNPCAbility::FStateTreeTask_GASCNPCAbility()
{
	bShouldAffectTransitions = true;
}

EStateTreeRunStatus FStateTreeTask_GASCNPCAbility::EnterState(FStateTreeExecutionContext& Context,
                                                                const FStateTreeTransitionResult& Transition) const
{
	UInstanceDataType* InstanceData = Context.GetInstanceDataPtr<UInstanceDataType>(*this);
	check(InstanceData);

	return InstanceData->OnEnterState(Context);
}

void FStateTreeTask_GASCNPCAbility::ExitState(FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition) const
{
	Super::ExitState(Context, Transition);
	UInstanceDataType* InstanceData = Context.GetInstanceDataPtr<UInstanceDataType>(*this);
	check(InstanceData);
	
	InstanceData->OnExitState(Context);
}

void FStateTreeTask_GASCNPCAbility::TriggerTransitions(FStateTreeExecutionContext& Context) const
{
	FStateTreeTaskCommonBase::TriggerTransitions(Context);

	UInstanceDataType* InstanceData = Context.GetInstanceDataPtr<UInstanceDataType>(*this);
	check(InstanceData);
}

EStateTreeRunStatus FStateTreeTask_GASCNPCAbility::Tick(FStateTreeExecutionContext& Context,
                                                          const float DeltaTime) const
{
	UInstanceDataType* InstanceData = Context.GetInstanceDataPtr<UInstanceDataType>(*this);
	check(InstanceData);
	
	return InstanceData->OnTick(Context, DeltaTime);
}

#if WITH_EDITOR
FText FStateTreeTask_GASCNPCAbility::GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView,
                                                      const IStateTreeBindingLookup& BindingLookup, EStateTreeNodeFormatting Formatting) const
{
	return FStateTreeTaskCommonBase::GetDescription(ID, InstanceDataView, BindingLookup, Formatting);
}
#endif // WITH_EDITOR
