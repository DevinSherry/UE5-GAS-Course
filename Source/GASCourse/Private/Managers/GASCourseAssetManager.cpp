// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/GASCourseAssetManager.h"
#include "Game/GameplayAbilitySystem/GASCourseNativeGameplayTags.h"

UGASCourseAssetManager::UGASCourseAssetManager()
{

}

UGASCourseAssetManager& UGASCourseAssetManager::Get()
{
	check(GEngine);

	UGASCourseAssetManager* MyAssetManager = Cast<UGASCourseAssetManager>(GEngine->AssetManager);
	return *MyAssetManager;
}

void UGASCourseAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	//Load Native Tags
	FGASCourseNativeGameplayTags::InitializeNativeTags();
}
