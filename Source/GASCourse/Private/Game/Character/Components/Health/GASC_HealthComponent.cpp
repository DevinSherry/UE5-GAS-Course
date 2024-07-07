// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Character/Components/Health/GASC_HealthComponent.h"
#include "MVVMGameSubsystem.h"
#include "MVVMSubsystem.h"
#include "Abilities/Tasks/AbilityTask_WaitAttributeChange.h"
#include "Game/GameplayAbilitySystem/AttributeSets/GASCourseHealthAttributeSet.h"
#include "Game/HUD/ViewModels/Health/GASC_UVM_Health.h"
#include "GASCourse/GASCourseCharacter.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UGASC_HealthComponent::UGASC_HealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	bWantsInitializeComponent = true;
	SetIsReplicatedByDefault(true);
}

void UGASC_HealthComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UGASC_HealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UGASC_HealthComponent, CurrentHealth);
	DOREPLIFETIME(UGASC_HealthComponent, MaxHealth);
}

void UGASC_HealthComponent::OnRep_CurrentHealth()
{
	if(HealthViewModel)
	{
		HealthViewModel->SetCurrentHealth(CurrentHealth);
	}
}

void UGASC_HealthComponent::OnRep_MaxHealth()
{
	if(HealthViewModel)
	{
		HealthViewModel->SetMaxHealth(MaxHealth);
	}
}

void UGASC_HealthComponent::InitializeViewModel()
{
	UMVVMGameSubsystem* ViewModelGameSubsystem = GetOwner()->GetGameInstance()->GetSubsystem<UMVVMGameSubsystem>();
	check(ViewModelGameSubsystem);

	UMVVMViewModelCollectionObject* GlobalViewModelCollection = ViewModelGameSubsystem->GetViewModelCollection();
	check(GlobalViewModelCollection);

	UGASC_UVM_Health* CharacterHealthViewModel = NewObject<UGASC_UVM_Health>();
	FMVVMViewModelContext CharacterHealthViewModelContext;
	CharacterHealthViewModelContext.ContextClass = CharacterHealthViewModelContextClass;
	CharacterHealthViewModelContext.ContextName = CharacterHealthContextName;
	if(CharacterHealthViewModelContext.IsValid())
	{
		GlobalViewModelCollection->AddViewModelInstance(CharacterHealthViewModelContext, CharacterHealthViewModel);
		HealthViewModel = CharacterHealthViewModel;
	}

	OnHealthViewModelInstantiated.Broadcast(HealthViewModel);
}

void UGASC_HealthComponent::Server_InitializeHealthAttributes_Implementation()
{
	if(AGASCourseCharacter* OwningCharacter = Cast<AGASCourseCharacter>(GetOwner()))
	{
		if(UAbilitySystemComponent* OwningASC = Cast<UAbilitySystemComponent>(OwningCharacter->GetAbilitySystemComponent()))
		{
			if(const UGASCourseHealthAttributeSet* HealthAttributes = Cast<UGASCourseHealthAttributeSet>(OwningASC->GetAttributeSet(HealthAttributeSet)))
			{
				CurrentHealth = HealthAttributes->GetCurrentHealth();
				MaxHealth = HealthAttributes->GetMaxHealth();
				OnRep_CurrentHealth();
				OnRep_MaxHealth();
			}
		}
	}
}

void UGASC_HealthComponent::RegisterHealthComponent()
{
	InitializeViewModel();
	Server_InitializeHealthAttributes();
}



