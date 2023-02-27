// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GASCourse/GASCourseCharacter.h"
#include "GASCoursePlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class GASCOURSE_API AGASCoursePlayerCharacter : public AGASCourseCharacter
{
	GENERATED_BODY()

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContextKBM;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContextGamepad;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UGASCourseInputConfig* DefaultInputConfig;

public:

	AGASCoursePlayerCharacter();

protected:

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Add GASCourseAbilitySystemComponent on PossessedBy
	virtual void PossessedBy(AController* NewController) override;

	virtual void OnRep_PlayerState() override;
	
};
