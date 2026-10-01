# Dragonbound — Adventurer GLB Import

## Authoritative source asset

The supplied human character source is:

`adventurer_rigged.glb`

Intended source path:

`Content/External/Human/adventurer_rigged.glb`

SHA-256 of the supplied file:
`71611f4dc025f4235c4ad981f451797fc3daafe7f9ded9ad123262caaceca33e`

Keep the source immutable and use it as the Rider visual source.

## Verified source contents

Inspection found:

- 1 skinned mesh
- 1 skeleton
- 28 joints
- 6 materials
- 4 animation clips
- Animations: `Idle`, `Walk`, `Run`, `SwordSwing`

Skeleton coverage includes pelvis/spine/neck/head, both arms/hands, and both legs/feet.

## UE 5.8 import

Import the GLB into:

`Content/Dragonbound/Characters/Rider/Source/`

Recommended first-import settings:

- Import Skeletal Mesh: enabled
- Import Skeleton: enabled
- Import Animations: enabled
- Preserve source materials initially
- Do not enable destructive optimization on the first import

Validate:

1. All 28 joints are present.
2. Character stands on the intended ground plane.
3. `Idle`, `Walk`, `Run`, and `SwordSwing` import and play correctly.
4. Feet, hands, head, and spine deform correctly.
5. Scale fits the existing Rider capsule and camera.
6. The character does not acquire gameplay behavior from the imported animation asset.

## Runtime Rider setup

The existing `ADBRiderCharacter` remains the gameplay owner.

Create the visual Blueprint under:

`Content/Dragonbound/Characters/Rider/`

Use the imported adventurer skeleton and mesh for the Rider's visible body. Keep the existing movement, camera, input, interaction, control-router, appearance, and mind-link components in C++.

Initial Animation Blueprint:

`Content/Dragonbound/Animation/Rider/ABP_Rider`

Initial variables:

- Speed
- Direction
- IsInAir
- LocomotionContext

Initial locomotion:

- Idle → supplied `Idle`
- Walk → supplied `Walk`
- Run → supplied `Run`

Keep `SwordSwing` imported but unused until M3 combat is implemented.

## Source versus runtime

`Content/External/Human/adventurer_rigged.glb` is the immutable source.

`Content/Dragonbound/Characters/Rider/` and `Content/Dragonbound/Animation/Rider/` contain runtime/editor-authored assets.

The current GitHub connection cannot directly upload this binary GLB from the chat session; add the exact source through Git/LFS before opening the final UE project on another machine.
