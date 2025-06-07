// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Systems/Targeting/AreaofEffect/GASC_TargetingPreset_AOE.h"
#include "Game/Systems/Targeting/AreaofEffect/GASC_TargetFilter_ActorClass.h"
#include "Game/Systems/Targeting/AreaofEffect/GASC_TargetingSelectionTask_AOE.h"
#include "GASCourse/GASCourseCharacter.h"

UGASC_TargetingPreset_AOE::UGASC_TargetingPreset_AOE(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	UGASC_TargetingSelectionTask_AOE* AOESelectionTask = ObjectInitializer.CreateDefaultSubobject<UGASC_TargetingSelectionTask_AOE>(this, TEXT("GASC AOE Task"));
	AOESelectionTask->SetShapeType(ETargetingAOEShape::Sphere);
	AOESelectionTask->SetRadius(100.0f);
	AOESelectionTask->SetCollisionCollisionChannel(ECC_Pawn);
	AOESelectionTask->SetIgnoreSourceActor(true);
	AOESelectionTask->SetIgnoreInstigatorActor(true);
	AOESelectionTask->AddCollisionObjectTypes(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	TargetingTaskSet.Tasks.Add(AOESelectionTask);

	UGASC_TargetFilter_ActorClass* AOEFilter = ObjectInitializer.CreateDefaultSubobject<UGASC_TargetFilter_ActorClass>(this, TEXT("GASC AOE Class Filter"));
	AOEFilter->AddRequiredActorClassFilter(AGASCourseCharacter::StaticClass());
	TargetingTaskSet.Tasks.Add(AOEFilter);
}
