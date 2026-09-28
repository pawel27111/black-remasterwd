using UnrealBuildTool;
using System.Collections.Generic;

public class BlackRemasteredEditorTarget : TargetRules
{
    public BlackRemasteredEditorTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.V5;
        ExtraModuleNames.Add("BlackPrototype");
    }
}
