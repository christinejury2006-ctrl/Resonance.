# ADR-0004 — Dragon AI Architecture

- **Status:** Accepted
- **Date:** 2026-10-01
- **Scope:** M2 Dragon Companion

## Context

The dragon must behave as a character rather than a scripted follower. M2
needs perception, curiosity, following, protective reactions, and a stable
extension point for authored StateTree behavior without putting the complete
AI brain into hard-coded C++.

## Decision

Use `ADBDragonAIController` as the runtime bridge between the dragon pawn,
Unreal perception, and an editor-authored StateTree.

### Runtime responsibilities

- AI Perception provides sight/hearing stimuli.
- Perception ranges are tuned from the dragon's growth stage.
- The controller owns the current focus actor and high-level companion
  state: Idle, Curious, Follow, Protect, and React.
- Interaction events such as Call and Protect can explicitly direct the
  controller's focus/reaction.
- A small C++ fallback brain provides usable companion behavior when no
  StateTree asset has been authored yet.

### StateTree responsibilities

When `DragonStateTree` is configured, the controller starts the StateTree
and does not run the fallback decision loop concurrently. This keeps the
runtime architecture deterministic: authored StateTree logic owns behavior,
while C++ remains the integration and fallback layer.

### Growth-stage awareness

Perception distance scales with Hatchling, Juvenile, Young Adult, and Adult
stages. The AI therefore has a single architecture that can survive dragon
growth without creating separate controllers.

## Consequences

- Designers can author and tune behavior in StateTree without rewriting the
  controller.
- The C++ layer remains testable without requiring binary StateTree assets.
- The M2 runtime foundation can exist before the final editor-authored AI
  graph is committed.
- Natural idle-life behavior still requires authored StateTree tasks/animation
  content and is therefore part of the M2 editor/content exit gate.

## Out of scope

Combat decision-making, flight AI, advanced command channels, enemies, and
adult-dragon behavior are later milestones.
