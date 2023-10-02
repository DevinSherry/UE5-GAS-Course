// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "Game/GameplayAbilitySystem/GASCourseGameplayAbilitySet.h"
#include "GASCourseChampionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GASCOURSE_API UGASCourseChampionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGASCourseChampionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Abilities")
	TObjectPtr<UGASCourseGameplayAbilitySet> DefaultAbilitySet;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	TObjectPtr<UGASAbilityTagRelationshipMapping> AbilityTagRelationshipMapping;

public:	

	void InitializeChampionComponent(UGASCourseAbilitySystemComponent* ASC);

	UFUNCTION()
	FORCEINLINE UGASCourseGameplayAbilitySet* GetDefaultAbilitySet() const
	{
		return (DefaultAbilitySet) ? DefaultAbilitySet : nullptr;
	}
	
	UFUNCTION()
	FORCEINLINE UGASAbilityTagRelationshipMapping* GetAbilityTagRelationshipMapping() const
	{
		return (AbilityTagRelationshipMapping) ? AbilityTagRelationshipMapping : nullptr;
	}
		
};
