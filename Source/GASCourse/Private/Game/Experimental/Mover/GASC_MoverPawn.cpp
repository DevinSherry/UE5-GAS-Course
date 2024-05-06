// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Experimental/Mover/GASC_MoverPawn.h"


// Sets default values
AGASC_MoverPawn::AGASC_MoverPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGASC_MoverPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGASC_MoverPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AGASC_MoverPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

