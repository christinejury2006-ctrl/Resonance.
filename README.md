# Dragonbound

> **A cinematic 3D fantasy action-adventure RPG about the bond between a
> Rider and the last dragon of an endangered lineage.**

> **Repository:** https://github.com/christinejury2006-ctrl/Resonance.
> (The repository's GitHub name ends with a dot — `Resonance.` — so its
> clone URL contains `Resonance..git`. The project and game title is
> *Dragonbound* — see below.)

Dragonbound is an original intellectual property in active development.
The player steps into the boots of an ordinary person who discovers the
final surviving hatchling of a dragon lineage that has been magically sealed
away. Through a deepening bond, the dragon unlocks elemental powers — and the
Rider develops corresponding abilities of their own. The player chooses a
male or female protagonist and can switch freely between first-person and
third-person perspectives in every part of the game: exploration, combat,
riding, and flight.

---

## Current Status

**M1 runtime foundation complete. M2 runtime foundation complete; editor
content is the remaining playable-slice work.** M2 now includes the dragon
character, growth/readability state, perception + StateTree bridge, mood
model, Awakening/Feeling bond stages, mind-link payloads, Rider↔dragon
interactions, persistent bond memory events, and automated world-less smoke
tests. The remaining M2 work is Unreal Editor authoring: the juvenile dragon
Blueprint/mesh/animation set, StateTree asset, NavMesh-backed first playable
scene, mind-speech presentation hooks, and the full-session exit test.

**Important:** the M2 C++ foundation has not been compiled in a UE 5.8
environment in this repository workflow yet. Do not treat the automation
tests as passed until they are run in Unreal.

**After cloning, follow [Docs/EDITOR_SETUP.md](Docs/EDITOR_SETUP.md)** to
author the input assets, touch config, appearance definitions, and the
grey-box map before playing.

### Controls (M1 defaults, authored in `DB_InputConfig` / `DB_TouchConfig`)

| Action | Touch (primary) | Keyboard/Mouse | Gamepad |
| --- | --- | --- | --- |
| Move | Left virtual joystick | W/A/S/D | Left Stick |
| Look | Right-side drag | Mouse | Right Stick |
| Jump | Bottom-right button | Space | Face Button Bottom |
| Sprint | Hold button | Left Shift (hold) | Face Button Left |
| Toggle 1st/3rd person | Camera button | V | Face Button Right |
| Interact | Interact button | E | Face Button Top |

> Touch controls activate automatically on phones; force them on desktop
> with the console command `DB.TouchControls.Force 1` (mouse simulates
> touch).

---

## Engine

**Unreal Engine 5.8** (C++ gameplay, Blueprint for tuning/authoring).

Rationale: cinematic realistic 3D with dynamic lighting and dense
vegetation is exactly what UE5's feature set (Lumen, Nanite, MetaHuman,
Niagara, cinematic tools) is built for; the target platforms (PC and
current-gen consoles) map one-to-one to UE5's supported platforms; and the
marketplace ecosystem provides high-quality medieval-fantasy art to reach
visual targets fast. UE 5.8 is the final planned major UE5 release, giving
the project the longest possible supported baseline before any future UE6
transition. Full decision record: [Docs/decisions/0001-engine-choice.md](Docs/decisions/0001-engine-choice.md).

---

## Documentation Map

| Document | Purpose |
| --- | --- |
| [Docs/GAME_DESIGN.md](Docs/GAME_DESIGN.md) | Current game design specification — pillars, story, world, characters, powers, controls |
| [Docs/DEVELOPMENT.md](Docs/DEVELOPMENT.md) | Staged development roadmap — milestones, vertical slice plan, testing, branching |
| [Docs/VISUAL_STYLE.md](Docs/VISUAL_STYLE.md) | Visual-quality requirements and art-direction standards |
| [Docs/SYSTEMS.md](Docs/SYSTEMS.md) | Technical architecture for all planned systems |
| [Docs/TODO.md](Docs/TODO.md) | Current task list — the team's immediate working queue |
| [Docs/DRAGON_GLB_IMPORT.md](Docs/DRAGON_GLB_IMPORT.md) | Exact juvenile dragon GLB source and UE 5.8 import contract |
| [Docs/DRAGON_ANIMATION_CONTRACT.md](Docs/DRAGON_ANIMATION_CONTRACT.md) | Animation naming, state mapping, and Blueprint contract |
| [Docs/decisions/](Docs/decisions/) | Architecture Decision Records (ADR) |

---

## Project Structure

```
Dragonbound/
├── Dragonbound.uproject
├── Config/                  # Shared editor/project config (.ini) — reviewed with every change
├── Content/                 # Game content (BP assets, maps, data tables, art imports)
│   ├── External/             # Immutable source art/assets (Git LFS)
│   │   └── Dragon/
│   │       └── war_dragon_rigged.glb
│   └── Dragonbound/         # Project namespace root
│       ├── Core/
│       ├── Characters/
│       ├── Powers/
│       ├── Maps/
│       └── Data/
├── Source/
│   ├── Dragonbound.Target.cs
│   ├── DragonboundEditor.Target.cs
│   └── Dragonbound/
├── Docs/
└── Tools/
```

**Content rules**

- Runtime project content lives under `Content/Dragonbound/` with a clear
  subfolder per system.
- Immutable source art may live under `Content/External/` and is tracked with
  Git LFS when committed; the Dragonbound runtime assets are authored under
  `Content/Dragonbound/`.
- The supplied `war_dragon_rigged.glb` is the M2 juvenile source asset. See
  `Docs/DRAGON_GLB_IMPORT.md` for the import contract.

---

## Getting Started

> Note: the repository now contains the **runtime C++ foundation** for M1
> and M2, but binary/editor-authored content is still intentionally
> machine-authored. The Unreal Editor is required for Blueprints, StateTree,
> Animation Blueprints, meshes, maps, NavMesh, and audio assets.

### 1. Prerequisites

- **Unreal Engine 5.8**
- **Git** with Git LFS
- **Visual Studio 2022** (Windows) or **Xcode 15+** (macOS) with the Unreal C++ toolchain.

### 2. Clone

```bash
git lfs install
git clone https://github.com/christinejury2006-ctrl/Resonance..git Dragonbound
cd Dragonbound
```

### 3. Open the project

- Double-click `Dragonbound.uproject`.
- Choose **Yes** if prompted to rebuild missing modules.
- The editor generates `Binaries/`, `Intermediate/`, `Saved/` — all
  git-ignored.

### 4. Build from the command line (optional)

```bat
"C:\Program Files\Epic Games\UE_5.8\Engine\Build\BatchFiles\Build.bat" DragonboundEditor Win64 Development -Project="C:\path\to\Dragonbound.uproject" -WaitMutex
```

---

## Team Workflow

- **Branching:** main-based trunk development. `main` must always compile.
  Feature work on short-lived `feature/<name>` branches.
- **Binary asset locking:** `uasset`/`umap` files cannot be merged.
- **Code style:** Unreal Engine C++ coding standard.
- **World building:** one developer owns a given map at a time.

---

## Legal

Dragonbound is an original intellectual property. **Do not import names,
characters, terminology, artwork, music, or lore from existing fantasy
franchises** (see GAME_DESIGN.md → Originality Policy). All third-party
assets must have verifiable licenses; see VISUAL_STYLE.md → Asset Sourcing.

License: [LICENSE.md](LICENSE.md) — proprietary, all rights reserved.
