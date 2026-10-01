# Dragonbound — M2 Editor Setup

> Unreal Engine 5.8 content-authored setup for the Dragon Companion and Bond v1.
> C++ supplies the runtime systems; the editor supplies the actual assets.

## 1. Dragon Blueprint

Create Content/Dragonbound/Characters/Dragon/BP_Dragon_Juvenile as a Blueprint child of ADBDragonCharacter.

Required components are already created by C++:
- DragonEmotion
- DragonBond
- DragonMindLink
- DragonInteraction

For the first playable pass, use any temporary skeletal mesh with capsule collision. Replace the mesh later without changing the gameplay class.

## 2. StateTree asset

Create Content/Dragonbound/AI/ST_DragonCompanion as a StateTree asset intended for UStateTreeAIComponent.

The runtime controller supports these states and has a safe C++ fallback when no StateTree asset is assigned:
- Idle — no current focus; stop movement.
- Curious — nearby perceived actor or direct call; observe rather than blindly attack.
- Follow — bond trust is high enough and the focus is within follow range.
- Protect — explicit protect interaction; hold position around the Rider/protected actor.
- React — perceived actor is outside the close range; approach using navigation.

Keep the StateTree responsible for high-level orchestration and presentation hooks. Do not duplicate C++ bond rules or create a second source of truth for trust/stage.

## 3. Perception

The C++ controller tunes perception from the dragon's growth stage:

| Growth stage | Sight | Lose sight | Hearing |
| --- | ---: | ---: | ---: |
| Hatchling | 700 cm | 850 cm | 650 cm |
| Juvenile (M2) | 1200 cm | 1500 cm | 950 cm |
| Young Adult | 1800 cm | 2200 cm | 1400 cm |
| Adult | 2400 cm | 3000 cm | 1900 cm |

Peripheral vision remains 100 degrees. The runtime fallback uses these values automatically; later StateTree/Data Asset tuning can refine them without changing the companion architecture.

## 4. Interaction test

The dragon implements IDBInteractable, so the M1 interaction trace can target it.

Expected prompt: Call to the dragon.

Press the existing interact action while the dragon is the current interaction target.

Expected result:
1. Dragon mood becomes Curious.
2. Dragon AI focuses the Rider and moves toward them.
3. A bond event is recorded.
4. The interaction delegate fires.

## 5. Bond memory test

Use Feed, Soothe and Protect during a play session. Each memory-flagged action is stored in UDBBondComponent::Memory and increases bond depth/trust. Soothe emits the mind-link cue Safety; Feed emits Warmth. Both payloads are forwarded to the Rider-side receiver.

## 6. M2 exit test

A successful editor pass should show the dragon as a persistent companion rather than a static prop:
- notices actors
- reacts to a call
- approaches/follows when trust allows
- can become protective
- exposes readable mood changes
- leaves persistent memory entries after Feed/Soothe/Protect
- sends mind-link payloads to the Rider receiver

The actual StateTree .uasset, skeletal mesh, animation blueprint, audio and final dragon model must be authored in UE 5.8. They should not be fabricated as text files in Git.
## 7. Presentation hooks

Bind UDBDragonEmotionComponent::OnMoodChanged in the dragon Animation Blueprint, audio controller, or Blueprint presentation layer. The gameplay component deliberately does not hard-code animation/audio assets. This keeps the same emotional state usable with temporary and final dragon art.
