// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Character/Projectile/GASCourseProjectile.h"
#include "Net/UnrealNetwork.h"
#include "GASCourseCharacter.h"
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

void AGASCourseProjectile::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(AGASCourseProjectile, TargetActor, COND_None, REPNOTIFY_Always);
}

// Called when the game starts or when spawned
void AGASCourseProjectile::BeginPlay()
{
	Super::BeginPlay();

	ProjectileCollisionComp->IgnoreActorWhenMoving(GetInstigator(), true);
	if(AGASCourseCharacter* TargetCharacter = Cast<AGASCourseCharacter>(TargetActor))
	{
		ProjectileMovementComp->bIsHomingProjectile = true;
		ProjectileMovementComp->HomingTargetComponent = TargetCharacter->GetTargetingSceneComponent();
	}
}

// Called every frame
void AGASCourseProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(ProjectileMovementComp->bIsHomingProjectile && TargetActor)
	{
		if(ProjectileMovementComp->bDisableHomingBasedOnDotProduct)
		{
			float DotToTarget = GetHorizontalDotProductTo(TargetActor);
			if(DotToTarget <= ProjectileMovementComp->DisableHomingDotProductMin)
			{
				ProjectileMovementComp->bIsHomingProjectile = false;
			}
		}
	}
}

