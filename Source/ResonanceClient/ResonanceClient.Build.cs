using UnrealBuildTool;

public class ResonanceClient : ModuleRules
{
    public ResonanceClient(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PublicIncludePaths.AddRange(new string[] { "ResonanceClient", "ResonanceCore" });

        PrivateDependencyModuleNames.AddRange(new string[] {
            "Core",
            "CoreUObject",
            "Engine",
            "UMG",
            "Slate",
            "SlateCore", 
            "ResonanceCore",
        });
    }
}
