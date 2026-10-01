# Dragonbound — M2 Phone-Only Preparation

> Purpose: prepare everything that does not require the Unreal Engine 5.8 Editor while the project is being handled from a phone.

## Current asset contracts

### Dragon
- Source file: `war_dragon_rigged.glb`
- Skeleton: 57 joints
- Materials: 4
- Supplied animations: Idle, Walk, Flap, Glide, Bite, Takeoff, Landing
- Planned Unreal destination: `Content/External/Dragon/`

### Rider / human
- Source file: `adventurer_rigged.glb`
- Skeleton: 28 joints
- Materials: 6
- Supplied animations: Idle, Walk, Run, SwordSwing
- Planned Unreal destination: `Content/External/Human/`

## Editor handoff order

1. Clone/download the `feature/m2-dragon-companion` branch on a computer.
2. Ensure Git LFS is installed and the GLB sources are present.
3. Open `Dragonbound.uproject` in Unreal Engine 5.8.
4. Import and validate the dragon source before creating any Blueprints.
5. Import and validate the rider source.
6. Create `BP_Dragon_Juvenile` and `ABP_Dragon_Juvenile`.
7. Create the rider visual Blueprint and `ABP_Rider`.
8. Build the first playable scene only after both asset imports are clean.
9. Compile and run the M2 automation tests in-editor.

## Do not do yet

- Do not create duplicate placeholder meshes.
- Do not rename supplied animation clips.
- Do not manually retarget until the imported skeletons have been inspected.
- Do not activate combat animation logic during the companion import pass.
- Do not commit generated Unreal `Saved/`, `Intermediate/`, or `DerivedDataCache/` content.

## Definition of ready for the computer

The repository already contains the UE project foundation, M2 runtime systems, Git LFS rules, import contracts, and editor handoff documentation. The remaining source-asset step is to place the two exact GLB binaries in their LFS-tracked folders, then perform the actual UE 5.8 import/validation.


## UE 5.8 import validation checklist

The first editor session should be treated as an asset-validation session, not a gameplay-building session.

### Dragon import
- Source: `war_dragon_rigged.glb`
- Import as a **Skeletal Mesh**.
- Import the embedded animations.
- Let Unreal create the first Skeleton asset; do not force the rider skeleton onto the dragon.
- Confirm the imported skeleton has the expected 57 joints.
- Confirm 4 material slots are present.
- Confirm the 7 expected animation sequences are present.
- Create/verify a Physics Asset only after the mesh imports cleanly.
- Play Idle, Walk, Flap, Glide, Bite, Takeoff and Landing individually before creating the Animation Blueprint.

### Rider import
- Source: `adventurer_rigged.glb`
- Import as a **Skeletal Mesh**.
- Import the embedded animations.
- Create a separate rider Skeleton.
- Confirm the expected 28-joint hierarchy.
- Confirm 6 material slots.
- Confirm Idle, Walk, Run and SwordSwing.
- Do not retarget the rider or dragon skeletons to each other unless an actual later requirement calls for it.

### Why this order matters

Unreal's skeletal workflow treats the Skeletal Mesh, Skeleton and Animation Sequences as linked assets, and Animation Blueprints are created against a specific Skeleton. citeturn0search2turn0search5

UE 5.8's Interchange import pipeline explicitly supports skeletal meshes and animations, including GLB/glTF import, so the source files should be validated first rather than manually reconstructed. citeturn0search3turn0search10

### Stop conditions

Stop the editor session and report the result if:
- the GLB fails to import;
- the joint count is unexpectedly different;
- materials are missing or badly assigned;
- any expected animation is missing;
- the mesh imports with obvious deformation;
- Unreal asks for an unexpected skeleton during the initial import.

Do not create `BP_Dragon_Juvenile` or `ABP_Dragon_Juvenile` until the dragon asset passes these checks.
