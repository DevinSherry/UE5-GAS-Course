// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Systems/Targeting/AreaofEffect/GASC_TargetFilter_ActorClass.h"


UGASC_TargetFilter_ActorClass::UGASC_TargetFilter_ActorClass(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

bool UGASC_TargetFilter_ActorClass::ShouldFilterTarget(const FTargetingRequestHandle& TargetingHandle,
	const FTargetingDefaultResultData& TargetData) const
{
	if (AActor* TargetActor = TargetData.HitResult.GetActor())
	{
		// if the target is one of these classes, filter it out
		for (UClass* ClassFilter : IgnoredActorClassFilters)
		{
			if (TargetActor->IsA(ClassFilter))
			{
				return true;
			}
		}

		// if the target is one of these classes, do not filter it out
		for (UClass* ClassFilter : RequiredActorClassFilters)
		{
			if (TargetActor->IsA(ClassFilter))
			{
				return false;
			}
		}

		// if we do not have required class filters, we do NOT want to filter this target
		return (RequiredActorClassFilters.Num() > 0);
	}

	return true;
}


void UGASC_TargetFilter_ActorClass::AddRequiredActorClassFilter(const TObjectPtr<UClass>& ActorClassToAdd)
{
	//warning C4996: 'TObjectPtr<T>::TObjectPtr': Constructing a TObjectPtr from a reference is deprecated - use a pointer instead. - Please update your code to the new API before upgrading to the next release, otherwise your project will no longer compile.
	RequiredActorClassFilters.Add(ActorClassToAdd);
}

void UGASC_TargetFilter_ActorClass::AddIgnoredActorClassFilter(const TObjectPtr<UClass>& ActorClassIgnored)
{
	//warning C4996: 'TObjectPtr<T>::TObjectPtr': Constructing a TObjectPtr from a reference is deprecated - use a pointer instead. - Please update your code to the new API before upgrading to the next release, otherwise your project will no longer compile.
	IgnoredActorClassFilters.Add(ActorClassIgnored);
}