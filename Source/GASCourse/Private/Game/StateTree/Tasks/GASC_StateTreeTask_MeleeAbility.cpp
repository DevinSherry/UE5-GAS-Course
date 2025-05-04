// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/StateTree/Tasks/GASC_StateTreeTask_MeleeAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "StateTreeExecutionContext.h"

void UStateTreeTask_GASCMeleeAbility_InstanceData::OnMeleeAbilityEnd(const FAbilityEndedData& EndData)
{
	if (EndData.AbilitySpecHandle == MeleeAbilitySpecHandle)
	{
		bMeleeAbilityActive = false;
	}
}

EStateTreeRunStatus UStateTreeTask_GASCMeleeAbility_InstanceData::OnEnterState(
	const FStateTreeExecutionContext& Context)
{
	if (MeleeData.MeleeGameplayAbilityClass == nullptr)
	{
		return EStateTreeRunStatus::Failed;
	}
	MeleeAbilityRef = NewObject<UGASC_GameplayAbility_Melee>(Context.GetWorld(), MeleeData.MeleeGameplayAbilityClass);
	if (MeleeAbilityRef)
	{
		if (APlayerController* PC = Cast<APlayerController>(Context.GetOwner()))
		{
			if (AGASCourseCharacter* OwningCharacter = Cast<AGASCourseCharacter>(PC->GetPawn()))
			{
				if (UAbilitySystemComponent* ASC = OwningCharacter->GetAbilitySystemComponent())
				{
					MeleeAbilityRef->MeleeAbilityData = MeleeData.MeleeData;
					ASC->OnAbilityEnded.AddUObject(this, &UStateTreeTask_GASCMeleeAbility_InstanceData::OnMeleeAbilityEnd);

					MeleeAbilitySpec = FGameplayAbilitySpec(
					MeleeData.MeleeGameplayAbilityClass,
					1,
					INDEX_NONE,
					MeleeAbilityRef);
					
					MeleeAbilitySpecHandle = ASC->GiveAbilityAndActivateOnce(MeleeAbilitySpec);
					
					bMeleeAbilityGranted = true;
					bMeleeAbilityActive = true;
					return EStateTreeRunStatus::Running;
				}
			}
		}
	}
	return EStateTreeRunStatus::Failed;
}

void UStateTreeTask_GASCMeleeAbility_InstanceData::OnExitState(const FStateTreeExecutionContext& Context)
{
	if (APlayerController* PC = Cast<APlayerController>(Context.GetOwner()))
	{
		if (AGASCourseCharacter* OwningCharacter = Cast<AGASCourseCharacter>(PC->GetPawn()))
		{
			if (UAbilitySystemComponent* ASC = OwningCharacter->GetAbilitySystemComponent())
			{
				ASC->ClearAbility(MeleeAbilitySpecHandle);
			}
		}
	}
}

EStateTreeRunStatus UStateTreeTask_GASCMeleeAbility_InstanceData::OnTick(const FStateTreeExecutionContext& Context,
	const float DeltaTime) const
{
	if (bMeleeAbilityGranted && !bMeleeAbilityActive)
	{
		UE_LOG(LogTemp, Warning, TEXT("Melee Ability not granted or active"));
		return EStateTreeRunStatus::Succeeded;
	}
	
	return EStateTreeRunStatus::Running;
}

FStateTreeTask_GASCMeleeAbility::FStateTreeTask_GASCMeleeAbility()
{
	bShouldAffectTransitions = true;
}

EStateTreeRunStatus FStateTreeTask_GASCMeleeAbility::EnterState(FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition) const
{
	UInstanceDataType* InstanceData = Context.GetInstanceDataPtr<UInstanceDataType>(*this);
	check(InstanceData);

	return InstanceData->OnEnterState(Context);
}

void FStateTreeTask_GASCMeleeAbility::ExitState(FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition) const
{
	Super::ExitState(Context, Transition);
	UInstanceDataType* InstanceData = Context.GetInstanceDataPtr<UInstanceDataType>(*this);
	check(InstanceData);
	
	InstanceData->OnExitState(Context);

	if (APlayerController* PC = Cast<APlayerController>(Context.GetOwner()))
	{
		if (AGASCourseCharacter* OwningCharacter = Cast<AGASCourseCharacter>(PC->GetPawn()))
		{
			if (UAbilitySystemComponent* ASC = OwningCharacter->GetAbilitySystemComponent())
			{
				ASC->ClearAbility(InstanceData->MeleeAbilitySpecHandle);
			}
		}
	}
}

void FStateTreeTask_GASCMeleeAbility::TriggerTransitions(FStateTreeExecutionContext& Context) const
{
	FStateTreeTaskCommonBase::TriggerTransitions(Context);

	UInstanceDataType* InstanceData = Context.GetInstanceDataPtr<UInstanceDataType>(*this);
	check(InstanceData);
	/*
	
	if (APlayerController* PC = Cast<APlayerController>(Context.GetOwner()))
	{
		if (AGASCourseCharacter* OwningCharacter = Cast<AGASCourseCharacter>(PC->GetPawn()))
		{
			if (UAbilitySystemComponent* ASC = OwningCharacter->GetAbilitySystemComponent())
			{
				ASC->ClearAbility(InstanceData->MeleeAbilitySpecHandle);
			}
		}
	}
	*/
}

EStateTreeRunStatus FStateTreeTask_GASCMeleeAbility::Tick(FStateTreeExecutionContext& Context,
                                                          const float DeltaTime) const
{
	UInstanceDataType* InstanceData = Context.GetInstanceDataPtr<UInstanceDataType>(*this);
	check(InstanceData);
	
	return InstanceData->OnTick(Context, DeltaTime);
}

EDataValidationResult FStateTreeTask_GASCMeleeAbility::Compile(FStateTreeDataView InstanceDataView,
	TArray<FText>& ValidationMessages)
{
	return FStateTreeTaskCommonBase::Compile(InstanceDataView, ValidationMessages);
}

#if WITH_EDITOR
FText FStateTreeTask_GASCMeleeAbility::GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView,
                                                      const IStateTreeBindingLookup& BindingLookup, EStateTreeNodeFormatting Formatting) const
{
	return FStateTreeTaskCommonBase::GetDescription(ID, InstanceDataView, BindingLookup, Formatting);
}
#endif // WITH_EDITOR