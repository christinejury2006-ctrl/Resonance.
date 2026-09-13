# ADR-0002 — Camera & Input Architecture (M1)

- **Status:** Accepted (amended by ADR-0003 — touch-first input)
- **Date:** 2026-09-13
- **Deciders:** Lead Game-Engineering Architect
- **Implements:** Docs/SYSTEMS.md §3 (Camera), §1 (Character), DEVELOPMENT.md M1

## Context

M1 delivers the Rider foundation: third-person and first-person movement,
free perspective switching in every gameplay context, and input foundations
that survive Mounted/Flying states (M6+) without a rewrite. Key design
tension: the toggle must be seamless (≤250 ms, no locomotion-state reset)
and the architecture must route future contexts (combat lock-on, flight)
without touching the blend core.

## Decisions

### 1. The camera lives on the PlayerController, not the character

A `UDBCameraDirectorComponent` owned by `ADBRiderPlayerController` is the
single source of truth for the player camera. The character never moves or
rotates a camera; it only supplies the view target (position, control
rotation, eye sockets). The view output is published by
`ADBPlayerCameraManager` (override of `UpdateViewTarget`), which reads the
director's blended POV. A `UCameraComponent` on the controller mirrors the
POV for post-process, effects, and interaction traces.

*Why:* one camera follows every pawn (Rider on foot now; mounted/flying
Rider on the dragon later). Perspective switching and context changes are
controller concerns, not character concerns.

### 2. Camera modes are stateless POV calculators blended by the director

- `UDBCameraMode` (base) computes a desired `FDBPOV` (location, rotation,
  FOV) each frame from a `FDBPOVContext` (view target, control rotation,
  delta time). Modes expose tuning (FOV, pitch limits, pivot offset, probe
  radius, smoothing speeds) — authored/retuned in Blueprint subclasses.
- `UDBCameraMode_ThirdPerson`: over-shoulder orbit with a sphere probe
  (`ECC_Camera`) from pivot to camera.
- `UDBCameraMode_FirstPerson`: eye-socket placement (skeletal "head" socket
  with capsule-height fallback) plus a near-wall micro-probe.
- `UDBCameraDirectorComponent` owns mode instances, blends POVs (location
  lerp, quaternion slerp, FOV lerp) over `PerspectiveBlendTime = 0.25s`,
  applies collision to the desired POV *before* steady-state smoothing
  (walls hard-stop; smoothing eases recovery), and initializes directly on
  the first frame (no blend from world origin).

*Why:* new contexts = new mode classes (Combat, Flight, Dialogue) registered
as per-context pairs — zero changes to blend/director code.

### 3. Locomotion contexts route everything

`ADBRiderCharacter::SetLocomotionContext(FGameplayTag)` broadcasts
`DB.Locomotion.*` tags (`OnFoot` active in M1; `Mounted`/`Flying` reserved
for M6). The director maps `TMap<FGameplayTag, FDBPerspectiveModePair>` —
context → {third-person mode, first-person mode} — falling back to default
classes. Mounted/Flying states later call the same setter; camera framing,
input contexts, and (M2+) AI/UI listeners react through the same tag. The
character also mirrors context to any input-context switcher (Enhanced Input
mapping contexts are per-context in `UDBInputConfig`).

### 4. One toggle input, no context may disable it

`IA_TogglePerspective` (Enhanced Input) calls
`Director->TogglePerspective()`. Switching never resets locomotion, ability,
or animation state; the blend starts from the current POV, so mid-sprint and
mid-jump toggles are continuous. Re-toggling mid-blend reverses smoothly.
`IDBPerspectiveAware` lets view targets react: the Rider hides its full-body
mesh in first person (`OwnerNoSee`, preserving self-shadowing) and can show
a first-person mesh override (`GetFirstPersonMesh()` — asset supplied in a
later milestone).

### 5. Input: Enhanced Input only, bound by identity, authored as data

`UDBInputConfig` (Data Asset) lists mapping contexts and six actions (Move,
Look, Jump, Sprint, TogglePerspective, Interact). C++ binds actions **by
identity**; keys, gamepad mappings, and modifiers live in the asset — so
remapping and scheme changes never require code. The character resolves its
config from itself, falling back to `ADBRiderGameMode::DefaultInputConfig`.
`DefaultInput.ini` pins Enhanced Input classes project-wide (set in M0).
InteractAction is declared but consumed by `UDBInteractionComponent`
(10 Hz camera-forward sphere trace targeting `IDBInteractable`) — the
dragon interaction in M2 is an ordinary interaction target.

### 6. Class naming (canonical, replaces earlier doc shorthand)

| SYSTEMS.md/DEVELOPMENT.md reference | Canonical class (M1) |
| --- | --- |
| `DBRiderCharacter` / "ADBPlayerController" | `ADBRiderCharacter`, `ADBRiderPlayerController` |
| `DBInputConfig` | `UDBInputConfig` |
| `DBCameraMode` (ViewTarget stack) | `UDBCameraMode` family + `UDBCameraDirectorComponent` (camera plugin `DragonboundCamera`) |
| `DBRiderGameMode` | `ADBRiderGameMode` |

## Consequences

- **Positive:** Mounted/Flying (M6) add mode classes + context entries +
  input contexts — no core rewrite. Interaction foundation is ready for the
  dragon (M2). Male/female Rider is an asset-level switch
  (`UDBRiderAppearanceDefinition`), MetaHuman-ready.
- **Costs:** Controller-owned camera means all camera querying must go
  through the director (enforced by convention, not compiler). The director
  tick runs `TG_PrePhysics`; camera-manager reads are one frame behind —
  acceptable for M1, revisited if cinematic sequencing (M4) needs
  sub-frame precision.

## Content authored in-editor (not in repo)

`UDBInputConfig` asset, input mapping context, six input actions, two
appearance definitions, and the grey-box map are authored per
[Docs/EDITOR_SETUP.md](../EDITOR_SETUP.md). C++ fails loudly but runs
without them (tests are world-less by design).
