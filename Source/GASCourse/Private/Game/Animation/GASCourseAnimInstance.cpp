// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Animation/GASCourseAnimInstance.h"
#include "AbilitySystemGlobals.h"
#include "../GASCourseCharacter.h"
#include "Game/Character/Components/GASCourseMovementComponent.h"
#include "Misc/DataValidation.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GASCourseAnimInstance)

UGASCourseAnimInstance::UGASCourseAnimInstance(const FObjectInitializer& ObjectInitializer)
{
}

void UGASCourseAnimInstance::InitializeWithAbilitySystem(UAbilitySystemComponent* ASC)
{
	check(ASC);
	GameplayTagPropertyMap.Initialize(this, ASC);
}

#if WITH_EDITOR
EDataValidationResult UGASCourseAnimInstance::IsDataValid(FDataValidationContext& Context) const
{
	Super::IsDataValid(Context);
	
	GameplayTagPropertyMap.IsDataValid(this, Context);
	return ((Context.GetNumErrors() > 0) ? EDataValidationResult::Invalid : EDataValidationResult::Valid);
}
#endif // WITH_EDITOR

void UGASCourseAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (const AActor* OwningActor = GetOwningActor())
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwningActor))
		{
			InitializeWithAbilitySystem(ASC);
		}
	}
}

void UGASCourseAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	const AGASCourseCharacter* Character = Cast<AGASCourseCharacter>(GetOwningActor());
	if (!Character)
	{
		return;
	}

	UGASCourseMovementComponent* CharMoveComp = CastChecked<UGASCourseMovementComponent>(Character->GetCharacterMovement());
	const FGASCharacterGroundInfo& GroundInfo = CharMoveComp->GetGroundInfo();
	GroundDistance = GroundInfo.GroundDistance;
}
