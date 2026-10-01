# Dragonbound — UE 5.8 M2 Handoff

This document is the single editor-side handoff for the M2 first playable companion pass.

## Verified source assets

The feature branch contains both supplied binary sources:

- `Content/External/Dragon/war_dragon_rigged.glb`
  - 2,150,728 bytes
  - SHA-256: `1179d43b10612b7f6ce0729db9498c981a33137425384195e67cb488fe54a242`
- `Content/External/Human/adventurer_rigged.glb`
  - 2,286,472 bytes
  - SHA-256: `71611f4dc025f4235c4ad981f451797fc3daafe7f9ded9ad123262caaceca33e`

Important: these two files were added through the GitHub web uploader and are currently ordinary repository blobs, not confirmed Git LFS pointer objects. Do not replace, re-encode, or modify them during the Unreal import pass.

## Editor order

Use UE 5.8 and work in this order:

1. Open `Dragonbound.uproject`.
2. Allow Unreal to generate/update project files and compiled binaries.
3. Import the dragon source into:
   `Content/Dragonbound/Characters/Dragon/Source/`
4. Validate the dragon skeleton and all seven supplied animations before creating runtime Blueprints.
5. Import the Rider source into:
   `Content/Dragonbound/Characters/Rider/Source/`
6. Validate the Rider skeleton and all four supplied animations.
7. Create `BP_Dragon_Juvenile` from `ADBDragonCharacter`.
8. Create `ABP_Dragon_Juvenile` on the imported dragon skeleton.
9. Create `BP_Rider` (or the project-standard Rider visual subclass) using the imported adventurer mesh while retaining `ADBRiderCharacter` as the gameplay class.
10. Create `ABP_Rider`.
11. Create `ST_DragonCompanion`.
12. Add the M2 test map and NavMesh.
13. Run the interaction sequence below.
14. Compile the project.
15. Run all `Dragonbound.M2.*` automation tests.
16. Only after the full-session exit test passes should M2 be considered complete.

## Dragon import acceptance

The imported dragon must show:

- 57 joints
- 4 source materials
- `Idle`, `Walk`, `Flap`, `Glide`, `Bite`, `Takeoff`, `Landing`
- working tail, neck/head/jaw, leg, wing and wing-finger deformation
- correct ground contact and scale beside the Rider

M2 actively uses:

- `Idle`
- `Walk`
- `Flap` only as presentation material for now

Keep `Glide`, `Takeoff`, and `Landing` reserved for flight. Keep `Bite` reserved for M3 combat.

## Rider import acceptance

The imported Rider must show:

- 28 joints
- 6 source materials
- `Idle`, `Walk`, `Run`, `SwordSwing`
- correct feet, hands, head and spine deformation
- scale compatible with the existing capsule/camera

M2 actively uses:

- `Idle`
- `Walk`
- `Run`

Keep `SwordSwing` reserved for M3.

## Dragon Blueprint

Create:

`Content/Dragonbound/Characters/Dragon/BP_Dragon_Juvenile`

Parent: `ADBDragonCharacter`

Set:

- Growth Stage = Juvenile
- Visual State = Calm
- imported dragon Skeletal Mesh
- Animation Mode = Use Animation Blueprint
- Anim Class = `ABP_Dragon_Juvenile`

Do not remove the C++ Emotion, Bond, Mind Link, Interaction or Visual components.

## Dragon Animation Blueprint

Create:

`Content/Dragonbound/Animation/Dragon/ABP_Dragon_Juvenile`

Variables:

- Speed
- Direction
- IsInAir
- VisualState
- GrowthStage

Required first pass:

- Idle
- Walk
- Alert
- Comforted
- Distressed
- Protective

The supplied dragon has no dedicated M2 reaction clips yet. Use temporary presentation poses/blends if necessary, then replace them with the authored reaction clips listed in `Docs/DRAGON_ANIMATION_CONTRACT.md`.

## StateTree

Create:

`Content/Dragonbound/AI/ST_DragonCompanion`

High-level states:

- Idle
- Curious
- Follow
- Protect
- React

Do not move bond calculations, interaction effects or mind-link payload generation into the StateTree.

## Rider visual setup

Create the Rider visual Blueprint under:

`Content/Dragonbound/Characters/Rider/`

Use the imported adventurer mesh.

Create:

`Content/Dragonbound/Animation/Rider/ABP_Rider`

Variables:

- Speed
- Direction
- IsInAir
- LocomotionContext

Connect:

- OnFoot → Idle/Walk/Run
- Mounted → reserved mounted presentation context
- Flying → reserved flying presentation context

The existing C++ Rider remains responsible for movement, camera, input, interaction and control routing.

## Controls

The existing control architecture is device-agnostic:

Touch, keyboard and gamepad feed the shared Enhanced Input layer; gameplay commands are routed through `UDBControlRouterComponent`.

Do not create a separate mobile-only dragon control path.

The current touch Interact action already reaches the Rider's dragon Call interaction.

## M2 playable sequence

Run this exact sequence:

1. Look at dragon → `Call to the dragon`.
2. Interact → dragon becomes Curious/Alert and approaches.
3. Feed → Comforted + Warmth reaches Rider.
4. Soothe → Comforted + Safety reaches Rider.
5. Protect → Protective.
6. Walk away → dragon follows when trust permits.
7. Leave/return during the same session → memory remains.
8. Repeat memory actions → Awakening can advance to Feeling.
9. Confirm visual-state changes drive animation/presentation.

## M2 exit condition

M2 passes only when the dragon reads as a persistent companion for a full session:

- perception reacts;
- Call changes behavior;
- locomotion is grounded and continuous;
- emotions visibly change presentation;
- bond memory changes;
- mind-link payloads reach the Rider;
- Follow/Protect behavior is observable;
- no UI text is required to explain every reaction.

## Out of scope

Do not activate:

- flight gameplay
- dragon combat
- Ember
- adult growth
- full dialogue
- Academy
- complex quests
- GAS combat
- enemies
- open-world production
