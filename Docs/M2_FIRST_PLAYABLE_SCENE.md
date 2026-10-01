# Dragonbound — M2 First Playable Scene Checklist

This is the exact UE 5.8 editor pass needed to turn the repository-side M2 systems into a playable companion prototype.

## A. Create the content folders

Create:

`Content/Dragonbound/Characters/Dragon/`  
`Content/Dragonbound/AI/`  
`Content/Dragonbound/Animation/Dragon/`  
`Content/Dragonbound/Audio/Dragon/`  
`Content/Dragonbound/Maps/`

## B. Juvenile dragon Blueprint

Create:

`BP_Dragon_Juvenile`

Parent class: `ADBDragonCharacter`.

Set the Visual Component:
- Growth Stage = Juvenile.
- Keep the supplied Silhouette Profile.
- Assign the temporary or original juvenile skeletal mesh.

Required visible result:
- Dragon is clearly smaller/younger than the planned adult.
- Long neck/head, swept wings, long tail and lean body silhouette remain readable.
- Capsule collision covers the body without swallowing the mesh.

## C. Animation Blueprint

Create:

`ABP_Dragon_Juvenile`

Minimum state inputs:
- Speed
- Direction
- IsInAir
- VisualState
- GrowthStage

Minimum locomotion states:
- Idle
- Walk
- Run
- Turn
- Rest

Minimum reaction states:
- Alert
- Comforted
- Distressed
- Protective

Bind `UDBDragonVisualComponent::OnVisualStateChanged` so state transitions can drive reaction animations without putting animation assets in gameplay C++.

## D. StateTree

Create:

`ST_DragonCompanion`

Use the runtime states:
- Idle
- Curious
- Follow
- Protect
- React

The StateTree should orchestrate high-level behavior only.

Do not duplicate:
- bond-stage rules;
- trust calculations;
- interaction effects;
- mind-link payload generation.

Those remain in the gameplay components.

## E. Navigation

Add a NavMeshBoundsVolume covering the companion test area.

The dragon must be able to:
1. start beside the Rider;
2. approach the Rider after Call;
3. follow while within the configured range;
4. stop without walking through the Rider;
5. remain grounded on ordinary terrain.

## F. Audio placeholders

Create simple placeholder Sound Cues for:
- Curious
- Comforted
- Distressed
- Protective
- Mind-link Warmth
- Mind-link Safety

They can be temporary. The purpose is to prove that emotion can have a non-verbal audio signature.

## G. First playable test sequence

Start with Rider and juvenile dragon together.

1. Look at dragon → interaction prompt reads **Call to the dragon**.
2. Press Interact → dragon becomes Curious and approaches.
3. Feed → dragon becomes Comforted; Rider receives Warmth.
4. Soothe → dragon becomes Comforted; Rider receives Safety.
5. Protect → dragon becomes Protective.
6. Walk away → dragon uses Follow when bond/trust conditions permit.
7. Leave and return during the same session → bond memory remains available.
8. Trigger several memory-flagged interactions → Awakening can advance to Feeling.
9. Observe the Animation Blueprint reacting to visual-state changes.

## H. M2 exit condition

M2 is not complete merely because the Blueprint exists.

The dragon must **read as alive for a full session**:
- locomotion is continuous;
- perception produces observable reactions;
- calls change behavior;
- emotions change body language/audio;
- interactions alter bond memory;
- mind-link payloads reach the Rider;
- the companion does not require UI text to explain every reaction.

## Explicitly out of scope

Do not add:
- flight;
- dragon combat;
- Ember;
- adult dragon;
- full dialogue;
- Academy systems;
- complex quests;
- GAS;
- enemies;
- open-world production.

Those belong to later milestones.
