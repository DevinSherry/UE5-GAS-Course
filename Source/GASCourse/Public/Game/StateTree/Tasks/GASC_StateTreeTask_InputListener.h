// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "GASC_StateTreeTask_InputListener.generated.h"

USTRUCT()
struct FEnhancedInputListenerData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Parameter)
	UInputAction* InputAction = nullptr;

	UPROPERTY(EditAnywhere, Category = Parameter, meta = (Categories = "Input.StateTree"))
	FGameplayTag TriggeredEventGameplayTag;

	UPROPERTY(EditAnywhere, Category = Parameter, meta = (Categories = "Input.StateTree"))
	FGameplayTag CompletedGameplayTag;

	UPROPERTY(EditAnywhere, Category = Parameter, meta = (Categories = "Input.StateTree"))
	FGameplayTag CanceledGameplayTag;
};

USTRUCT()
struct FStateTreeEnhancedInputListenerTaskInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Parameter)
	TArray<FEnhancedInputListenerData> InputListeners;

	TArray<FEnhancedInputActionEventBinding*> InputActionBindings;

	
};

/**
 * A state tree task designed to listen for input actions and send events to the StateTree system
 * when the configured input action is triggered. This task is utilized within the Gameplay Ability System
 * to process and react to user input during specific StateTree states.
 */
USTRUCT(meta = (DisplayName = "Listen to Enhanced Input Action Events"))
struct FStateTreeTask_GASCInputListener : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FStateTreeEnhancedInputListenerTaskInstanceData;

	FStateTreeTask_GASCInputListener();

	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;

	virtual void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;

	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;

	//virtual EDataValidationResult Compile(FStateTreeDataView InstanceDataView, TArray<FText>& ValidationMessages) override;

#if WITH_EDITOR
	virtual FText GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView, const IStateTreeBindingLookup& BindingLookup, EStateTreeNodeFormatting Formatting = EStateTreeNodeFormatting::Text) const override;
	virtual FName GetIconName() const override
	{
		return FName("StateTreeEditorStyle|Node.Event");
	}
	virtual FColor GetIconColor() const override
	{
		return UE::StateTree::Colors::Blue;
	}
#endif

};
