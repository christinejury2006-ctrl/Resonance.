# Dragonbound — War Dragon GLB Import

## Authoritative source asset

The exact supplied M2 dragon source is:

`war_dragon_rigged.glb`

Intended source path:

`Content/External/Dragon/war_dragon_rigged.glb`

SHA-256 of the supplied file:
`1179d43b10612b7f6ce0729db9498c981a33137425384195e67cb488fe54a242`

Keep this source immutable. Do not re-rig, rename, edit, or overwrite it. Re-import the source if a future replacement is intentionally approved.

## Verified source contents

Inspection of the supplied GLB found:

- 1 skinned mesh
- 1 skeleton
- 57 joints
- 4 materials
- 7 animation clips
- Animations: `Flap`, `Glide`, `Idle`, `Walk`, `Bite`, `Takeoff`, `Landing`
- Rig coverage: tail, neck/head/jaw, four legs, both wings, and wing-finger chains

This file supersedes the earlier M2 import note that listed only `Flap` as an existing animation.

## UE 5.8 import

Import the GLB into:

`Content/Dragonbound/Characters/Dragon/Source/`

Recommended first-import settings:

- Import Skeletal Mesh: enabled
- Import Skeleton: enabled
- Import Animations: enabled
- Import Morph Targets: only if present
- Preserve source materials initially
- Do not enable destructive optimization on the first import

Validate:

1. Skeleton opens with all expected 57 joints.
2. Dragon faces the expected forward axis.
3. Feet contact the intended ground plane.
4. `Idle`, `Walk`, `Flap`, `Glide`, `Bite`, `Takeoff`, and `Landing` play without import corruption.
5. Wings, wing fingers, tail, neck, jaw, and legs deform correctly.
6. Scale is appropriate beside the Rider.
7. No unexpected root motion or axis conversion is introduced.

## Runtime asset

Create:

`Content/Dragonbound/Characters/Dragon/BP_Dragon_Juvenile`

Parent:

`ADBDragonCharacter`

Set:

- Growth Stage = Juvenile
- Visual State = Calm
- Animation Mode = Use Animation Blueprint
- Animation Class = `ABP_Dragon_Juvenile`

Keep gameplay state in the existing C++ components. Blueprint owns presentation and editor tuning.

## Animation mapping

Use the supplied clips directly where they fit:

| Supplied clip | Initial Dragonbound use |
| --- | --- |
| `Idle` | Grounded idle |
| `Walk` | Follow locomotion |
| `Flap` | Wing motion / later flight presentation |
| `Glide` | Reserved for later flight |
| `Bite` | Reserved for M3 combat |
| `Takeoff` | Reserved for later flight |
| `Landing` | Reserved for later flight |

Do not force `Bite`, `Takeoff`, or `Landing` into M2 gameplay just because the clips exist. Their presence does not expand the M2 scope.

## Source versus runtime

`Content/External/Dragon/war_dragon_rigged.glb` is the immutable source.

`Content/Dragonbound/Characters/Dragon/` contains Unreal-authored runtime assets.

The current GitHub connection can write repository text and Git objects but cannot directly upload the binary GLB from this chat session. The exact binary therefore remains a local import/drop asset until it is added through Git/LFS.

