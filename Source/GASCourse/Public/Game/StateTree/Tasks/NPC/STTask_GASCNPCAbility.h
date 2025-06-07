// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "Game/GameplayAbilitySystem/GameplayAbilities/NPC/GASC_NPC_GameplayAbilityBase.h"
#include "STTask_GASCNPCAbility.generated.h"

USTRUCT()
struct FGASC_NPCAbilityTaskInstanceData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category = Parameter, meta=(ClassFilter="GASC_NPC_GameplayAbilityBase"))
	TSubclassOf<UGASC_NPC_GameplayAbilityBase> NPCGameplayAbilityClass;
	
	UPROPERTY(EditAnywhere, Category = Parameter)
	FNPCAbilityData NPCAbilityData;
};

UCLASS()
class UStateTreeTask_GASCNPCAbility_InstanceData : public UObject
{
	GENERATED_BODY()

	~UStateTreeTask_GASCNPCAbility_InstanceData();

public:

	UFUNCTION()
	void OnNPCAbilityEnd(const FAbilityEndedData& EndData);\

	UFUNCTION()
	void OnNPCAbilityFailed(const UGameplayAbility* InAbility, const FGameplayTagContainer& FailTags);

	UFUNCTION()
	void OnNPCAbilityActivated(UGameplayAbility* InAbility);

	UPROPERTY(EditAnywhere, Category = Parameter)
	FGASC_NPCAbilityTaskInstanceData NPCAbilityData;

	EStateTreeRunStatus OnEnterState(const FStateTreeExecutionContext& Context);
	void OnExitState(const FStateTreeExecutionContext& Context);
	EStateTreeRunStatus OnTick(const FStateTreeExecutionContext& Context, const float DeltaTime) const;

	bool bNPCAbilityActive = false;
	bool bNPCAbilityGranted = false;
	
	UPROPERTY()
	UGASC_NPC_GameplayAbilityBase* NPCAbilityRef = nullptr;

	UPROPERTY()
	FGameplayAbilitySpec NPCAbilitySpec;

	UPROPERTY()
	FGameplayAbilitySpecHandle NPCAbilitySpecHandle;
	
};

/**
 * A state tree task designed to listen for input actions and send events to the StateTree system
 * when the configured input action is triggered. This task is utilized within the Gameplay Ability System
 * to process and react to user input during specific StateTree states.
 */
USTRUCT(meta = (DisplayName = "Grant & Activate NPC Ability"))
struct FStateTreeTask_GASCNPCAbility : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	using UInstanceDataType = UStateTreeTask_GASCNPCAbility_InstanceData;

	FStateTreeTask_GASCNPCAbility();

	virtual const UStruct* GetInstanceDataType() const override { return UInstanceDataType::StaticClass(); }
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
	virtual void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;

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