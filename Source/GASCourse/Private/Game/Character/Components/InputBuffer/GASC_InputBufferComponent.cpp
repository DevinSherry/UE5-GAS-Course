// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Character/Components/InputBuffer/GASC_InputBufferComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"
#include "Game/Character/Components/InputBuffer/GASC_InputBuffer_Settings.h"
#include "Game/Character/Player/GASCoursePlayerController.h"
#include "Game/GameplayAbilitySystem/GASCourseAbilitySystemComponent.h"
#include "GASCourse/GASCourseCharacter.h"

DEFINE_LOG_CATEGORY(LOG_GASC_InputBufferComponent);

static FString INPUT_BUFFER_COMPONENT_NAME;

// Sets default values for this component's properties
UGASC_InputBufferComponent::UGASC_InputBufferComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;

	INPUT_BUFFER_COMPONENT_NAME = GetPathNameSafe(this);

	// ...
}

void UGASC_InputBufferComponent::InitializeComponent()
{
	Super::InitializeComponent();
	InputBufferSettings = GetDefault<UGASC_InputBuffer_Settings>();
	if (!InputBufferSettings || InputBufferSettings->BufferedInputActions.IsEmpty())
	{
		return;
	}

	TArray<FSoftObjectPath> AssetPaths;
	for (const TSoftObjectPtr<UInputAction>& SoftPtr : InputBufferSettings->BufferedInputActions)
	{
		if (!SoftPtr.IsValid())
		{
			AssetPaths.Add(SoftPtr.ToSoftObjectPath());
		}
		else
		{
			// Already loaded
			InputActionsToConsume.Add(SoftPtr.Get());
		}
	}

	// Bulk load all objects at once
	FStreamableManager& Streamable = UAssetManager::GetStreamableManager();

	if (AssetPaths.Num() > 0)
	{
		Streamable.RequestAsyncLoad(
			AssetPaths,
			FStreamableDelegate::CreateUObject(this, &UGASC_InputBufferComponent::LoadInputActionsFromSettings)
		);
	}
}

void UGASC_InputBufferComponent::LoadInputActionsFromSettings()
{
	InputBufferSettings = GetDefault<UGASC_InputBuffer_Settings>();
	if (!InputBufferSettings || InputBufferSettings->BufferedInputActions.IsEmpty())
	{
		return;
	}

	for (const TSoftObjectPtr<UInputAction>& SoftPtr : InputBufferSettings->BufferedInputActions)
	{
		if (UInputAction* Action = SoftPtr.Get())
		{
			InputActionsToConsume.Add(Action);
		}
		else
		{
			UE_LOG(LOG_GASC_InputBufferComponent, Warning, 
	   TEXT("Failed to load input action from soft pointer"));

		}
	}

	// You can now use LoadedInputActions
	UE_LOG(LOG_GASC_InputBufferComponent, Log, TEXT("All input actions loaded (%d total)."), InputActionsToConsume.Num());
}

void UGASC_InputBufferComponent::OpenInputBuffer_Implementation()
{
	UE_LOG(LOG_GASC_InputBufferComponent, Log, TEXT("Input Buffer Open: %s"), *INPUT_BUFFER_COMPONENT_NAME);
	
	bInputBufferOpen = true;
	OnInputBufferOpenedEvent.Broadcast();
}

void UGASC_InputBufferComponent::CloseInputBuffer_Implementation()
{
	UE_LOG(LOG_GASC_InputBufferComponent, Log, TEXT("Input Buffer Closed: %s"), *INPUT_BUFFER_COMPONENT_NAME);
	bInputBufferOpen = false;
	ActivateBufferedInputAbility();
	FlushInputBuffer();
	OnInputBufferClosedEvent.Broadcast();
}

bool UGASC_InputBufferComponent::FlushInputBuffer()
{
	UE_LOG(LOG_GASC_InputBufferComponent, Log, TEXT("Input Buffer Flushed: %s"), *INPUT_BUFFER_COMPONENT_NAME);
	BufferedInputActions.Empty();
	OnInputBufferFlushedEvent.Broadcast();
	return true;
}

void UGASC_InputBufferComponent::ActivateBufferedInputAbility()
{
	if(OwningCharacter)
	{
		if(UGASCourseAbilitySystemComponent* ASC = OwningCharacter->GetAbilitySystemComponent())
		{
			if (GetBufferedInputActions().Num() > 0)
			{
				if (UInputAction* InputActionToSimulate = BufferedInputActions[0])
				{
					UE_LOG(LOG_GASC_InputBufferComponent, Log, TEXT("Input Action Simulated: %s, %s"),
					*InputActionToSimulate->GetName(), *INPUT_BUFFER_COMPONENT_NAME);
					SimulateInputAction(InputActionToSimulate);
					OnInputBufferedConsumedEvent.Broadcast(InputActionToSimulate);
				}
			}
		}
	}
}

void UGASC_InputBufferComponent::AddInputActionToBuffer(UInputAction* InAction)
{
	if (InAction && IsInputBufferOpen())
	{
		UE_LOG(LOG_GASC_InputBufferComponent, Log, TEXT("Input Action Added to Buffer: %s, %s"),
		*InAction->GetName(), *INPUT_BUFFER_COMPONENT_NAME);
		BufferedInputActions.AddUnique(InAction);
	}
}

void UGASC_InputBufferComponent::ListenToInputActions()
{
	InputBufferSettings = GetDefault<UGASC_InputBuffer_Settings>();
	if (!InputBufferSettings)
	{
		return;
	}

	OwningCharacter = Cast<AGASCourseCharacter>(GetOwner());
	if(!OwningCharacter)
	{
		return;
	}

	OwningPlayerController = Cast<AGASCoursePlayerController>(OwningCharacter->GetController());
	if (!OwningPlayerController)
	{
		return;
	}

	EnhancedInputComponent = Cast<UEnhancedInputComponent>(OwningPlayerController->InputComponent);
	if (!EnhancedInputComponent)
	{
		return;
	}

	for (UInputAction* InputAction : InputActionsToConsume)
	{
		if (InputAction)
		{
			const FEnhancedInputActionEventBinding* TriggeredEventBinding = &EnhancedInputComponent->BindActionValueLambda(InputAction, ETriggerEvent::Triggered, [this, InputAction](const FInputActionValue& Value)
			{
				AddInputActionToBuffer(InputAction);
			});

			Bindings.Add(TriggeredEventBinding);
		}
	}
}

void UGASC_InputBufferComponent::SimulateInputAction(const UInputAction* InputAction) const
{
	if (!OwningPlayerController)
	{
		return;
	}

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
		OwningPlayerController->GetLocalPlayer()))
	{
		FInputActionValue Value(EInputActionValueType::Boolean, FVector(1.0f, 1.0f, 1.0f));
		const TArray<UInputModifier*>& Modifiers = {};
		const TArray<UInputTrigger*>& Triggers = {};
		Subsystem->InjectInputForAction(InputAction, Value, Modifiers, Triggers);
		UE_LOG(LOG_GASC_InputBufferComponent, Log, TEXT("Input Action Simulated: %s, %s"),
		*InputAction->GetName(), *INPUT_BUFFER_COMPONENT_NAME);
	}
}

// Called when the game starts
void UGASC_InputBufferComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	ListenToInputActions();
}

void UGASC_InputBufferComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if (EnhancedInputComponent)
	{
		for (const FEnhancedInputActionEventBinding* Binding: Bindings)
		{
			EnhancedInputComponent->RemoveActionEventBinding(Binding->GetHandle());
		}
	}
}
