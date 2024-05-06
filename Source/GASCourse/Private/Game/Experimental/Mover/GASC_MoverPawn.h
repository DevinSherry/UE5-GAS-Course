// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MoverSimulationTypes.h"
#include "GameFramework/Pawn.h"
#include "MoverSimulationTypes.h"
#include "GASC_MoverPawn.generated.h"

class UCharacterMoverComponent;

UCLASS()
class AGASC_MoverPawn : public APawn,  public IMoverInputProducerInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AGASC_MoverPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	
};
