// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class GASCourseEditorTarget : TargetRules
{
	public GASCourseEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		
		ExtraModuleNames.AddRange(
			new string[]
			{
				"GASCourseEditor"
			});
	}
}
