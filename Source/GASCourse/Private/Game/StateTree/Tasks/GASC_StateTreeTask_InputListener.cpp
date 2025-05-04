// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/StateTree/Tasks/GASC_StateTreeTask_InputListener.h"
#include "StateTreeExecutionContext.h"
#include "EnhancedInputComponent.h"
#include "Components/StateTreeComponent.h"
#include "Game/Character/Components/InputBuffer/GASC_InputBufferComponent.h"
#include "Game/Character/Player/GASCoursePlayerCharacter.h"


FStateTreeTask_GASCInputListener::FStateTreeTask_GASCInputListener()
{
}

EStateTreeRunStatus FStateTreeTask_GASCInputListener::EnterState(FStateTreeExecutionContext& Context,
                                                                 const FStateTreeTransitionResult& Transition) const
{
	if (Context.IsValid())
	{
		FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
		if (APlayerController* PC = Cast<APlayerController>(Context.GetOwner()))
		{
			AGASCoursePlayerCharacter* PlayerCharacter = Cast<AGASCoursePlayerCharacter>(PC->GetPawn());
			if (UGASC_InputBufferComponent* InputBuffer = PlayerCharacter->GetInputBufferComponent())
			{
				if (UEnhancedInputComponent* InputComponent = Cast<UEnhancedInputComponent>(PC->InputComponent))
				{
					if (UStateTreeComponent* ST = PC->GetComponentByClass<UStateTreeComponent>())
					{
						for (FEnhancedInputListenerData InputData : InstanceData.InputListeners)
						{
							FEnhancedInputActionEventBinding* TriggeredEventBinding = &InputComponent->BindActionValueLambda(InputData.InputAction, ETriggerEvent::Triggered, [this, ST, InputData](const FInputActionValue& Value)
							{
								ST->SendStateTreeEvent(InputData.TriggeredEventGameplayTag);
							});

							InstanceData.InputActionBindings.Add(TriggeredEventBinding);

							FEnhancedInputActionEventBinding* CompletedEventBinding = &InputComponent->BindActionValueLambda(InputData.InputAction, ETriggerEvent::Completed, [this, ST, InputData](const FInputActionValue& Value)
							{
								ST->SendStateTreeEvent(InputData.CompletedGameplayTag);
							});

							InstanceData.InputActionBindings.Add(CompletedEventBinding);

							FEnhancedInputActionEventBinding* CanceledEventBinding = &InputComponent->BindActionValueLambda(InputData.InputAction, ETriggerEvent::Canceled, [this, ST, InputData](const FInputActionValue& Value)
							{
								ST->SendStateTreeEvent(InputData.CanceledGameplayTag);
							});

							InstanceData.InputActionBindings.Add(CanceledEventBinding);
						}
					}
				}
			}
		}
	}
	return EStateTreeRunStatus::Running;
}

void FStateTreeTask_GASCInputListener::ExitState(FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition) const
{
	if (Context.IsValid())
	{
		FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
		if (APlayerController* PC = Cast<APlayerController>(Context.GetOwner()))
		{
			if (UEnhancedInputComponent* InputComponent = Cast<UEnhancedInputComponent>(PC->InputComponent))
			{
				for (const FEnhancedInputActionEventBinding* Binding : InstanceData.InputActionBindings)
				{
					// Unbind all actions
					InputComponent->RemoveActionBinding(Binding->GetHandle());
				}
			}
			InstanceData.InputActionBindings.Empty();
		}
	}

	FStateTreeTaskCommonBase::ExitState(Context, Transition);
}

EStateTreeRunStatus FStateTreeTask_GASCInputListener::Tick(FStateTreeExecutionContext& Context,
	const float DeltaTime) const
{
	return FStateTreeTaskCommonBase::Tick(Context, DeltaTime);
}

EDataValidationResult FStateTreeTask_GASCInputListener::Compile(FStateTreeDataView InstanceDataView,
	TArray<FText>& ValidationMessages)
{
	if (const FStateTreeEnhancedInputListenerTaskInstanceData* InstanceData =
		InstanceDataView.GetMutablePtr<FStateTreeEnhancedInputListenerTaskInstanceData>())
	{
		if (InstanceData->InputListeners.IsEmpty())
		{
			ValidationMessages.Add(FText::FromString("Input Listeners array is empty! Add at least one entry."));
			return EDataValidationResult::Invalid;
		}
		for (FEnhancedInputListenerData InputData : InstanceData->InputListeners)
		{
			if (InputData.InputAction == nullptr)
			{
				ValidationMessages.Add(FText::FromString("Input Action is null! Add a valid input action."));
				return EDataValidationResult::Invalid;
			}

			if (!InputData.TriggeredEventGameplayTag.IsValid() && !InputData.CompletedGameplayTag.IsValid() && !InputData.CanceledGameplayTag.IsValid())
			{
				ValidationMessages.Add(FText::FromString("One or more Gameplay Tags are invalid!"));
				return EDataValidationResult::Invalid;
			}
		}
	}
	return Super::Compile(InstanceDataView, ValidationMessages);
}

#if WITH_EDITOR
FText FStateTreeTask_GASCInputListener::GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView,
                                                       const IStateTreeBindingLookup& BindingLookup, EStateTreeNodeFormatting Formatting) const
{
	return FStateTreeTaskCommonBase::GetDescription(ID, InstanceDataView, BindingLookup, Formatting);
}
#endif // WITH_EDITOR
