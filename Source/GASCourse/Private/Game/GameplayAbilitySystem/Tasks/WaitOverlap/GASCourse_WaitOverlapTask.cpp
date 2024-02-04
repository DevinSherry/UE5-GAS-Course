// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GameplayAbilitySystem/Tasks/WaitOverlap/GASCourse_WaitOverlapTask.h"

#include "GASCourseNPC_Base.h"
#include "Components/SphereComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GASCourse_WaitOverlapTask)

UGASCourse_WaitOverlapTask::UGASCourse_WaitOverlapTask(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
}

void UGASCourse_WaitOverlapTask::OnComponentOverlapCallback(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	if(OtherActor && !OverlappedActors.Contains(OtherActor))
	{
		// Construct TargetData
		FGameplayAbilityTargetData_SingleTargetHit * TargetData = new FGameplayAbilityTargetData_SingleTargetHit(SweepResult);

		// Give it a handle and return
		FGameplayAbilityTargetDataHandle	Handle;
		Handle.Data.Add(TSharedPtr<FGameplayAbilityTargetData>(TargetData));
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnOverlap.Broadcast(Handle);
		}
		OverlappedActors.Add(OtherActor);
	}
}

void UGASCourse_WaitOverlapTask::Activate()
{
	if (InitializeOverlapComponent())
	{
		OverlapComponent->OnComponentBeginOverlap.AddDynamic(this, &UGASCourse_WaitOverlapTask::OnComponentOverlapCallback);
	}
}

UGASCourse_WaitOverlapTask* UGASCourse_WaitOverlapTask::WaitForOverlap(UGameplayAbility* OwningAbility,
	float InSphereRadius, bool bInDebugDraw)
{
	UGASCourse_WaitOverlapTask* MyObj = NewAbilityTask<UGASCourse_WaitOverlapTask>(OwningAbility);
	MyObj->SphereRadius = InSphereRadius;
	MyObj->bDebugDraw = bInDebugDraw;
	return MyObj;
}

void UGASCourse_WaitOverlapTask::OnDestroy(bool AbilityEnded)
{
	if (OverlapComponent)
	{
		OverlapComponent->OnComponentBeginOverlap.RemoveDynamic(this, &UGASCourse_WaitOverlapTask::OnComponentOverlapCallback);
		OverlapComponent->DestroyComponent();
	}

	OverlappedActors.Empty();
	Super::OnDestroy(AbilityEnded);
}

bool UGASCourse_WaitOverlapTask::InitializeOverlapComponent()
{
	if(const AActor* ActorOwner = GetAvatarActor())
	{
		OverlapComponent = NewObject<USphereComponent>(GetAvatarActor(), USphereComponent::StaticClass());
		if(OverlapComponent)
		{
			OverlapComponent->RegisterComponent();
#if WITH_EDITOR
			if(bDebugDraw)
			{
				OverlapComponent->SetHiddenInGame(false);
			}
#endif 			
			OverlapComponent->SetSphereRadius(SphereRadius);
			const FAttachmentTransformRules& TransformRules = FAttachmentTransformRules::SnapToTargetIncludingScale;
			OverlapComponent->AttachToComponent(ActorOwner->GetRootComponent(), TransformRules);
			OverlapComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			OverlapComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
			OverlapComponent->SetCollisionProfileName(TEXT("OverlapOnlyPawn"), true);

			TArray<AActor*> InitialOverlappingActors;
			OverlapComponent->GetOverlappingActors(InitialOverlappingActors);
			
			TArray<TWeakObjectPtr<AActor>> WeakActorArray;
			for(AActor* Actor : InitialOverlappingActors)
			{
				if(Actor && Actor != GetAvatarActor())
				{
					WeakActorArray.Add(Actor);
					OverlappedActors.Add(Actor);
				}
			}

			if(!OverlappedActors.IsEmpty())
			{
				FGameplayAbilityTargetData_ActorArray* TargetData_Actors = new FGameplayAbilityTargetData_ActorArray();
				TargetData_Actors->SetActors(WeakActorArray);
			
				// Give it a handle and return
				FGameplayAbilityTargetDataHandle	Handle;
				Handle.Data.Add(TSharedPtr<FGameplayAbilityTargetData>(TargetData_Actors));

				if (ShouldBroadcastAbilityTaskDelegates())
				{
					OnOverlap.Broadcast(Handle);
				}
			}
		}
	}

	return OverlapComponent ? true : false; 
}
