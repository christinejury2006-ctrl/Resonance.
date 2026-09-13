// Dragonbound — editor target.
// Standard Unreal editor build target. No customizations at Phase 0.

using UnrealBuildTool;
using System.Collections.Generic;

public class DragonboundEditorTarget : TargetRules
{
	public DragonboundEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.Latest;
		ExtraModuleNames.Add("Dragonbound");
	}
}
