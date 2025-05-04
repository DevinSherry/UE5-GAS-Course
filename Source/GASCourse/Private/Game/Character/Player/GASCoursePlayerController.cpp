// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Character/Player/GASCoursePlayerController.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Game/GameplayAbilitySystem/GASCourseNativeGameplayTags.h"
#include "GASCourse/GASCourseCharacter.h"
#include "Components/StateTreeComponent.h"
#include "Kismet/GameplayStatics.h"

AGASCoursePlayerController::AGASCoursePlayerController(const FObjectInitializer& ObjectInitializer)
{
	bEnableMouseOverEvents = true;
	PlayerStateTreeComponent = CreateDefaultSubobject<UStateTreeComponent>(TEXT("StateTreeComponent"));
}


void AGASCoursePlayerController::InitializeStateTree()
{
	
}

void AGASCoursePlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void AGASCoursePlayerController::BeginPlayingState()
{
	Super::BeginPlayingState();
}

void AGASCoursePlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void AGASCoursePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
}

AGASCoursePlayerState* AGASCoursePlayerController::GetGASCoursePlayerState() const
{
	return CastChecked<AGASCoursePlayerState>(PlayerState, ECastCheckedType::NullAllowed);
}

UGASCourseAbilitySystemComponent* AGASCoursePlayerController::GetGASCourseAbilitySystemComponent() const
{
	const AGASCoursePlayerState* PS = GetGASCoursePlayerState();
	return (PS ? PS->GetAbilitySystemComponent() : nullptr);
}

void AGASCoursePlayerController::PreProcessInput(const float DeltaTime, const bool bGamePaused)
{
	Super::PreProcessInput(DeltaTime, bGamePaused);
}

void AGASCoursePlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	if (UGASCourseAbilitySystemComponent* ASC = GetGASCourseAbilitySystemComponent())
	{
		ASC->ProcessAbilityInput(DeltaTime, bGamePaused);
	}
	
	Super::PostProcessInput(DeltaTime, bGamePaused);
}

void AGASCoursePlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AGASCoursePlayerState* PS = GetPlayerState<AGASCoursePlayerState>();
	if (PS)
	{
		// Init ASC with PS (Owner) and our new Pawn (AvatarActor)
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, InPawn);
	}

	if(UGASCourseAbilitySystemComponent* ASC = GetGASCourseAbilitySystemComponent())
	{
		ASC->GenericGameplayEventCallbacks.FindOrAdd(Event_Gameplay_OnDamageDealt).AddUObject(this, &AGASCoursePlayerController::OnDamageDealtCallback);
	}
	
	if (PlayerStateTreeComponent)
	{
		InitializeStateTree();
		PlayerStateTreeComponent->StartLogic();
	}

}

void AGASCoursePlayerController::CreateHUD_Implementation()
{

}

bool AGASCoursePlayerController::InputKey(const FInputKeyParams& Params)
{
	return Super::InputKey(Params);
}

void AGASCoursePlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	// Needed in case the PC wasn't valid when we Init-ed the ASC.
	if (AGASCoursePlayerState* PS = GetPlayerState<AGASCoursePlayerState>())
	{
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, GetPawn());
	}
	CreateHUD();
}

void AGASCoursePlayerController::OnRep_Pawn()
{
	Super::OnRep_Pawn();
}

void AGASCoursePlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	GetMousePositionInViewport();
}

void AGASCoursePlayerController::OnDamageDealtCallback(const FGameplayEventData* Payload)
{
	OnDamageDealt(*Payload);
}

void AGASCoursePlayerController::GetMousePositionInViewport()
{
	const FVector2D MousePositionInViewport = UWidgetLayoutLibrary::GetMousePositionOnViewport(this);
	UGameplayStatics::DeprojectScreenToWorld(this, MousePositionInViewport, MousePositionDeprojectedToWorld, MouseDirectionDeprojectedToWorld);
}
