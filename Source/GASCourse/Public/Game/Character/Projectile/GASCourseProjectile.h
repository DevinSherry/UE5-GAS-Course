// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "Types/TargetingSystemTypes.h"
#include "GASCourseProjectile.generated.h"

class TargetingSystemTypes;

UCLASS()
class GASCOURSE_API AGASCourseProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGASCourseProjectile();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** Projectile Movement Component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Projectile, meta = (AllowPrivateAccess = "true"))
	class UGASCourseProjectileMovementComp* ProjectileMovementComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Projectile, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* ProjectileCollisionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,  Category = Projectile, meta = (ExposeOnSpawn=true), Replicated)
	class AActor* TargetActor = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Ricochet")
	bool bCanRicochet;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Ricochet")
	FGameplayTagQuery RicochetTagRequirements;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Ricochet")
	UTargetingPreset* RicochetTargetingPreset;

	UFUNCTION(BlueprintNativeEvent)
	void OnProjectileRicochet();


protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ricochet")
	TArray<AActor*> HitTargets;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit );

	UFUNCTION()
	void OnTargetRequestCompleted(FTargetingRequestHandle TargetingRequestHandle);

	UFUNCTION()
	void OnTargetDeathCallback(FGameplayTag MatchingTag, int32 NewCount);

private:
	
	UPROPERTY()
	FTargetingRequestHandle CurrentTargetHandle;

	UPROPERTY()
	TArray<AActor*> FoundTargets;

	FDelegateHandle OnTargetDeathDelegateHandle;

	UFUNCTION()
	void EnableProjectileHoming();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
