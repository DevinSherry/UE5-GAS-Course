// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class GASCourseServerTarget : TargetRules
{
	public GASCourseServerTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Server;
		
		ExtraModuleNames.AddRange(
			new string[]
			{
				"GASCourse"
			});
	}
}
