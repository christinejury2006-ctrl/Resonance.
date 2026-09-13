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
away. Through a deepening bond, the dragon unlocks elemental powers — and
the Rider develops corresponding abilities of their own. The player chooses
a male or female protagonist and can switch freely between first-person and
third-person perspectives in every part of the game: exploration, combat,
riding, and flight.

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
| [Docs/decisions/](Docs/decisions/) | Architecture Decision Records (ADR) |

---

## Project Structure

```
Dragonbound/
├── Dragonbound.uproject
├── Config/                  # Shared editor/project config (.ini) — reviewed with every change
├── Content/                 # Game content (BP assets, maps, data tables, art imports)
│   └── Dragonbound/         # Project namespace root (added in Milestone 1)
│       ├── Core/            # Base classes, game mode, player, camera
│       ├── Characters/      # Rider, Dragon
│       ├── Powers/          # Elemental power assets & gameplay ability configs
│       ├── Maps/
│       └── Data/            # Data tables, data assets, tag registration
├── Source/
│   ├── Dragonbound.Target.cs
│   ├── DragonboundEditor.Target.cs
│   └── Dragonbound/         # Primary game module (C++ core)
│       ├── Public/
│       └── Private/
├── Docs/                    # This documentation suite
└── Tools/                   # Team scripts (added as needed)
```

**Content rules**

- All project content lives under `Content/Dragonbound/` with a clear
  subfolder per system. Marketplace packs are installed to
  `Content/External/` (git-ignored, reinstallable) and migrated into
  `Content/Dragonbound/` only when modified or renamed.
- No assets in the top-level `Content/` folder.
- Naming conventions are enforced as they are defined (see SYSTEMS.md).

---

## Getting Started

> Note: this repository intentionally contains **no content assets yet**.
> Milestone 1 introduces the first Rider, camera, and test map.

### 1. Prerequisites

- **Unreal Engine 5.8** — via Epic Games Launcher (recommended), or a
  source build from the EpicGames/UnrealEngine GitHub `5.8` branch
  (requires Epic GitHub account access).
- **Git** with **Git LFS** (`git lfs install` once per machine) — all
  binary assets are stored via LFS (see `.gitattributes`).
- **Visual Studio 2022** (Windows) or **Xcode 15+** (macOS) with the
  Unreal C++ toolchain. See
  [Epic's hardware/software requirements](https://dev.epicgames.com/documentation/en-us/unreal-engine/hardware-and-software-specifications-for-unreal-engine).

### 2. Clone

```bash
git lfs install
git clone https://github.com/christinejury2006-ctrl/Resonance..git Dragonbound
cd Dragonbound
```

### 3. Open the project

- Double-click `Dragonbound.uproject`, or:
  `"C:\Program Files\Epic Games\UE_5.8\Engine\Build\BatchFiles\RunUAT.bat" ...`
- Choose **Yes** if prompted to rebuild missing modules.
- The editor generates `Binaries/`, `Intermediate/`, `Saved/` — all
  git-ignored.

### 4. Build from the command line (optional)

Windows (Developer 64-bit, Development configuration):

```bat
"C:\Program Files\Epic Games\UE_5.8\Engine\Build\BatchFiles\Build.bat" DragonboundEditor Win64 Development -Project="C:\path\to\Dragonbound.uproject" -WaitMutex
```

---

## Team Workflow

- **Branching:** main-based trunk development. `main` must always compile.
  Feature work on short-lived `feature/<name>` branches. See
  [Docs/DEVELOPMENT.md](Docs/DEVELOPMENT.md) for the full convention.
- **Binary asset locking:** `uasset`/`umap` files cannot be merged. Two
  people must not edit the same asset; for large teams, adopt
  `git lfs lock` on assets in progress (documented in DEVELOPMENT.md).
- **Code style:** Unreal Engine C++ coding standard (UpperCamelCase for
  types, `b` prefix for bools, `UPROPERTY` on all UHT-exposed members).
  See `CONTRIBUTING.md` (added with the first code milestone).
- **World building:** one developer owns a given map at a time (maps
  cannot be merged).

---

## Legal

Dragonbound is an original intellectual property. **Do not import names,
characters, terminology, artwork, music, or lore from existing fantasy
franchises** (see GAME_DESIGN.md → Originality Policy). All third-party
assets must have verifiable licenses; see VISUAL_STYLE.md → Asset Sourcing.

License: [LICENSE.md](LICENSE.md) — proprietary, all rights reserved.
