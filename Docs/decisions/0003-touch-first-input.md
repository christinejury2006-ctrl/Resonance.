# ADR-0003 — Touch-First Input Architecture (M1 revision)

- **Status:** Accepted
- **Date:** 2026-09-13
- **Deciders:** Lead Game-Engineering Architect
- **Amends:** ADR-0002 §5 (input architecture)
- **Implements:** GAME_DESIGN.md §9 (revised), DEVELOPMENT.md M1 (revised)

## Context

Product direction correction during M1: **Dragonbound is phone-first.**
Touch controls are the primary input; keyboard/mouse and gamepad are
secondary development/testing schemes. The correction must not lower the
cinematic visual target — the bar stays "premium cinematic fantasy," with
mobile-conscious architecture beneath it.

The M1 codebase was already compatible: the Rider binds Enhanced Input
actions by identity, and gameplay never inspects an input device. The work
is therefore additive — a virtual-control layer that becomes *another
source* for the same actions, plus mobile-conscious configuration.

## Decisions

### 1. Touch is a source, not a special path

Virtual controls inject into the **same `UInputAction` assets** the
character binds (`IA_Move`, `IA_Look`, `IA_Jump`, `IA_Sprint`,
`IA_Interact`, `IA_TogglePerspective`). Injection uses
`UEnhancedPlayerInput::InjectInputForAction` (empty modifier/trigger
arrays; touch-side scaling is applied before injection). Gameplay code is
device-agnostic by construction: whether a value came from a touchscreen,
gamepad, or keyboard is invisible to `ADBRiderCharacter` and the camera
director. The injection surface is confined to two methods on
`UDBTouchControlLayer` so any API drift is a one-file fix.

### 2. New plugin: DragonboundTouch

`UDBTouchConfig` (Data Asset) — all touch behavior is data: zone
fractions, joystick geometry/dead zone, look sensitivity/inversion,
safe-area insets, button sizing, and the widget class.
`UDBTouchControlLayer` (component on `ADBRiderPlayerController`) —
device detection, config resolution, injection. `UDBTouchControlsWidget`
(UMG) — routing and visuals. The plugin never depends on game code
(config falls back via the game mode, set by the controller/HUD).

### 3. Virtual control scheme (M1)

- **Left zone** (half the viewport): virtual joystick — fixed base
  anchor, thumb tracks the finger, radius-clamped, dead-zone'd, normalized
  to [-1,1]. Screen-up = forward.
- **Right zone**: anywhere-drag look. Delta normalized against a
  reference height (1080) so sensitivity is resolution-independent;
  inversion is a config flag, independent of the KBM modifier chain.
- **Bottom-right cluster (2×2)**: Jump, Sprint (hold), Interact,
  TogglePerspective. Press/release maps 1:1 to the digital actions
  (Started/Completed), so hold semantics (sprint) work unchanged.
- Multi-touch routing by pointer index; buttons take precedence over
  look-zone starts; one joystick + one look drag concurrently.

### 4. Mobile-conscious layout

- All layout is **fractional** against the viewport's smaller dimension
  and the safe-area-inset box — any aspect ratio (16:9 → 21:9) lays out
  correctly; slate units keep it DPI-scale aware.
- **Safe areas** (notches, rounded corners, home indicators) are
  configurable insets applied to all layout math. Native OS notch APIs are
  plumbed when packaging begins; the inset seam is already the config.
- Landscape orientation is the product orientation (documented;
  platform settings land with the packaging milestone).

### 5. Activation & testing

Touch controls activate when: config present + enabled + local player +
touch is primary. Touch-primacy: mobile platforms at runtime, otherwise
`UCommonInputSubsystem` input-type detection. Desktop testing:
`DB.TouchControls.Force 1` (console variable) — mouse simulates touch
(left-drag on the right side = look, left zone = joystick, click buttons).

### 6. Camera tuned for touch

Look sensitivity and drag behavior are touch-config data (drag anywhere,
1:1 tracking, no momentum in M1 — momentum is a tuning addition later).
The existing director smoothing (12/20 Hz interp) plus pitch clamping
keeps touch pans responsive without jitter; perspective toggle remains a
touch button, preserving the ADR-0002 "no context disables the toggle"
contract.

## Consequences

- **Positive:** one input pipeline for all devices; touch tuning is
  data-only; the same camera/movement systems serve phone, gamepad, and
  KBM. Virtual controls are a presentation layer, not a gameplay fork.
- **Costs:** touch injection relies on Enhanced Input's injection API
  (stable since 5.1; single-file fix if it ever changes). Haptics, UI
  feedback polish, and native safe-area plumbing are deferred to the
  mobile packaging milestone.

## Deferred (deliberate)

Native notch/safe-area APIs, haptics, look-drag momentum option, touch
button remapping UI, Android/iOS orientation settings, mobile scalability
preset tuning (M5 performance pass), on-device profiling.
