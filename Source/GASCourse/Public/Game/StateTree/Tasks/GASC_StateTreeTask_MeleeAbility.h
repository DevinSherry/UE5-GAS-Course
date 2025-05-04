// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "GASCourse/Public/Game/GameplayAbilitySystem/GameplayAbilities/Melee/GASC_GameplayAbility_Melee.h"
#include "GASC_StateTreeTask_MeleeAbility.generated.h"

USTRUCT()
struct FGASC_MeleeAbilityTaskInstanceData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category = Parameter, meta=(ClassFilter="GASC_GameplayAbility_Melee"))
	TSubclassOf<UGASC_GameplayAbility_Melee> MeleeGameplayAbilityClass;
	
	UPROPERTY(EditAnywhere, Category = Parameter)
	FMeleeAbilityData MeleeData;
};

UCLASS()
class UStateTreeTask_GASCMeleeAbility_InstanceData : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION()
	void OnMeleeAbilityEnd(const FAbilityEndedData& EndData);

	UPROPERTY(EditAnywhere, Category = Parameter)
	FGASC_MeleeAbilityTaskInstanceData MeleeData;

	EStateTreeRunStatus OnEnterState(const FStateTreeExecutionContext& Context);
	void OnExitState(const FStateTreeExecutionContext& Context);
	EStateTreeRunStatus OnTick(const FStateTreeExecutionContext& Context, const float DeltaTime) const;

	bool bMeleeAbilityActive = false;
	bool bMeleeAbilityGranted = false;
	
	UPROPERTY()
	UGASC_GameplayAbility_Melee* MeleeAbilityRef = nullptr;

	UPROPERTY()
	FGameplayAbilitySpec MeleeAbilitySpec;

	UPROPERTY()
	FGameplayAbilitySpecHandle MeleeAbilitySpecHandle;
	
};

/**
 * A state tree task designed to listen for input actions and send events to the StateTree system
 * when the configured input action is triggered. This task is utilized within the Gameplay Ability System
 * to process and react to user input during specific StateTree states.
 */
USTRUCT(meta = (DisplayName = "Grant & Activate Melee Ability"))
struct FStateTreeTask_GASCMeleeAbility : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	using UInstanceDataType = UStateTreeTask_GASCMeleeAbility_InstanceData;

	FStateTreeTask_GASCMeleeAbility();

	virtual const UStruct* GetInstanceDataType() const override { return UInstanceDataType::StaticClass(); }
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
	virtual void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;
	virtual EDataValidationResult Compile(FStateTreeDataView InstanceDataView, TArray<FText>& ValidationMessages) override;

	virtual void TriggerTransitions(FStateTreeExecutionContext& Context) const override;

#if WITH_EDITOR
	virtual FText GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView, const IStateTreeBindingLookup& BindingLookup, EStateTreeNodeFormatting Formatting = EStateTreeNodeFormatting::Text) const override;
	virtual FName GetIconName() const override
	{
		return FName("StateTreeEditorStyle|Node.Find");
	}
	virtual FColor GetIconColor() const override
	{
		return UE::StateTree::Colors::Blue;
	}
#endif
};