// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class WS4 : ModuleRules
{
	public WS4(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "AIModule" ,"Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
