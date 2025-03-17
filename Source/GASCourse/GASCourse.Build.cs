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
        
		PrivateDependencyModuleNames.AddRange(new[] { "AIModule", "UMG", "Slate", "NetCore", "MotionWarping" });
		PublicDependencyModuleNames.AddRange(new[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "TargetingSystem", "GameplayAbilities", 
			"GameplayTasks", "GameplayTags", "ModelViewViewModel"});
        
		if (Target.Type == TargetType.Editor)
		{
			PublicDependencyModuleNames.AddRange(new[] 
			{
				"UnrealEd"
			});
		}
	}
}