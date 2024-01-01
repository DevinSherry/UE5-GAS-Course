// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class GASCourseClientTarget : TargetRules
{
	public GASCourseClientTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Client;
		
		ExtraModuleNames.AddRange(
			new string[]
			{
				"GASCourse"
			});
	}
}
