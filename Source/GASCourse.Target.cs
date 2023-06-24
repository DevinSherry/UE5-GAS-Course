// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class GASCourseTarget : TargetRules
{
	public GASCourseTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_2;
		ExtraModuleNames.Add("GASCourse");
		
		/*
		if (!bUseIris)
		{
			// If we enable Iris for a single target we also need to set the TargetBuildEnvironment to unique, as other projects in the solution might want it compiled out
			BuildEnvironment = TargetBuildEnvironment.Unique;
			bUseIris = true;
		}
		*/
	}
}
