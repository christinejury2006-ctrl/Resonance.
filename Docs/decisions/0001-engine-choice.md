# ADR-0001 — Engine Choice: Unreal Engine 5.8

- **Status:** Accepted
- **Date:** 2026-09-13
- **Deciders:** Lead Game-Engineering Architect

## Context

Dragonbound targets **cinematic, realistic 3D fantasy** visuals with dynamic
lighting, atmospheric effects, dense vegetation, high-quality VFX, natural
animation, and cinematic camera work. Target platforms are **PC (high-end
first) and current-generation consoles**; the architecture must allow later
optimization for different hardware levels. Development is a long-term,
incremental vertical-slice approach, built in an engine whose tooling
supports solo-to-small-team iteration speed.

Candidate engines: **Unreal Engine 5.x**, **Unity 6 / Unity 2022 LTS**,
**Godot 4.x**.

## Decision

**Adopt Unreal Engine 5.8** as the project engine and platform baseline.

- Gameplay architecture in **C++** (fast iteration in Blueprint layered on top).
- UE 5.8 is the final planned major UE5 release (Epic announced 5.8 on
  2026-06-23 and described it as the last major 5.x release on the roadmap),
  giving the project the longest supported baseline in the UE5 line.
- UE6 was publicly announced (2026-05-24) but has no feature set, preview
  build, or release date (expected preview builds ~2027+). It is **not** a
  viable foundation for a project starting now; the project will track UE6
  and evaluate migration much later.

## Rationale

| Requirement | UE5.8 fit |
| --- | --- |
| Cinematic realistic 3D, dynamic lighting, dense vegetation, high-quality VFX | Lumen (global illumination/reflections), Nanite (virtualized geometry), Niagara (VFX), Chaos (physics/destruction), MetaHuman (characters) |
| PC + current-gen consoles | First-class platform support, TSR/upscaler scalability path for lower hardware tiers |
| Dragon companion: flight, animation quality | Control Rig, full-body IK, physics-driven animation, Sequencer for cinematic/attached cameras |
| Premium open-world ambition later | World Partition, HLOD, streaming tools designed for large worlds |
| Small team, fast visual target | Huge marketplace of verified medieval-fantasy art (characters, dragons, environments, animation packs) + MetaHuman for hero NPCs |
| Rider ability system + AI companion | Gameplay Ability System (GAS), AI Perception, Behavior Trees, Smart Objects, StateTree, MassEntity |
| Long-term stability | Mature 5.x line at its final major release; 5% royalty only beyond $1M USD revenue; source access via Epic GitHub |

### Why not Unity 6

- Realistic/cinematic rendering pipeline is viable but demands more bespoke
  work to reach UE5-default quality (GI, volumetric, vegetation density).
- Dragon **flight** across large streaming worlds is a weaker story on
  Unity's HDRP streaming tooling for the team's experience level.
- Runtime-fee/version churn history adds commercial uncertainty.

### Why not Godot 4

- Excellent open-source engine, but reaching "premium realistic cinematic"
  visual parity requires significantly more engine-level work; the art and
  tooling ecosystem for photoreal medieval fantasy is far smaller. Rejected
  for this project's visual bar, not on general principle.

## Consequences

**Positive**

- World-class rendering baseline; the visual target is largely about art
  and scene authoring, not engine engineering.
- Proven architecture patterns for every planned system (GAS for powers,
  perception/StateTree for the dragon companion, World Partition for the
  eventual open world).

**Costs / commitments**

- C++ compile times and a heavier editor — mitigated by strict
  modularization (small, focused modules; minimal interdependencies).
- Binary asset merge conflicts (uasset/umap) — mitigated by Git LFS,
  asset-lock workflow, and one-owner-per-map rules.
- Source-built upgrades require Epic GitHub access; we standardize on
  Launcher builds for the team unless a source build is required.

## Alternatives Considered

1. **Unity 6 + HDRP** — viable, rejected for streaming/rendering parity and
   ecosystem fit for this specific game.
2. **Godot 4.3+** — rejected for visual-target reach and art ecosystem.
3. **Unreal Engine 5.6/5.7** — rejected in favor of 5.8 (same line, newer,
   longest remaining support window).
4. **Waiting for Unreal Engine 6** — rejected: no release date, no feature
   set; shipping is the priority, and 5.8 content migrates forward.

## Decision Record

Decisions are recorded as ADRs in `Docs/decisions/` using this format.
Subsequent ADRs cover module boundaries, GAS adoption, save architecture,
and content-conventions as those systems land.
