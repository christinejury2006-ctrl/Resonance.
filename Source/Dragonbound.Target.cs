// Dragonbound — game target.
// Standard Unreal build target. No customizations at Phase 0.

using UnrealBuildTool;
using System.Collections.Generic;

public class DragonboundTarget : TargetRules
{
	public DragonboundTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.Latest;
		ExtraModuleNames.Add("Dragonbound");
	}
}
