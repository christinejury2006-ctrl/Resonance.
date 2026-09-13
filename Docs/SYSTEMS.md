# Dragonbound — Systems Architecture

> **Status:** Living document — Milestone 0 (foundation)
> **Last updated:** 2026-09-13
>
> This document specifies **how Dragonbound is built**: module and class
> architecture for every planned system, designed for the full game even
> when a milestone only needs part of it. Systems are built when their
> milestone arrives, in the shape specified here — so the project can grow
> for months without an architectural rewrite.
>
> Legend: **M1–M5** = vertical-slice milestones · **M6+** = post-slice
> roadmap (see DEVELOPMENT.md).

---

## 0. Architecture Principles

1. **C++ is the source of truth.** Gameplay logic, interfaces, and
   state live in C++. Blueprints are used for asset wiring, visual
   tuning, and authoring — never for core logic that must be reviewed,
   debugged, and refactored.
2. **Data-driven everything.** Stats, elemental mastery, dialogue,
   bond events, and equipment live in Data Assets / Data Tables. Design
   changes must not require code changes.
3. **Interfaces over inheritance.** Systems talk through Unreal
   interfaces and Gameplay Tags; avoid deep class hierarchies.
4. **Everything is gameplay-tagged.** A single global Gameplay Tag
   registry (e.g., `DB.Bond.Stage.…`, `DB.Power.Ember.…`,
   `DB.State.…`) drives abilities, dialogue, AI, and UI queries.
5. **Contexts are explicit.** Every gameplay context (OnFoot,
   Mounted, Flying, FirstPerson, ThirdPerson, Dialogue, Cinematic) is a
   named state the architecture can query — no boolean soup.
6. **No premature systems.** A class appears in the codebase in the
   milestone that needs it. Until then it is a specification below, not
   a stub.
7. **Modules stay small.** New feature domains become their own
   modules (e.g., `DragonboundFlight`) rather than fattening the primary
   module.

### Module map

| Module (UE) | Plugin | Contents | Lands |
| --- | --- | --- | --- |
| `Dragonbound` | — | Primary runtime module: core game classes | M0 (empty) → M1 |
| `DragonboundCamera` | ✓ | Camera mode stack, blending, cinematic camera | M1 |
| `DragonboundTouch` | ✓ | Touch-first virtual controls (joystick, drag-look, buttons) + injection into shared Enhanced Input actions | M1 (ADR-0003) |
| `DragonboundDialogue` | ✓ | Dialogue runtime, graph data assets, UI | M4 |
| `DragonboundEditor` | ✓ (editor) | Custom editors, asset validation, automation | M3+ |

---

## 1. Character System

**Purpose:** the playable Rider, controllable from two perspectives with
identical gameplay semantics.

### Architecture

```
DBRiderCharacter : ADBCharacterBase : ACharacter (or CharacterMovementComponent-based)
├── owns: DBRiderMovementComponent (M1) — camera-relative movement, mantle/vault hooks (M1/M3)
├── owns: DBRiderCameraModeStack? no — camera lives on the PlayerController (see §3)
├── owns: DBRiderEquipmentComponent (M3) — weapons, visual gear slots
├── owns: DBAbilitySystemComponent (M3, GAS) — owner of Rider abilities
├── implements: IDBPerspectiveAware (FOV/camera preferences per perspective)
├── implements: IDBInteractableUser (interaction queries)
└── Male/female: same class, different skeletal-mesh assets + MetaHuman
    bodies; shared animation blueprints (gender-neutral locomotion retargeted)
```

### Key classes (C++)

| Class | Responsibility | Milestone |
| --- | --- | --- |
| `ADBCharacterBase` | Shared health/attributes, team id, gameplay-tag state | M3 |
| `ADBRiderCharacter` | The playable Rider | M1 |
| `UDBRiderMovementComponent` | Camera-relative motion, sprint, jump, (later) mantle/vault | M1 |
| `UDBRiderEquipmentComponent` | Weapon/armor slots, visual+mechanical effects | M3 |
| `UDBRiderAbilitySet` (Data Asset) | Grants Rider abilities per element/unlock | M3 |
| `ADBPlayerController` | Input routing, camera mode stack owner, HUD | M1 |
| `UDBInputConfig` (Data Asset) | Input Mapping Contexts + Input Actions per context | M1 |

### Movement requirements

- Locomotion: walk/jog/sprint with acceleration/deceleration curves,
  camera-relative input, 8-direction blends, foot-planting.
- Jump/fall with weight; land feedback.
- (M3) combat-locomotion blend (strafe, lock-on gait).
- (M6+) mounted movement is *not* Rider locomotion — see §8.
- Perspective switch (M1) must not reset locomotion state (see §3).

### Input architecture (touch-first — ADR-0002/0003)

- **One pipeline, many devices.** All input sources — touch virtual
  controls, gamepad, keyboard/mouse — feed the same `UInputAction`
  assets; gameplay code never inspects a device. Touch injects via
  `UEnhancedPlayerInput::InjectInputForAction` (plugin
  `DragonboundTouch`); hardware input flows through Enhanced Input mapping
  contexts (`UDBInputConfig`).
- **Virtual controls:** left joystick (movement), right anywhere-drag
  (look), 2×2 action buttons (jump/sprint/interact/camera). Layout is
  fractional, DPI-aware, and safe-area-inset aware (`UDBTouchConfig`).
- Touch primacy is detected at runtime (mobile platforms /
  `UCommonInputSubsystem`); desktop testing via `DB.TouchControls.Force 1`.
- Touch look sensitivity/inversion are touch-config data, independent of
  the KBM modifier chain; deltas normalize against a 1080 reference
  height.

### MetaHuman plan

- Vertical slice: MetaHuman bodies/faces for the Rider and key NPCs;
  shared rig standard means all future characters benefit from the same
  facial-animation pipeline.
- First-person: MetaHuman body with true first-person camera + custom
  first-person arm mesh override for weapon/arm visibility.

---

## 2. Dragon System

**Purpose:** a full companion character — following, AI, combat,
dialogue, emotion, growth, mounting, and flight. **Never a mount with a
health bar.**

### Class architecture

```
ADBDragonCharacter : ADBCharacterBase (ACharacter + custom DragonMovementComponent)
├── implements: IDBCompanion, IDBPerspectiveAware, IDBInteractable, IDBDialogueSpeaker
├── owns: DBDragonMovementComponent (M6 flight) — replaces CharacterMovement when flying
├── owns: DBDragonAbilitySystemComponent (M3) — dragon abilities, combined-attack channel
├── owns: DBDragonVisualComponent (M2) — growth stage meshes, elemental material states
├── owns: DBDragonEmotionComponent (M2) — internal emotional state machine
└── driven by: DBDragonAIController — perception + StateTree brain (M2+)
```

| Class | Responsibility | Milestone |
| --- | --- | --- |
| `ADBDragonCharacter` | Pawn; attributes, abilities, interaction | M2 |
| `UDBDragonAIController` | Perception + StateTree execution | M2 |
| `UDBDragonPerceptionConfig` (Data Asset) | Sight/hearing ranges per growth stage | M2 |
| `UDBDragonEmotionComponent` | Emotional state (mood, intensity), decays & modifiers | M2 |
| `UDBDragonVisualComponent` | Growth-stage swapping, elemental material states | M2 (stages M4+) |
| `UDBDragonCombatComponent` | Target evaluation, attack selection, positioning | M3 |
| `UDBDragonFlightComponent` | Flight model, takeoff/landing, aerial combat | M6 |

### AI architecture

- **Perception:** AI Perception (sight/hearing) tuned per growth stage;
  the dragon notices things (enemies, points of interest, the Rider's
  state) and *reacts* — curiosity and protectiveness are authored states,
  not random movement.
- **Brain:** **StateTree** for decision logic; Behavior Trees only for
  low-level task sequences (move-to, attack routines, investigate).
  Smart Objects for resting perches, ledges, water sources.
- **Companion follow** is not "teleport behind player": it uses
  steering/locomotion with natural offsets per context (walking, combat,
  flight) and per growth stage.
- **Command channel (M3+):** the Rider's contextual commands
  (attack/defend/stay/follow) are *requests* weighted by bond stage and
  the dragon's emotional state — an un-bonded dragon ignores or improvises.
- **Emotion layer (M2):** emotional state is data (mood, intensity,
  decay), fed by events (story, combat, care, neglect, environment),
  expressed through animation layers, vocalization, idle behavior, and
  dialogue. The "dragon remembers" rule: high-valence events persist in a
  **memory list** (Data Asset/struct) that dialogue and AI consult.

### Growth stages

| Stage | Locomotion | Combat | Interaction | Lands |
| --- | --- | --- | --- | --- |
| Hatchling | carried / short hops | none (protect it) | feed, soothe, carry | M2 |
| Juvenile | ground, agile | ground combat, Ember basics | follow, play, protect | M3 |
| Young adult | ground + **flight** | ground + first aerial | ride, flight | M6 |
| Adult | full mastery | all elements, Unison | full partnership | M10+ |

Each stage is a **distinct designed model** (VISUAL_STYLE.md §3.2) with
its own movement/animation set; stages share the AI brain and attribute
sets with data-driven differences.

---

## 3. Camera System

**Purpose:** free first/third-person switching **in every context**
(exploration, combat, riding, flight) with cinematic quality in both.

### Architecture: camera mode stack on the PlayerController

```
ADBPlayerController
└── UDBCameraDirectorComponent
    ├── stack of UDBCameraMode (modes: 3P Exploration, 3P Combat, 3P Flight,
    │    1P Ground, 1P Combat, 1P Flight, Dialogue, Cinematic)
    ├── perspective state: EDBPerspective { ThirdPerson, FirstPerson } + per-context blend rules
    ├── blend: UE5 ViewTarget blending + custom camera modes (each mode implements
    │    GetPOV; the stack defines activation priority and blend curves)
    └── owns cinematic cameras: spring-arm/boom, rail, attach points
```

| Class | Responsibility | Milestone |
| --- | --- | --- |
| `UDBCameraDirectorComponent` | Mode stack, perspective switching, context routing | M1 |
| `UDBCameraMode` (base) | Abstract mode: input, blend, POV | M1 |
| `UDBCameraMode_ThirdPerson` | Over-shoulder; combat variant with lock-on offset | M1 (combat M3) |
| `UDBCameraMode_FirstPerson` | True FP with head/arm awareness | M1 |
| `UDBCameraMode_Flight` | Aerial follow, speed-based FOV/roll feel | M6 |
| `UDBCameraMode_Dialogue` | Framing from dialogue graph data | M4 |
| `UDBCameraMode_Cinematic` | Sequencer-driven, hands off | M4 |

### Switching contract (Pillar 5 enforcement)

- One input: **Toggle Perspective** (Enhanced Input), available in every
  context by design. Contexts provide *context-specific framing rules*,
  never a disabled toggle.
- Switch = crossfade ≤ 250 ms through the character's head position;
  locomotion/ability state is **never** reset by a switch.
- First-person must render the Rider's body awareness (arms, weapon,
  shadow) and keep the dragon fully visible/present in-frame where
  possible — the dragon should be *felt* beside you.
- Combat lock-on (M3) behaves per perspective (3P offset framing / 1P
  focus assist), not by disabling one perspective.
- Flight (M6): 1P = saddle view with dragon neck/head awareness; 3P =
  cinematic follow; both feed the same flight input contract.
- Tests (automation, M1+): switch during sprint, jump, combat combo,
  mounted, flight — state and feel preserved.

---

## 4. Combat System

**Purpose:** Rider melee, dragon combat AI, combined Rider+dragon
attacks, and the damage pipeline — all data-driven, all on GAS.

### Architecture

```
Gameplay Ability System (GAS)
├── DBAttributeSet_Health / _Stamina / _Bond? (no — bond is not GAS; see §6)
├── DBAbilitySystemComponent (Rider) + DBAbilitySystemComponent (Dragon)
├── Abilities:
│   ├── DBRider abilities: light/heavy combos, dodge, block, lock-on, elemental moves
│   ├── DBDragon abilities: breath, claw/bite/tail, wing buffet, elemental moves
│   └── DBCombinedAbility: two-phase channel — Rider initiates (GA on Rider ASC),
│       dragon confirms (GA on Dragon ASC) → synchronized effect, shared cooldown,
│       bond-gated (tag query), position/sync validated
├── GameplayEffects: damage, elemental status (burn, chill, shock…), buffs
├── Damage pipeline: DBCombatComponent → attribute change → hit reactions
│   (physical materials, impact VFX/SFX), damage numbers optional-off
└── Targeting: soft/hard lock-on (3P), focus assist (1P), dragon target
    share via tag `DB.Target.Shared`
```

| Class | Responsibility | Milestone |
| --- | --- | --- |
| `UDBAbilitySystemComponent` | Shared ASC base (owner tagging, combo windows) | M3 |
| `UDBCombatComponent` | Hit detection, damage pipeline, hit reactions | M3 |
| `UDBCombinedAbility` | Rider+dragon synchronization protocol | M3 |
| `UDBEnemyBase` | Enemy archetype base (3 slice archetypes as data variants) | M3 |
| `UDBAttributeSets` | Health/Stamina/elemental pools as Data Assets | M3 |

### Combined attack protocol (the combat identity)

1. Rider initiates a channel (input + bond-stage + tag gating).
2. Dragon ASC receives a **combine request** — validated by distance,
   line-of-sight, dragon state (not busy, not distressed).
3. Dragon animates into position (Motion Warping), Rider animation
   syncs via montage notify.
4. Effect fires as **one** gameplay event with both ASCs as sources —
   damage, VFX, camera kick, audio all keyed to that event.
5. Failure cases degrade gracefully (dragon interrupted → Rider's move
   completes solo; no invisible teleports).

### Enemy archetypes (slice)

Light skirmisher / shielded brute / ranged harrier — data-driven variants
of `UDBEnemyBase` (health, speed, attack sets, AI brain subtrees), tuned
for readable tells (VISUAL_STYLE.md §3.1).

---

## 5. Powers System

**Purpose:** the seven elements as **six-layer capability packages**
(dragon abilities, Rider abilities, Rider combat moves, dragon combat
moves, combined attacks, mastery progression), unlocked progressively.

### Architecture

```
Powers are DATA + GAS, not code-per-element:
├── UDBElementalPowerDefinition (Data Asset) — the element: identity, colors,
│   VFX/audio language, lore, unlock story hook
├── UDBAbilitySet per layer (dragon/Rider/combined) → granted to the relevant ASC
├── DBMasteryTable (Data Table) — per-element upgrade tree
├── Gameplay Tags gate everything: DB.Power.Ember, DB.Power.Ember.Layer.Combined, …
└── Unlock = story event grants tags + ability sets (DBProgressionComponent)
```

| Class | Responsibility | Milestone |
| --- | --- | --- |
| `UDBElementalPowerDefinition` | Element identity and package reference | M3 (Ember) |
| `UDBAbilitySet` | Grants grouped abilities to an ASC | M3 |
| `UDBMasteryComponent` | Mastery points/levels per element (Rider + dragon sides) | M3 (UI M5) |
| `UDBProgressionComponent` | Global unlock/progression state (story flags, tags) | M3 |

### Rules

- **No per-element C++ classes for mechanics.** Differences are data:
  gameplay effects, curves, montages, VFX, AI weights. Element-specific
  *behavior* only where data cannot express it (rare, reviewed).
- Ember (M3) is the template: its full six-layer package is built first
  and becomes the checklist every later element must complete.
- Element switching (M7+): active element is tag-driven; the dragon
  visually and behaviorally reflects the active element (material states,
  breath type, AI move weights).
- Rider powers **mirror the bond** — same element, human-scale
  expression (Ember: torch/ignition utility, ignited blade, burning
  arrows) — always a counterpart to the dragon's expression.

---

## 6. Bond System

**Purpose:** the central relationship state — stage-gated communication,
memory, and emotional context. Explicitly **not** a GAS attribute or a
grind meter.

### Architecture

```
UDBBondComponent (on the Rider; the dragon holds its own mirror)
├── BondStage: enum { Awakening, Feeling, Words, Dialogue, Unison } — monotonic, story-driven
├── BondDepth: bounded value grown ONLY by story events and shared experiences
│   (battle wins together, discoveries, choices) — never by repeatable grinding
├── Event log: DBondEvent { type, timestamp, valence, context tags, memory-flagged? }
│   → dragon "memory" = filtered/flagged events (the dragon remembers)
├── Trust/Resonance: derived scores feeding dialogue options, command
│   compliance, combined-attack availability, and story branches
└── Mind-speech channel: UDBMindLinkComponent
    ├── Dragon → Rider: stage-gated payloads
    │   Awakening: raw emotion pulses
    │   Feeling:    emotion + sensation + image flashes
    │   Words:      fragmentary speech (subtitles + voice)
    │   Dialogue:   full dialogue (routed through Dialogue system §7)
    │   Unison:     shared perception (late game)
    └── Presentation: audio signature + visual layer (VISUAL_STYLE §4) — M2
```

| Class | Responsibility | Milestone |
| --- | --- | --- |
| `UDBBondComponent` | Stage, depth, events, trust/resonance | M2 |
| `UDBMindLinkComponent` | Mind-speech channel + presentation hooks | M2 |
| `UDBBondEvent` (struct) | Serialized bond events | M2 |
| `UDBBondDefinition` (Data Asset) | Stage thresholds, tuning per stage | M2 |

### Rules

- Bond stage is **story-monotonic** (save/load safe as a single enum +
  depth value).
- Bond depth gates *soft* things (dialogue options, command compliance,
  combined attacks); story gates the hard things (stages, elements).
- The dragon's mirror state (emotion, memory) lives on the dragon — the
  bond is a **two-sided** data relationship, not a player stat.
- Every major story choice writes a `DBondEvent`; dialogue and AI
  consult memory. This is the "the dragon remembers" rule made concrete.

---

## 7. Dialogue System

**Purpose:** NPC conversations, the dragon's spoken mind-speech, quest
hooks, and cinematic framing — one system for all of it.

### Architecture

```
DragonboundDialogue plugin (M4)
├── UDBDialogueAsset (Data Asset): graph of nodes
│   ├── nodes: line, choice, condition (bond stage/depth/tags/quest state),
│   │   event (grant quest flag, bond event, camera directive), jump
│   ├── speakers: NPCs and the Dragon (mind-speech channel flag)
│   └── camera directives per node (framing, shot type)
├── UDBDialogueComponent (on participants): Start/End, line delivery,
│   voice/audio, mind-speech presentation for dragon lines
├── Localization-ready text (FText everywhere, voice table keyed by line id)
└── Runtime is UI-agnostic: the plugin emits line/choice events;
    the HUD/dialogue widget (CommonUI) renders them
```

| Class | Responsibility | Milestone |
| --- | --- | --- |
| `UDBDialogueAsset` | Dialogue graph data | M4 |
| `UDBDialogueComponent` | Runtime playback, delivery | M4 |
| `UDBDialogueNode` (UObject hierarchy) | Node types | M4 |
| Dialogue widget (CommonUI) | Presentation | M4 |

### Rules

- One runtime for NPC dialogue **and** dragon mind-speech — the dragon
  is a speaker with a channel flag, not a separate dialogue system.
- All dialogue conditions are tag/data queries — no dialogue-specific
  C++ hacks per quest.
- Voice-over: line-id-keyed audio tables; placeholder TTS allowed in
  development, never shipped.

---

## 8. Flight System

**Purpose:** dragon flight as a full traversal and combat mode —
takeoff, flight model, aerial exploration, aerial combat, landing.

### Architecture

```
UDBDragonFlightComponent + DBDragonMovementComponent (M6)
├── Flight model: custom movement mode replacing ground locomotion while airborne
│   (lift/drag curves, speed states: hover, cruise, sprint, dive; stamina pool)
├── Transitions: takeoff (ground run or vertical launch), landing (perch + ground)
├── Mounted Rider: attach points per dragon stage; Rider input = flight control
│   (Enhanced Input flight mapping context); Rider can go FP/TP freely (camera §3)
├── Aerial combat: flight-specific dragon abilities (dive, strafe breath,
│   wing buffet), mounted Rider abilities (saddle casting/archery),
│   aerial combined attacks (same protocol as ground — §4)
├── Exploration: air current systems, updrafts, landing zones (Smart Objects),
│   world streaming tuned for flight speed (World Partition streaming source
│   follows the dragon, not the player)
└── First-person flight: saddle view with dragon head/neck awareness; the
    horizon/velocity read is tuned as its own feel problem
```

### Rules

- Flight is **not** "character movement with gravity off" — it is a
  separate movement mode with its own feel tuning, state machine, and
  animation graph (glide, flap cycles, banking).
- Ground and air gameplay share the same dragon (same pawn, same ASC) —
  flight changes movement mode and ability availability, never identity.
- Takeoff/landing are **physical** (clearance, approach, weight) — a
  dragon that can't fit doesn't land there. This constraint drives level
  design, which is intentional.
- Flight is earned in the story (young-adult stage, M6); nothing about
  the ground game is designed as "flight but worse."

---

## 9. Progression System

**Purpose:** unlocks, saves, and the player-facing progression model
(Rider skill, dragon growth, elemental mastery, bond).

### Architecture

```
UDBProgressionComponent (on the Rider) — source of truth for unlocks
├── Unlock flags: gameplay tags (DB.Unlock.*) granted by story/quest events
├── Elemental mastery: per-element mastery records (Rider side + dragon side)
│   → UDBMasteryComponent owns the tree; tables are data
├── Dragon growth: stage enum owned by the dragon (DBDragonVisualComponent/state)
├── Save data: UDBSaveGame — serialized snapshot of progression, bond, dragon
│   state, quest state, world state; versioned (SchemaVersion) with migration
│   hooks from day one (M3+)
└── Acquisition rules: story grants capabilities, exploration grants mastery
    (GAME_DESIGN §7.3) — enforced by tag ownership, no XP curves
```

| Class | Responsibility | Milestone |
| --- | --- | --- |
| `UDBProgressionComponent` | Unlock flags, save interface | M3 |
| `UDBMasteryComponent` | Per-element mastery state | M3 (UI M5) |
| `UDBSaveGame` | Versioned save schema | M3 |
| `UDBQuestTrackerComponent` | Quest/dialogue state (M4) | M4 |

### Rules

- There is **no generic XP/level system**. Progression is
  story+exploration+mastery; if a system ever needs a number, it is a
  bounded, named value (bond depth, mastery rank), not a level.
- Save data is versioned from its first milestone; no migration-less
  schema changes after M4 content lands.

---

## 10. Cross-System Contracts (how the pieces hold together)

1. **Perspective:** `EDBPerspective` + context tags are readable by
   camera, combat, dragon AI, and UI. No system may assume a perspective.
2. **Bond gating:** combined attacks, dragon commands, and dialogue
   options query bond stage/depth via tags + `UDBBondComponent` — combat
   never hard-codes bond values.
3. **The dragon's autonomy:** combat and follow AI respect the dragon's
   emotional state and memory; commands are requests, weighted by bond.
   No system may teleport the dragon for convenience (except explicitly
   authored, reviewed cases).
4. **Save boundary:** everything persistent lives in `UDBSaveGame` via
   components that implement `IDBSaveable`. The save system is introduced
   the moment the first persistent system (bond) lands.
5. **Input:** Enhanced Input only. Mapping contexts per gameplay
   context (OnFoot, Mounted, Flying, Dialogue, UI); remapping via
   Enhanced Input's native mapping (M2).
6. **Naming:** `DB` prefix everywhere; classes named for what they are
   (`ADBDragonCharacter`, not `ADBDragonPawnBaseV2`).

---

## 11. Content Conventions

```
Content/Dragonbound/
├── Core/          # BP variants of C++ bases, game mode, HUD
├── Characters/
│   ├── Rider/     # meshes, materials, anim BPs, MetaHuman sources
│   └── Dragon/    # per-stage meshes, materials, anim BPs, rig
├── Powers/
│   ├── Ember/     # ability configs, GEs, VFX, audio, mastery table
│   ├── Tide/ …    # one folder per element, created when the element starts
├── AI/            # StateTrees, perception configs, Smart Object defs
├── Dialogue/      # dialogue assets, voice tables
├── Maps/          # slice + future maps (single-owner rule)
├── Data/          # data tables, tag registrations, global data assets
└── UI/            # widgets (CommonUI), HUD
```

- Asset naming: `DB_<Type>_<Name>` (e.g., `DB_GA_Ember_Breath`, `DB_Map_SliceVale`).
- C++ classes are the base; BP assets derive for tuning. No
  logic-only BPs.
- Maps: one owner at a time (binary conflict rule).

---

## 12. What This Document Deliberately Leaves for Later

Specified at their milestone, not now:

- Quest/dialogue runtime details (M4), save schema v1 fields (M3),
  element-switching UX (M7), audio architecture (M2 for mind-speech
  signature, M3 combat audio, M4 mix), networking (single-player only —
  any multiplayer decision is a future ADR), controller remapping UX
  (M2), analytics/telemetry (M5+).
