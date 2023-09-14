// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class GASCourseTarget : TargetRules
{
	public GASCourseTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V4;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		ExtraModuleNames.Add("GASCourse");
		if (!bUseIris)
		{
			BuildEnvironment = TargetBuildEnvironment.Unique;
			bUseIris = true;
		}

		if (Type == TargetType.Editor)
		{
			ExtraModuleNames.AddRange( 
				new string[]
				{
					"GASCourseEditor"
				});
		}
	}
}
