// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GASCourse : ModuleRules
{
	public GASCourse(ReadOnlyTargetRules Target) : base(Target)
	{
		SetupIrisSupport(Target);
		
		PrivateDependencyModuleNames.AddRange(new string[] { "AnimationLocomotionLibraryRuntime"});
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				"GASCourse"
			}
		);

		PrivateIncludePaths.AddRange(
			new string[] {
			}
		);

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput",
			"GameplayAbilities", "GameplayTags", "GameplayTasks", "NetCore", "AIModule", "Slate", "SlateCore", "UMG",
		});
	}
}
