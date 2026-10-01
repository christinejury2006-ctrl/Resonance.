# Dragonbound — Juvenile Animation Contract

## Supplied source clips

The current `war_dragon_rigged.glb` contains:

- `Idle`
- `Walk`
- `Flap`
- `Glide`
- `Bite`
- `Takeoff`
- `Landing`

These clips are now the starting animation library. Do not recreate an equivalent Idle/Walk/Flap clip unless the imported versions fail validation or are deliberately replaced.

## Additional authored reactions

The M2 presentation layer still needs authored reaction coverage for:

- `A_Dragon_Juvenile_Alert`
- `A_Dragon_Juvenile_Comforted`
- `A_Dragon_Juvenile_Distressed`
- `A_Dragon_Juvenile_Protective`

Recommended later authored locomotion/presentation clips, only if the supplied motion is insufficient:

- Turn
- Rest
- Run

## Animation Blueprint

Create:

`Content/Dragonbound/Animation/Dragon/ABP_Dragon_Juvenile`

Use the skeleton imported from `war_dragon_rigged.glb`.

Variables:

- Speed
- Direction
- IsInAir
- VisualState
- GrowthStage

Initial grounded locomotion:

- Idle → supplied `Idle`
- Walk → supplied `Walk`
- Run → authored/selected later
- Turn → authored/selected later
- Rest → authored/selected later

Reaction mapping:

- Calm → normal locomotion
- Alert → Alert
- Comforted → Comforted
- Distressed → Distressed
- Protective → Protective

Reactions must return to the active locomotion state and must not become the owner of gameplay state.

## Reserved clips

The supplied `Flap`, `Glide`, `Takeoff`, and `Landing` clips are valuable future flight material. `Bite` is reserved for M3 combat. Keep them imported and available without activating those systems early.

## Blueprint

Create:

`Content/Dragonbound/Characters/Dragon/BP_Dragon_Juvenile`

Parent:

`ADBDragonCharacter`

Set:

- Skeletal Mesh → imported dragon mesh
- Animation Mode → Use Animation Blueprint
- Anim Class → `ABP_Dragon_Juvenile`
- Growth Stage → Juvenile
- Initial Visual State → Calm

Keep Dragon Emotion, Bond, Mind Link, Interaction, and Visual components from C++ intact.

## M2 acceptance

Call → Curious/Alert → approach → Walk → Feed/Soothe → Comforted → Protect → Protective → Follow.

The dragon should read as a persistent companion for the full session.