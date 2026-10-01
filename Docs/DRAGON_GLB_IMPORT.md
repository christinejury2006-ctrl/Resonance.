# Dragonbound — War Dragon GLB Import

## Source asset

The M2 juvenile companion source asset is:

`war_dragon_rigged.glb`

The exact uploaded source must be placed at:

`Content/External/Dragon/war_dragon_rigged.glb`

This is intentionally kept under `Content/External/` as an immutable source asset. Do not edit, re-rig, rename, or overwrite the source file.

## Known source characteristics

- Format: GLB
- Skinned mesh: 1
- Skeleton: 57 joints
- Materials: 3
- Existing animation: `Flap`
- Rig coverage includes body, neck/head/jaw, tail, four legs, both wings and wing-finger chains.

## UE 5.8 import

Import the GLB into:

`Content/Dragonbound/Characters/Dragon/Source/`

Recommended import policy:

- Import Skeletal Mesh: enabled
- Import Skeleton: enabled for the first import
- Import Animations: enabled
- Import Morph Targets: only if the source actually contains them
- Material import: enabled initially so the source appearance is preserved for inspection
- Do not enable destructive mesh optimization on the first import

After import, verify:

1. The skeleton opens without broken bones.
2. The dragon faces the expected forward axis.
3. Feet contact the ground at the intended root height.
4. The existing Flap animation plays correctly.
5. Wing membranes deform correctly.
6. Tail, neck, jaw and wing fingers deform without obvious collapse.
7. The mesh scale is appropriate for the Rider capsule and camera.

## Dragonbound runtime asset

Create:

`Content/Dragonbound/Characters/Dragon/BP_Dragon_Juvenile`

Parent:

`ADBDragonCharacter`

Assign the imported skeletal mesh to the visible mesh component.

Set:

- Growth Stage = Juvenile
- Visual State = Calm
- Animation Class = `ABP_Dragon_Juvenile`

Keep gameplay behavior in the existing C++ components. The Blueprint should primarily bind the imported visual asset and expose tuning values.

## Animation Blueprint

Create:

`Content/Dragonbound/Animation/Dragon/ABP_Dragon_Juvenile`

Use the imported skeleton.

Required variables:

- Speed
- Direction
- IsInAir
- VisualState
- GrowthStage

Minimum states:

- Idle
- Walk
- Run
- Turn
- Rest
- Alert
- Comforted
- Distressed
- Protective

Use the existing `Flap` animation as the first wing-motion source. Do not invent a fake locomotion set in C++; animation assets remain editor-authored.

## Source-versus-runtime rule

`Content/External/Dragon/war_dragon_rigged.glb` is the source.

`Content/Dragonbound/Characters/Dragon/` contains Dragonbound-authored runtime assets.

If the source is replaced later, re-import it rather than editing generated Unreal assets by hand.

## Important repository note

The current ChatGPT GitHub connection can create/update UTF-8 repository files but cannot upload this binary GLB directly into the repository. Therefore this document prepares the exact destination and import contract without pretending that the binary has already been committed.

Once the GLB is uploaded through Git/LFS, the rest of the Blueprint setup can use the paths above unchanged.
