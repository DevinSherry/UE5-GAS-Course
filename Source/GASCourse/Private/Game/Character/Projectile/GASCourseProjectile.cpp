// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Character/Projectile/GASCourseProjectile.h"
#include "Net/UnrealNetwork.h"
#include "GASCourse/GASCourseCharacter.h"
#include "Game/Character/Projectile/Components/GASCourseProjectileMovementComp.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "TargetingSystem/TargetingSubsystem.h"
#include "Types/TargetingSystemTypes.h"

// Sets default values
AGASCourseProjectile::AGASCourseProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ProjectileMovementComp = CreateDefaultSubobject<UGASCourseProjectileMovementComp>(TEXT("ProjectileMovementComp"));
	ProjectileCollisionComp = CreateDefaultSubobject<USphereComponent>("ProjectileCollisionComp");

	SetRootComponent(ProjectileCollisionComp);

	ProjectileCollisionComp->SetCollisionProfileName("Projectile");
	ProjectileCollisionComp->SetEnableGravity(false);
}

void AGASCourseProjectile::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(AGASCourseProjectile, TargetActor, COND_None, REPNOTIFY_Always);
}

void AGASCourseProjectile::OnProjectileRicochet_Implementation()
{
}

void AGASCourseProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                 FVector NormalImpulse, const FHitResult& Hit)
{
	HitTargets.Add(OtherActor);

	ProjectileCollisionComp->IgnoreActorWhenMoving(OtherActor, true);
	ProjectileCollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if(UTargetingSubsystem* TargetingSubsystem = UTargetingSubsystem::Get(GetInstigator()->GetWorld()))
	{
		FTargetingSourceContext TargetingSourceContext;
		TargetingSourceContext.InstigatorActor = GetInstigator();
		TargetingSourceContext.SourceActor = this;
		TargetingSourceContext.SourceLocation = GetActorLocation();
		TargetingSourceContext.SourceObject = this;
		FTargetingRequestDelegate OnCompletedDelegate;
		OnCompletedDelegate.BindUFunction(this, FName("OnTargetRequestCompleted"));
		CurrentTargetHandle = UTargetingSubsystem::MakeTargetRequestHandle(RicochetTargetingPreset, TargetingSourceContext);
		FTargetingAsyncTaskData& AsyncTaskData = FTargetingAsyncTaskData::FindOrAdd(CurrentTargetHandle);
		AsyncTaskData.bReleaseOnCompletion = true;
		TargetingSubsystem->StartAsyncTargetingRequestWithHandle(CurrentTargetHandle, OnCompletedDelegate);
	}
}

void AGASCourseProjectile::OnTargetRequestCompleted(FTargetingRequestHandle TargetingRequestHandle)
{
	UE_LOG(LogTemp, Warning, TEXT("TargetRequestCompleted"));

	if(UTargetingSubsystem* TargetingSubsystem = UTargetingSubsystem::Get(GetInstigator()->GetWorld()))
	{
		TargetingSubsystem->GetTargetingResultsActors(TargetingRequestHandle, FoundTargets);
		if(FoundTargets.IsEmpty())
		{
			Destroy();
			return;
		}

		TargetingSubsystem->RemoveAsyncTargetingRequestWithHandle(TargetingRequestHandle);

		for(int32 i = FoundTargets.Num() -1; i >= 0;  --i)
		{
			if(HitTargets.Contains(FoundTargets[i]))
			{
				FoundTargets.RemoveAt(i);
			}
		}

		if(FoundTargets.IsEmpty())
		{
			Destroy();
			return;
		}

		ProjectileCollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryAndProbe);
		if(AActor* NewTarget = FoundTargets[0])
		{
			ProjectileMovementComp->HomingTargetComponent = NewTarget->GetRootComponent();
			FRotator RotationDirection = (UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), NewTarget->GetActorLocation()));
			FVector DirectionVector = RotationDirection.Vector();
			DirectionVector.Normalize();
			ProjectileMovementComp->Velocity = DirectionVector * ProjectileMovementComp->InitialSpeed;
			ProjectileMovementComp->bIsHomingProjectile = true;

			OnProjectileRicochet();
		}
		else
		{
			Destroy();
		}
	}
}

// Called when the game starts or when spawned
void AGASCourseProjectile::BeginPlay()
{
	Super::BeginPlay();

	ProjectileCollisionComp->OnComponentHit.AddDynamic(this, &ThisClass::AGASCourseProjectile::OnHit);
	if(!bCanRicochet)
	{
		if(AGASCourseCharacter* InstigatorActor = Cast<AGASCourseCharacter>(GetInstigator()))
		{
			FGameplayTagContainer OwnedTags = InstigatorActor->GetAbilitySystemComponent()->GetOwnedGameplayTags();
			bCanRicochet = OwnedTags.MatchesQuery(RicochetTagRequirements);
		}
	}

	ProjectileMovementComp->bShouldBounce = bCanRicochet;
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

