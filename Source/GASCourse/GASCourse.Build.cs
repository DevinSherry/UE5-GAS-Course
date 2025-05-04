// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

// GASCourse.Build.cs
public class GASCourse : ModuleRules
{
	public GASCourse(ReadOnlyTargetRules Target) : base(Target)
	{
		SetupGameplayDebuggerSupport(Target);
		SetupIrisSupport(Target);
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        
		PrivateDependencyModuleNames.AddRange(new[] { "AIModule", "UMG", "Slate", "NetCore", "MotionWarping", "StateTreeModule", "GameplayStateTreeModule" });
		PublicDependencyModuleNames.AddRange(new[] { "Core", "CoreUObject", "Engine", "InputCore", "PhysicsCore","EnhancedInput", "TargetingSystem", "GameplayAbilities", 
			"GameplayTasks", "GameplayTags", "ModelViewViewModel", "Niagara", "StateTreeModule"
		});
        
		if (Target.Type == TargetType.Editor)
		{
			PublicDependencyModuleNames.AddRange(new[] 
			{
				"UnrealEd"
			});
		}
	}
}