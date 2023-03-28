// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MyProject : ModuleRules
{
	public MyProject(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject","MetaSoundTool","MetasoundEngine", "Engine", "InputCore", "HeadMountedDisplay"});
		PrivateDependencyModuleNames.AddRange(new string[]{"MetaSoundTool","MetasoundEngine"});
	}
}
