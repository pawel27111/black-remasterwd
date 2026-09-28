using UnrealBuildTool;

public class BlackRemastered : ModuleRules
{
    public BlackRemastered(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        
        PublicDependencyModuleNames.AddRange(new string[] {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "HeadMountedDisplay",
            "NavigationSystem",
            "AIModule",
            "GameplayTasks",
            "PhysicsCore",
            "Chaos",
            "Niagara",
            "MetaSoundEngine",
            "UMG",
            "Slate",
            "SlateCore"
        });
        
        PrivateDependencyModuleNames.AddRange(new string[] {
            "Core",
            "CoreUObject",
            "Engine",
            "Slate",
            "SlateCore",
            "RenderCore",
            "RHI",
            "RuntimeFloat16_Optimized"
        });
        
        // Chaos Physics for destruction
        PrivateDependencyModuleNames.Add("Chaos");
        PrivateDependencyModuleNames.Add("ChaosCore");
        PrivateDependencyModuleNames.Add("ChaosSolvers");
        
        // Nanite for high-poly assets
        PrivateDependencyModuleNames.Add("Nanite");
        
        // Lumen for dynamic lighting
        PrivateDependencyModuleNames.Add("Lumen");
        
        // Niagara for particles
        PrivateDependencyModuleNames.Add("Niagara");
        PrivateDependencyModuleNames.Add("NiagaraCore");
        PrivateDependencyModuleNames.Add("NiagaraShader");
        
        // MetaSounds for audio
        PrivateDependencyModuleNames.Add("MetaSoundEngine");
        
        // Optimization
        bUseUnity = false;
        bUsePCHFiles = true;
        bUseRTTI = true;
        bUseExceptionHandling = false;
        bEnableUndeterminedIdentifiers = true;
        
        // Include paths
        PublicIncludePaths.Add(Path.Combine(EngineDirectory, "Source/Runtime/Engine/Public"));
        PrivateIncludePaths.Add(Path.Combine(EngineDirectory, "Source/Runtime/Private"));
        PrivateIncludePaths.Add(Path.Combine(EngineDirectory, "Source/Runtime/Engine/Private"));
        PrivateIncludePaths.Add("Private");
        
        // Definitions
        PublicDefinitions.Add("BLACKREMASTERED_VERSION=1");
        PublicDefinitions.Add("UE_BUILD_DEBUG=0");
        PublicDefinitions.Add("UE_BUILD_DEVELOPMENT=1");
        
        // Linker settings
        bEnableBufferVisualizationEventTrace = false;
        bEnableDataDrivenShaderPermutationReduction = true;
        
        // PCH includes
        PublicPCHHeaderFile = "Public/BlackRemastered.h";
        PrivatePCHHeaderFile = "Private/BlackRemasteredPrivatePCH.h";
        
        // Additional compiler flags
        if (Target.bDebugBuilds == true)
        {
            PrivateDefinitions.Add("DEBUG=1");
        }
        else
        {
            PrivateDefinitions.Add("DEBUG=0");
        }
    }
}
