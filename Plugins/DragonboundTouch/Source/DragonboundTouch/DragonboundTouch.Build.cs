// DragonboundTouch — touch-first input plugin.
// See Docs/decisions/0003-touch-first-input.md and Docs/EDITOR_SETUP.md §9.

using UnrealBuildTool;

public class DragonboundTouch : ModuleRules
{
	public DragonboundTouch(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"UMG",
			"Slate",
			"SlateCore",
			"CommonInput",
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
		});
	}
}
