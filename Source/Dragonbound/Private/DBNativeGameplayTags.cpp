// Dragonbound — native gameplay tag registration.

#include "DBNativeGameplayTags.h"

namespace DBGameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Locomotion_OnFoot, "DB.Locomotion.OnFoot", "Rider is moving on foot.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Locomotion_Mounted, "DB.Locomotion.Mounted", "Rider is mounted on the dragon.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Locomotion_Flying, "DB.Locomotion.Flying", "Rider is flying with the dragon.");
}
