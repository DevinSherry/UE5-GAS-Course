// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Systems/Subsystems/MeleeTrace/Shapes/GASC_MeleeShape_Base.h"
#include "Game/Systems/Subsystems/MeleeTrace/Settings/GASC_MeleeSubsystem_Settings.h" 

FCollisionShape UGASC_MeleeShape_Base::CreateCollisionShape() const
{
	unimplemented();
	return FCollisionShape();
}

FQuat UGASC_MeleeShape_Base::GetRotationOffset() const
{
	return FQuat::Identity;
}

FCollisionShape UGASC_MeleeShape_Sphere::CreateCollisionShape() const
{
	return FCollisionShape::MakeSphere(Radius);
}

UGASC_MeleeShape_Sphere* UGASC_MeleeShape_Sphere::MakeSphereShape(float Radius)
{
	auto* NewShape = NewObject<UGASC_MeleeShape_Sphere>();
	NewShape->Radius = Radius;
	return NewShape;
}

FCollisionShape UGASC_MeleeShape_Capsule::CreateCollisionShape() const
{
	return FCollisionShape::MakeCapsule(Radius, HalfHeight);
}

UGASC_MeleeShape_Capsule* UGASC_MeleeShape_Capsule::MakeCapsuleShape(float HalfHeight, float Radius, const FRotator& RotationOffset)
{
	auto* NewShape = NewObject<UGASC_MeleeShape_Capsule>();
	NewShape->HalfHeight = HalfHeight;
	NewShape->Radius = Radius;
	NewShape->RotationOffset = RotationOffset;
	return NewShape;
}

FQuat UGASC_MeleeShape_Capsule::GetRotationOffset() const
{
	return RotationOffset.Quaternion();
}

FCollisionShape UGASC_MeleeShape_Box::CreateCollisionShape() const
{
	return FCollisionShape::MakeBox(BoxExtent);
}

UGASC_MeleeShape_Box* UGASC_MeleeShape_Box::MakeBoxShape(FVector BoxExtent, const FRotator& RotationOffset)
{
	auto* NewShape = NewObject<UGASC_MeleeShape_Box>();
	NewShape->BoxExtent = BoxExtent;
	NewShape->RotationOffset = RotationOffset;
	return NewShape;
}

FQuat UGASC_MeleeShape_Box::GetRotationOffset() const
{
	return RotationOffset.Quaternion();
}
