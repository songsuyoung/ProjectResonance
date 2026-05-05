using UnrealBuildTool;

public class ResonanceCore : ModuleRules
{
    public ResonanceCore(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(new string[] { "ResonanceCore" });
        
        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "Json",
                "JsonUtilities"
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore"
            }
        );
    }
}