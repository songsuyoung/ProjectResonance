// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ResonanceClient : ModuleRules
{
    public ResonanceClient(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PublicIncludePaths.AddRange(new string[] { "ResonanceClient" });

        PrivateDependencyModuleNames.AddRange(new string[] { 
            "Core"
        });

    }
}
