// Dragonbound — primary game module build rules.
//
// Dependency policy (see Docs/SYSTEMS.md §0):
//   - Add engine modules here only when a real system needs them.
//   - Feature domains get their own modules (e.g., the DragonboundCamera
//     plugin for the camera mode stack) rather than fattening this one.
//   - GAS lands in M3; dragon AI dependencies land in M2 when AI code lands.

using UnrealBuildTool;

public class Dragonbound : ModuleRules
{
	public Dragonbound(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"GameplayTags",
			"AIModule",
			"NavigationSystem",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"DragonboundCamera",
			"DragonboundTouch",
			"UMG",
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
		});
	}
}