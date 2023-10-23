// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Character/Projectile/GASCourseProjectile.h"
#include "Game/Character/Projectile/Components/GASCourseProjectileMovementComp.h"
#include "Components/SphereComponent.h"

// Sets default values
AGASCourseProjectile::AGASCourseProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ProjectileMovementComp = CreateDefaultSubobject<UGASCourseProjectileMovementComp>(TEXT("ProjectileMovementComp"));
	ProjectileCollisionComp =CreateDefaultSubobject<USphereComponent>("ProjectileCollisionComp");

	SetRootComponent(ProjectileCollisionComp);

	ProjectileCollisionComp->SetCollisionProfileName("Projectile");
	ProjectileCollisionComp->SetEnableGravity(false);
}

// Called when the game starts or when spawned
void AGASCourseProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGASCourseProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

