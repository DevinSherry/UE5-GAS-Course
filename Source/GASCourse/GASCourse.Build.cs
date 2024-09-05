// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GASCourse : ModuleRules
{
	public GASCourse(ReadOnlyTargetRules Target) : base(Target)
	{
		PrivateDependencyModuleNames.AddRange(new string[] { "AIModule", "UMG", "Slate", "NetCore" });
		if(Target.Type == TargetRules.TargetType.Editor)
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
				"UnrealEd",
			
			});
		}
		SetupGameplayDebuggerSupport(Target);
		SetupIrisSupport(Target);
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "GameplayAbilities", "GameplayTasks", "GameplayTags", "ModelViewViewModel", "CogCommon"});
	}
}
