// Dragonbound — primary game module build rules.
//
// Dependency policy (see SYSTEMS.md):
//   - Add engine modules here only when a real system needs them.
//   - Keep the module lean at Phase 0; GAS, StateTree, Niagra, etc. are added
//     when their systems land in their milestone.
//   - Future feature plugins (dragon AI, flight, powers) get their own
//     modules rather than fattening this one.

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
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
		});
	}
}
