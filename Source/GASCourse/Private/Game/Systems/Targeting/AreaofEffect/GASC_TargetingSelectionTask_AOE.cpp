// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Systems/Targeting/AreaofEffect/GASC_TargetingSelectionTask_AOE.h"


FCollisionShape UGASC_TargetingSelectionTask_AOE::GetCollisionShapeFromTask() const
{
	switch (ShapeType)
	{
	case ETargetingAOEShape::Box:
		return FCollisionShape::MakeBox(HalfExtent);

	case ETargetingAOEShape::Cylinder:
		return FCollisionShape::MakeBox(HalfExtent);

	case ETargetingAOEShape::Sphere:
		return FCollisionShape::MakeSphere(Radius.GetValue());

	case ETargetingAOEShape::Capsule:
		return FCollisionShape::MakeCapsule(Radius.GetValue(), HalfHeight.GetValue());
	default:
		UE_LOG(LogTemp, Error, TEXT("Invalid AOE Shape Type"));
		return FCollisionShape();
	}
}

ETargetingAOEShape UGASC_TargetingSelectionTask_AOE::GetTargetingAOEShape() const
{
	return ShapeType;
}

FVector UGASC_TargetingSelectionTask_AOE::GetAoESourceOffset(const FTargetingRequestHandle& TargetingHandle) const
{
	return GetSourceOffset(TargetingHandle);
}

void UGASC_TargetingSelectionTask_AOE::AddCollisionObjectTypes(TEnumAsByte<EObjectTypeQuery> InObjectTypeToAdd)
{
	CollisionObjectTypes.Add(InObjectTypeToAdd);
}
