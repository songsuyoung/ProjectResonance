// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Resonance : ModuleRules
{
	public Resonance(ReadOnlyTargetRules Target) : base(Target)
	{
		PrivateDependencyModuleNames.AddRange(new string[] { "ResonanceCore" });
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(new string[] { "Resonance", "ResonanceCore" });

        PublicDependencyModuleNames.AddRange(new string[] { 
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"GameplayTags",
			"ResonanceClient",
			"Slate",
			"SlateCore",
			"UMG",
			"JsonUtilities",
			"Json"
		});
	}
}
