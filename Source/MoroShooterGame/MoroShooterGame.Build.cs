// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MoroShooterGame : ModuleRules
{
	public MoroShooterGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
