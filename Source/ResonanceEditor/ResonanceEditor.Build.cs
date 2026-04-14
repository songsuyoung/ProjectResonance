using UnrealBuildTool;

public class ResonanceEditor : ModuleRules
{
    public ResonanceEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PublicIncludePaths.AddRange(new string[] { "ResonanceEditor" });

        PrivateDependencyModuleNames.AddRange(new string[] {
            "Core",
            "CoreUObject",
            "Engine",
            "LevelEditor",
            "Slate",
            "SlateCore",
        });
    }
}
