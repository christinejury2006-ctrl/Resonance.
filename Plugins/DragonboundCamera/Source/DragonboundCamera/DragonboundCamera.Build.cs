// DragonboundCamera — camera mode stack plugin.
// See Docs/SYSTEMS.md §3 (Camera) and Docs/decisions/0002-camera-and-input.md.

using UnrealBuildTool;

public class DragonboundCamera : ModuleRules
{
	public DragonboundCamera(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"GameplayTags",
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
		});
	}
}
