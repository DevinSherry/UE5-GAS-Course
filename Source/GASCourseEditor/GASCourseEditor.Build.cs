// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GASCourseEditor : ModuleRules
{
	public GASCourseEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[]
			{
				"GASCourseEditor/Public"
			});

		PrivateIncludePaths.AddRange(
			new string[] 
			{
				"GASCourseEditor/Private"
			});

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core", 
				"CoreUObject",
				"Engine",
				"EditorFramework",
				"EditorFramework",
				"GASCourse",
				"UnrealEd",
			});
		PrivateDependencyModuleNames.AddRange(
			new string[] 
			{
			});

		PrivateIncludePathModuleNames.AddRange(
			new string[]
			{
			});

		DynamicallyLoadedModuleNames.AddRange(
			new string[] 
			{
			});

	}
}
