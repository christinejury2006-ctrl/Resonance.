// Dragonbound — native gameplay tags.
//
// The global tag vocabulary (Docs/SYSTEMS.md §0 principle 4). Systems query
// these tags; adding a tag here is an explicit, reviewed vocabulary change.

#pragma once

#include "NativeGameplayTags.h"

namespace DBGameplayTags
{
	// --- Locomotion contexts (camera modes, input contexts, AI route on these) ---

	/** Rider is moving on foot (M1; the only active context until M6). */
	DRAGONBOUND_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Locomotion_OnFoot);

	/** Rider is mounted on the dragon (M6+; reserved — no system consumes it yet). */
	DRAGONBOUND_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Locomotion_Mounted);

	/** Rider is flying with the dragon (M6+; reserved). */
	DRAGONBOUND_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Locomotion_Flying);
}
