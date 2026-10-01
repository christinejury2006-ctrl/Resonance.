# Dragonbound — Juvenile Animation Contract

## Purpose

The supplied `war_dragon_rigged.glb` is the exact M2 juvenile source mesh/skeleton.
Animation assets are authored separately and assigned to `ABP_Dragon_Juvenile`.

Do not add animation logic to gameplay C++. The Animation Blueprint consumes the
dragon's movement and visual-state information.

## Required animation set

Create these animations using the imported dragon skeleton:

| Asset | Required | Use |
| --- | --- | --- |
| `A_Dragon_Juvenile_Idle` | Yes | Default grounded idle |
| `A_Dragon_Juvenile_Walk` | Yes | Low-speed follow |
| `A_Dragon_Juvenile_Run` | Yes | Faster follow / reposition |
| `A_Dragon_Juvenile_Turn` | Recommended | Direction-change presentation |
| `A_Dragon_Juvenile_Rest` | Recommended | Companion downtime |
| `A_Dragon_Juvenile_Alert` | Yes | Curious/alert response |
| `A_Dragon_Juvenile_Comforted` | Yes | Warm/relaxed bond response |
| `A_Dragon_Juvenile_Distressed` | Yes | Fear/distress response |
| `A_Dragon_Juvenile_Protective` | Yes | Protective response |
| `A_Dragon_Juvenile_Flap` | Existing | Supplied GLB animation; preserve as source |

If an animation is not ready yet, leave that slot unassigned rather than
inventing a fake gameplay animation.

## Animation Blueprint

Create:

`Content/Dragonbound/Animation/Dragon/ABP_Dragon_Juvenile`

Skeleton:
the skeleton imported from `war_dragon_rigged.glb`.

### Event Graph variables

- `Speed` — current horizontal movement speed.
- `Direction` — signed movement direction relative to actor facing.
- `IsInAir` — whether the dragon is airborne.
- `VisualState` — numeric representation of `EDBDragonVisualState`.
- `GrowthStage` — numeric representation of `EDBDragonGrowthStage`.

The Blueprint should read these from the owning `ADBDragonCharacter` and its
movement/visual components.

### Locomotion

Use a grounded locomotion state/blend setup:

- Idle at zero speed.
- Walk at low speed.
- Run at higher speed.
- Turn when a dedicated turn animation is appropriate.
- Rest only when the companion is intentionally resting.

The exact blend thresholds should be tuned against the actual imported mesh and
animation speeds in UE 5.8 rather than hard-coded in C++.

### Reaction states

Visual state maps to presentation:

- Calm → Idle / normal locomotion
- Alert → Alert
- Comforted → Comforted
- Distressed → Distressed
- Protective → Protective

Reactions must not permanently interrupt locomotion. Use short state-specific
animations or blends, then return to the current locomotion state.

## Blueprint

Create:

`Content/Dragonbound/Characters/Dragon/BP_Dragon_Juvenile`

Parent class:

`ADBDragonCharacter`

Set:

- Skeletal Mesh → imported juvenile dragon mesh
- Animation Mode → Use Animation Blueprint
- Anim Class → `ABP_Dragon_Juvenile`
- Growth Stage → Juvenile
- Initial Visual State → Calm

Keep the existing C++ components intact:

- Dragon Emotion
- Bond
- Mind Link
- Interaction
- Visual

Do not duplicate their behavior in Blueprint.

## Collision / scale validation

Before tuning movement:

1. Place the dragon beside the Rider.
2. Confirm its feet sit correctly on the floor.
3. Confirm the capsule covers the torso without being excessively large.
4. Confirm the head/wings/tail do not create unusable collision.
5. Confirm the dragon can turn without colliding with itself or the Rider.
6. Tune mesh scale before tuning animation blend thresholds.

## M2 acceptance

Once the animation set exists, the first playable test should show:

Call → Alert/Curious reaction → approach → locomotion → Feed/Soothe →
Comforted → Protect → Protective → Follow.

The dragon should remain a single persistent companion; animation is presentation
of the runtime state, not the owner of that state.
