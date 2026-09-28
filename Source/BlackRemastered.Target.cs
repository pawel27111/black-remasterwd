using UnrealBuildTool;
using System.Collections.Generic;

public class BlackRemasteredTarget : TargetRules
{
    public BlackRemasteredTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.V5;
        ExtraModuleNames.Add("BlackPrototype");
    }
}
