# Dragonbound

> **A cinematic 3D fantasy action-adventure RPG about the bond between a
> Rider and the last dragon of an endangered lineage.**

> **Repository:** https://github.com/christinejury2006-ctrl/Resonance.
> (The repository's GitHub name ends with a dot — `Resonance.` — so its
> clone URL contains `Resonance..git`. The project and game title is
> *Dragonbound*.)

Dragonbound is an original intellectual property in active development.
The player steps into the boots of an ordinary person who discovers the
final surviving hatchling of a dragon lineage that has been magically sealed
away. Through a deepening bond, the dragon unlocks elemental powers — and the
Rider develops corresponding abilities of their own. The player chooses a
male or female protagonist and can switch freely between first-person and
third-person perspectives in every part of the game: exploration, combat,
riding, and flight.

## Current Status

**M1 runtime foundation complete. M2 runtime foundation complete; editor
content is the remaining playable-slice work.** M2 now includes the dragon
character, growth/readability state, perception + StateTree bridge, mood
model, Awakening/Feeling bond stages, mind-link payloads, Rider↔dragon
interactions, persistent bond memory events, and automated world-less smoke
tests.

The supplied source assets are now inspected and have explicit UE import
contracts: the juvenile dragon contains 7 animation clips and the adventurer
contains 4 Rider animation clips. The actual Unreal Blueprint/Animation
Blueprint imports still require UE 5.8 editor validation.

**Important:** the M2 C++ foundation has not been compiled in a UE 5.8
environment in this repository workflow yet. Do not treat the automation
tests as passed until they are run in Unreal.

## Documentation Map

| Document | Purpose |
| --- | --- |
| `Docs/GAME_DESIGN.md` | Current game design specification |
| `Docs/DEVELOPMENT.md` | Staged development roadmap |
| `Docs/VISUAL_STYLE.md` | Visual-quality requirements |
| `Docs/SYSTEMS.md` | Technical architecture |
| `Docs/TODO.md` | Current task list |
| `Docs/DRAGON_GLB_IMPORT.md` | Exact dragon GLB source and UE import contract |
| `Docs/DRAGON_ANIMATION_CONTRACT.md` | Dragon animation state mapping |
| `Docs/ADVENTURER_GLB_IMPORT.md` | Exact human GLB source and Rider import contract |
| `Docs/decisions/` | Architecture Decision Records (ADR) |

## Project Structure

```
Dragonbound/
├── Dragonbound.uproject
├── Config/
├── Content/
│   ├── External/              # Immutable source art/assets (Git LFS)
│   │   ├── Dragon/
│   │   │   └── war_dragon_rigged.glb
│   │   └── Human/
│   │       └── adventurer_rigged.glb
│   └── Dragonbound/
│       ├── Core/
│       ├── Characters/
│       ├── Animation/
│       ├── Powers/
│       ├── Maps/
│       └── Data/
├── Source/
├── Docs/
└── Tools/
```

**Content rules**

- Runtime project content lives under `Content/Dragonbound/`.
- Immutable source art lives under `Content/External/` and is tracked with
  Git LFS when committed.
- The supplied GLBs are source assets only; imported Unreal assets belong
  under the Dragonbound runtime namespace.
