// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GASCourse : ModuleRules
{
	public GASCourse(ReadOnlyTargetRules Target) : base(Target)
	{
		SetupGameplayDebuggerSupport(Target);
		SetupIrisSupport(Target);
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PrivateDependencyModuleNames.AddRange(new string[] { "AIModule", "UMG", "Slate", "NetCore"});
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "TargetingSystem", "GameplayAbilities", 
			"GameplayTasks", "GameplayTags", "ModelViewViewModel", "CogCommon"});
		
		
		if(Target.Configuration != UnrealTargetConfiguration.Shipping)
		{
			PublicDependencyModuleNames.AddRange(new string[] 
			{
				"CogAbility",
                "CogAI",
                "CogAll",
                "CogDebug",
                "CogEngine",
                "CogImgui",
                "CogInput",
                "CogWindow",
			});
		}
		

		if (Target.Type == TargetType.Editor)
		{
			PublicDependencyModuleNames.AddRange(new string[]
			{
				"UnrealEd"
			});
		}
	}
}
