// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "GASCourseInputConfig.h"
#include "GameplayTagContainer.h"
#include "GASCourseEnhancedInputComponent.generated.h"

/**
 * 
 */
UCLASS()
class GASCOURSE_API UGASCourseEnhancedInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
	
public:

	template<class UserClass, typename FuncType>
	void BindActionByTag(const UGASCourseInputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func);
};

template<class UserClass, typename FuncType>
void UGASCourseEnhancedInputComponent::BindActionByTag(const UGASCourseInputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func)
{
	check(InputConfig);
	if (const UInputAction* IA = InputConfig->FindInputActionForTag(InputTag))
	{
		BindAction(IA, TriggerEvent, Object, Func);
	}
}
