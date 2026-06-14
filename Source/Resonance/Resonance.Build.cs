// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Resonance : ModuleRules
{
	public Resonance(ReadOnlyTargetRules Target) : base(Target)
	{
		PrivateDependencyModuleNames.AddRange(new string[] {  });
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(new string[] { "Resonance" });

        PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"GameplayTags",
			"ResonanceCore",
			"ResonanceClient",
			"Slate",
			"SlateCore",
			"UMG",
			"JsonUtilities",
			"Json",
			"GameplayStateTreeModule",
			"AIModule",
			"NavigationSystem",
		});
	}
}
