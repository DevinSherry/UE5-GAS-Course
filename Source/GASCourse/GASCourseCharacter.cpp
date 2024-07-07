// Copyright Epic Games, Inc. All Rights Reserved.

#include "GASCourseCharacter.h"
#include "Game/Character/Components/GASCStatusEffectListenerComp.h"
#include "Game/Character/Components/Health/GASC_HealthComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Game/GameplayAbilitySystem/GASCourseGameplayAbilitySet.h"
#include "Game/GameplayAbilitySystem/GASCourseNativeGameplayTags.h"
#include "Game/Character/Components/GASCourseMovementComponent.h"
#include "Net/UnrealNetwork.h"


//////////////////////////////////////////////////////////////////////////
// AGASCourseCharacter

AGASCourseCharacter::AGASCourseCharacter(const class FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass<UGASCourseMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	SetReplicates(true);
	
	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	TargetingSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("TargetingComponent"));
	TargetingSceneComponent->SetupAttachment(RootComponent);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them

	//JumpZVelocity is overridden by the base attribute set value, JumpZVelocityOverride.
	GetCharacterMovement()->JumpZVelocity = 700.f;
	//MaxWalkSpeed is overridden by the base attribute set value, MovementSpeed.
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	StatusEffectListenerComp = CreateDefaultSubobject<UGASCStatusEffectListenerComp>(TEXT("StatusEffectListenerComp"));
	CharacterHealthComponent = CreateDefaultSubobject<UGASC_HealthComponent>(TEXT("CharacterHealthComponent"));
}

void AGASCourseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AGASCourseCharacter, RotateToDirection);
	DOREPLIFETIME(AGASCourseCharacter, ReplicationVarList);
	DOREPLIFETIME(AGASCourseCharacter, RepAnimMontageInfo);
}

void AGASCourseCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	PostBeginPlayHealthComponentRegistration();
	
	GameplayEffectAssetTagsToRemove.AddTag(FGameplayTag::RequestGameplayTag(FName("Effect.AssetTag.Status")));
	if(AbilitySystemComponent)
	{
		if(StatusEffectListenerComp)
		{
			if(GetLocalRole() == ROLE_Authority)
			{
				AbilitySystemComponent->OnGameplayEffectAppliedDelegateToSelf.AddUObject(StatusEffectListenerComp, &UGASCStatusEffectListenerComp::OnStatusEffectApplied_Server);
			}
			else
			{
				AbilitySystemComponent->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(StatusEffectListenerComp, &UGASCStatusEffectListenerComp::OnStatusEffectApplied_Client);
			}
			AbilitySystemComponent->OnAnyGameplayEffectRemovedDelegate().AddUObject(StatusEffectListenerComp, &UGASCStatusEffectListenerComp::OnStatusEffectRemoved);
			StatusEffectListenerComp->ApplyDefaultActiveStatusEffects();
		}
	}
}

void AGASCourseCharacter::InitializeAbilitySystem(UGASCourseAbilitySystemComponent* InASC)
{
	if(GetLocalRole() != ROLE_Authority || !InASC)
	{
		return;
	}
	if(DefaultAbilitySet)
	{
		DefaultAbilitySet->GiveToAbilitySystem(InASC, nullptr);
	}

	if(AbilityTagRelationshipMapping)
	{
		InASC->SetTagRelationshipMapping(AbilityTagRelationshipMapping);
	}

	if(GameplayStatusEffectTable)
	{
		InASC->SetGameplayEffectStatusTable(GameplayStatusEffectTable);
	}

	if(InASC)
	{
		InASC->AddGameplayEventTagContainerDelegate(FGameplayTagContainer(Event_OnDeath), FGameplayEventTagMulticastDelegate::FDelegate::CreateUObject(this, &ThisClass::CharacterDeathGameplayEventCallback));
		InASC->RegisterGameplayTagEvent(FGameplayTag(Collision_IgnorePawn), EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AGASCourseCharacter::IgnorePawnCollisionGameplayTagEventCallback);
	}
}

void AGASCourseCharacter::RegisterViewModels()
{
	/*
	 * Register all components that use view models here.
	 */

	//Register character health component & view model.
	if(CharacterHealthComponent)
	{
		CharacterHealthComponent->RegisterHealthComponent();
	}
}

void AGASCourseCharacter::PostBeginPlayHealthComponentRegistration_Implementation()
{
}

void AGASCourseCharacter::CharacterDeathGameplayEventCallback(FGameplayTag MatchingTag,
                                                              const FGameplayEventData* Payload)
{
	if(UGASCourseAbilitySystemComponent* InASC = GetAbilitySystemComponent())
	{
		InASC->RemoveActiveEffectsWithTags(GameplayEffectAssetTagsToRemove);
	}
}

void AGASCourseCharacter::IgnorePawnCollisionGameplayTagEventCallback(FGameplayTag MatchingTag, int32 NewCount)
{
	if(MatchingTag == Collision_IgnorePawn)
	{
		if(NewCount > 0)
		{
			GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
		}
		else
		{
			GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, DefaultCollisionResponseToPawn);
		}
	}
}

UGASCourseAbilitySystemComponent* AGASCourseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

float AGASCourseCharacter::GetCrouchSpeed() const
{
	if (const UGASCourseCharBaseAttributeSet* BaseAttributeSet = GetAbilitySystemComponent()->GetSetChecked<UGASCourseCharBaseAttributeSet>())
	{
		return BaseAttributeSet->GetCrouchSpeed();
	}
	UE_LOG(LogTemp, Warning, TEXT("NO VALID ATTRIBUTE SET FOUND"));
	return 0.0f;
}

float AGASCourseCharacter::GetJumpZVelocityOverride() const
{
	if (const UGASCourseCharBaseAttributeSet* BaseAttributeSet = GetAbilitySystemComponent()->GetSetChecked<UGASCourseCharBaseAttributeSet>())
	{
		return BaseAttributeSet->GetJumpZVelocityOverride();
	}
	UE_LOG(LogTemp, Warning, TEXT("NO VALID ATTRIBUTE SET FOUND"));
	return 0.0f;
}

float AGASCourseCharacter::GetCurrentHealth() const
{
	if (const UGASCourseHealthAttributeSet* BaseAttributeSet = GetAbilitySystemComponent()->GetSetChecked<UGASCourseHealthAttributeSet>())
	{
		return BaseAttributeSet->GetCurrentHealth();
	}
	UE_LOG(LogTemp, Warning, TEXT("NO VALID ATTRIBUTE SET FOUND"));
	return 0.0f;
}

void AGASCourseCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	const FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		if (UGASCourseAbilitySystemComponent* GASCourseASC = GetAbilitySystemComponent())
		{
			//Block any type of movement if character has tag Status.MovementInputBlocked
			if(GASCourseASC->HasMatchingGameplayTag(Status_Block_MovementInput))
			{
				return;
			}
			if(MovementVector.Length() > 0.0f)
			{
				GASCourseASC->SetLooseGameplayTagCount(Status_IsMoving, 1);
			}
			// find out which way is forward
			const FRotator Rotation = Controller->GetControlRotation(); //TODO: Maybe breaks something?
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get forward vector
			const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
			// get right vector 
			const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

			// add movement 
			AddMovementInput(ForwardDirection, MovementVector.Y);
			AddMovementInput(RightDirection, MovementVector.X);
		}
	}
}

void AGASCourseCharacter::StopMove(const FInputActionValue& Value)
{
	// input is a Vector2D
	const FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		if (UGASCourseAbilitySystemComponent* GASCourseASC = GetAbilitySystemComponent())
		{
			//Block any type of movement if character has tag Status.MovementInputBlocked
			if(GASCourseASC->HasMatchingGameplayTag(Status_Block_MovementInput))
			{
				return;
			}
			if(FMath::IsNearlyZero(MovementVector.Length()))
			{
				GASCourseASC->SetLooseGameplayTagCount(Status_IsMoving, 0);
			}
		}
	}
}

void AGASCourseCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AGASCourseCharacter::Input_Crouch(const FInputActionValue& Value)
{
	const UGASCourseAbilitySystemComponent* GASCourseASC = GetAbilitySystemComponent();
	//Block any type of movement if character has tag Status.MovementInputBlocked
	if(GASCourseASC->HasMatchingGameplayTag(Status_Block_MovementInput))
	{
		return;
	}
	const UCharacterMovementComponent* GASCharacterMovementComponent = CastChecked<UCharacterMovementComponent>(GetCharacterMovement());
	
	if (bIsCrouched || GASCharacterMovementComponent->bWantsToCrouch)
	{
		UnCrouch();
	}
	else if (GASCharacterMovementComponent->IsMovingOnGround())
	{
		Crouch();
	}
}

void AGASCourseCharacter::OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	UGASCourseAbilitySystemComponent* GASCourseASC = GetAbilitySystemComponent();
	//Block any type of movement if character has tag Status.MovementInputBlocked
	if(GASCourseASC->HasMatchingGameplayTag(Status_Block_MovementInput))
	{
		return;
	}
	
	GASCourseASC->SetLooseGameplayTagCount(Status_Crouching, 1);
	Super::OnStartCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
}

void AGASCourseCharacter::OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	UGASCourseAbilitySystemComponent* GASCourseASC = GetAbilitySystemComponent();
	//Block any type of movement if character has tag Status.MovementInputBlocked
	if(GASCourseASC->HasMatchingGameplayTag(Status_Block_MovementInput))
	{
		return;
	}
	
	GASCourseASC->SetLooseGameplayTagCount(Status_Crouching, 0);
	Super::OnEndCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
}

bool AGASCourseCharacter::CanJumpInternal_Implementation() const
{
	const UGASCourseAbilitySystemComponent* GASCourseASC = GetAbilitySystemComponent();
	if(GASCourseASC->HasMatchingGameplayTag(Status_Block_MovementInput))
	{
		return false;
	}
	const UGASCourseMovementComponent* GASCharacterMovementComponent = CastChecked<UGASCourseMovementComponent>(GetCharacterMovement());
	if(GASCharacterMovementComponent->bAllowJumpFromCrouch)
	{
		return JumpIsAllowedInternal();
	}
	
	return Super::CanJumpInternal_Implementation();
}

void AGASCourseCharacter::Jump()
{
	const UGASCourseMovementComponent* GASCharacterMovementComponent = CastChecked<UGASCourseMovementComponent>(GetCharacterMovement());
	if(GASCharacterMovementComponent->bAllowJumpFromCrouch)
	{
		UnCrouch();
	}
	
	Super::Jump();
}

void AGASCourseCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	check(AbilitySystemComponent);
	check(GetCapsuleComponent());

	check(StatusEffectListenerComp);

	DefaultCollisionResponseToPawn = GetCapsuleComponent()->GetCollisionResponseToChannel(ECC_Pawn);
}

FReplicationProxyVarList& AGASCourseCharacter::Call_GetReplicationProxyVarList_Mutable()
{
	MARK_PROPERTY_DIRTY_FROM_NAME(AGASCourseCharacter, ReplicationVarList, this);
	return ReplicationVarList;
}

void AGASCourseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

void AGASCourseCharacter::OnRep_ReplicationVarList()
{
	
	UGASCourseAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	if (ASC)
	{
		// Update ASC client version of RepAnimMontageInfo
		ASC->SetNumericAttributeBase(UGASCourseCharBaseAttributeSet::GetMovementSpeedAttribute(), ReplicationVarList.AttributeOne);
		ASC->SetNumericAttributeBase(UGASCourseCharBaseAttributeSet::GetCrouchSpeedAttribute(), ReplicationVarList.AttributeTwo);

		// Here you should add the tags to the simulated proxies ie.:
		if(bool bFirstTagExists = ((ReplicationVarList.GameplayTagsBitMask & 0x01) != 0))
		{
			ASC->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag("Data.Sample"));
		}
		else
		{
			ASC->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag("Data.Sample"));
		}
	}
}

void AGASCourseCharacter::SetCharacterRotation_Client_Implementation(FRotator InRotation)
{
	RotateToDirection = InRotation;
	SetActorRotation(RotateToDirection);
	ForceNetUpdate();
	
	if(GetLocalRole() == ROLE_Authority)
	{
		SetActorRotation(RotateToDirection);
		ForceNetUpdate();
	}
	else
	{
		SetCharacterRotation_Server(RotateToDirection);
	}
}

void AGASCourseCharacter::SetCharacterRotation_Server_Implementation(FRotator InRotation)
{
	SetCharacterRotation_Multicast(InRotation);
	ForceNetUpdate();
}

void AGASCourseCharacter::SetCharacterRotation_Multicast_Implementation(FRotator InRotation)
{
	SetActorRotation(InRotation);
	ForceNetUpdate();
}

void AGASCourseCharacter::ForceReplication()
{
	ForceNetUpdate();
}

void AGASCourseCharacter::NetMulticast_InvokeGameplayCueExecuted_FromSpec_Implementation(const FGameplayEffectSpecForRPC Spec, FPredictionKey PredictionKey)
{
	if (HasAuthority() || PredictionKey.IsLocalClientKey() == false)
	{
		GetAbilitySystemComponent()->InvokeGameplayCueEvent(Spec, EGameplayCueEvent::Executed);
	}
}

void AGASCourseCharacter::NetMulticast_InvokeGameplayCueExecuted_Implementation(const FGameplayTag GameplayCueTag, FPredictionKey PredictionKey, FGameplayEffectContextHandle EffectContext)
{
	if (HasAuthority() || PredictionKey.IsLocalClientKey() == false)
	{
		GetAbilitySystemComponent()->InvokeGameplayCueEvent(GameplayCueTag, EGameplayCueEvent::Executed, EffectContext);
	}
}

void AGASCourseCharacter::NetMulticast_InvokeGameplayCuesExecuted_Implementation(const FGameplayTagContainer GameplayCueTags, FPredictionKey PredictionKey, FGameplayEffectContextHandle EffectContext)
{
	if (HasAuthority() || PredictionKey.IsLocalClientKey() == false)
	{
		for (const FGameplayTag& GameplayCueTag : GameplayCueTags)
		{
			GetAbilitySystemComponent()->InvokeGameplayCueEvent(GameplayCueTag, EGameplayCueEvent::Executed, EffectContext);
		}
	}
}

void AGASCourseCharacter::NetMulticast_InvokeGameplayCueExecuted_WithParams_Implementation(const FGameplayTag GameplayCueTag, FPredictionKey PredictionKey, FGameplayCueParameters GameplayCueParameters)
{
	if (HasAuthority() || PredictionKey.IsLocalClientKey() == false)
	{
		GetAbilitySystemComponent()->InvokeGameplayCueEvent(GameplayCueTag, EGameplayCueEvent::Executed, GameplayCueParameters);
	}
}

void AGASCourseCharacter::NetMulticast_InvokeGameplayCuesExecuted_WithParams_Implementation(const FGameplayTagContainer GameplayCueTags, FPredictionKey PredictionKey, FGameplayCueParameters GameplayCueParameters)
{
	if (HasAuthority() || PredictionKey.IsLocalClientKey() == false)
	{
		for (const FGameplayTag& GameplayCueTag : GameplayCueTags)
		{
			GetAbilitySystemComponent()->InvokeGameplayCueEvent(GameplayCueTag, EGameplayCueEvent::Executed, GameplayCueParameters);
		}
	}
}

void AGASCourseCharacter::NetMulticast_InvokeGameplayCueAdded_Implementation(const FGameplayTag GameplayCueTag, FPredictionKey PredictionKey, FGameplayEffectContextHandle EffectContext)
{
	if (HasAuthority() || PredictionKey.IsLocalClientKey() == false)
	{
		GetAbilitySystemComponent()->InvokeGameplayCueEvent(GameplayCueTag, EGameplayCueEvent::OnActive, EffectContext);
	}
}

void AGASCourseCharacter::NetMulticast_InvokeGameplayCueAdded_WithParams_Implementation(const FGameplayTag GameplayCueTag, FPredictionKey PredictionKey, FGameplayCueParameters Parameters)
{
	// If server generated prediction key and auto proxy, skip this message. 
	// This is an RPC from mixed replication mode code, we will get the "real" message from our OnRep on the autonomous proxy
	// See UAbilitySystemComponent::AddGameplayCue_Internal for more info.
	
	bool bIsMixedReplicationFromServer = (GetAbilitySystemComponent()->ReplicationMode == EGameplayEffectReplicationMode::Mixed && PredictionKey.IsServerInitiatedKey() && IsLocallyControlled());

	if (HasAuthority() || (PredictionKey.IsLocalClientKey() == false && !bIsMixedReplicationFromServer))
	{
		GetAbilitySystemComponent()->InvokeGameplayCueEvent(GameplayCueTag, EGameplayCueEvent::OnActive, Parameters);
	}
}


void AGASCourseCharacter::NetMulticast_InvokeGameplayCueAddedAndWhileActive_FromSpec_Implementation(const FGameplayEffectSpecForRPC& Spec, FPredictionKey PredictionKey)
{
	if (HasAuthority() || PredictionKey.IsLocalClientKey() == false)
	{
		GetAbilitySystemComponent()->InvokeGameplayCueEvent(Spec, EGameplayCueEvent::OnActive);
		GetAbilitySystemComponent()->InvokeGameplayCueEvent(Spec, EGameplayCueEvent::WhileActive);
	}
}

void AGASCourseCharacter::NetMulticast_InvokeGameplayCueAddedAndWhileActive_WithParams_Implementation(const FGameplayTag GameplayCueTag, FPredictionKey PredictionKey, FGameplayCueParameters GameplayCueParameters)
{
	if (HasAuthority() || PredictionKey.IsLocalClientKey() == false)
	{
		GetAbilitySystemComponent()->InvokeGameplayCueEvent(GameplayCueTag, EGameplayCueEvent::OnActive, GameplayCueParameters);
		GetAbilitySystemComponent()->InvokeGameplayCueEvent(GameplayCueTag, EGameplayCueEvent::WhileActive, GameplayCueParameters);
	}
}

void AGASCourseCharacter::NetMulticast_InvokeGameplayCuesAddedAndWhileActive_WithParams_Implementation(const FGameplayTagContainer GameplayCueTags, FPredictionKey PredictionKey, FGameplayCueParameters GameplayCueParameters)
{
	if (HasAuthority() || PredictionKey.IsLocalClientKey() == false)
	{
		for (const FGameplayTag& GameplayCueTag : GameplayCueTags)
		{
			GetAbilitySystemComponent()->InvokeGameplayCueEvent(GameplayCueTag, EGameplayCueEvent::OnActive, GameplayCueParameters);
			GetAbilitySystemComponent()->InvokeGameplayCueEvent(GameplayCueTag, EGameplayCueEvent::WhileActive, GameplayCueParameters);
		}
	}
}

FGameplayAbilityRepAnimMontage& AGASCourseCharacter::Call_GetRepAnimMontageInfo_Mutable()
{
	MARK_PROPERTY_DIRTY_FROM_NAME(AGASCourseCharacter, RepAnimMontageInfo, this);
	return RepAnimMontageInfo;
}

void AGASCourseCharacter::Call_OnRep_ReplicatedAnimMontage()
{
	UGASCourseAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	if (ASC)
	{
		// Update ASC client version of RepAnimMontageInfo
		ASC->SetRepAnimMontageInfoAccessor(RepAnimMontageInfo);
		// Call OnRep of AnimMontageInfo
		ASC->ReplicatedAnimMontageOnRepAccesor();
	}
}