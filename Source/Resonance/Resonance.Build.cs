// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Resonance : ModuleRules
{
	public Resonance(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(new string[] { "Resonance" });

        PublicDependencyModuleNames.AddRange(new string[] { 
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"ResonanceClient"
		});
	}
}
