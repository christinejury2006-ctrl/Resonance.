# Dragonbound — Editor Setup (M1)

> **Who:** anyone opening the project for the first time (or after a clean clone).
> **When:** before playing M1.
> **Prereqs:** Unreal Engine 5.8, project cloned, editor compiles
> (README → Getting Started).

M1's C++ is complete, but five things are **authored in-editor** (they are
content, not code): the input config + actions, the touch config + widget,
the two appearance definitions, the grey-box map, and the game-mode wiring.
This guide covers all of them. ~25 minutes total.

---

## 1. Folder structure

In the Content Browser, create the M1 namespace (see README → Project
Structure):

```
Content/Dragonbound/
├── Core/
├── Characters/
├── Maps/
└── Data/
```

## 2. Input actions + mapping context (Enhanced Input)

Create these in `Content/Dragonbound/Data/Input/`:

**Input Actions** (right-click → Input → Input Action):

| Asset | Value Type | Modifiers |
| --- | --- | --- |
| `IA_Move` | Axis2D (Vector2D) | none |
| `IA_Look` | Axis2D (Vector2D) | Negate: bX=false, **bY=true**, bZ=false (mouse/stick up = look up) |
| `IA_Jump` | Digital (bool) | none |
| `IA_Sprint` | Digital (bool) | none |
| `IA_TogglePerspective` | Digital (bool) | none |
| `IA_Interact` | Digital (bool) | none |

**Mapping Context** `IMC_OnFoot` — add mappings:

| Action | Keyboard/Mouse | Gamepad |
| --- | --- | --- |
| IA_Move | W/A/S/D | Left Stick (Y = forward axis) |
| IA_Look | Mouse XY | Right Stick XY |
| IA_Jump | Space | Face Button Bottom |
| IA_Sprint | Left Shift | Face Button Left |
| IA_TogglePerspective | V | Face Button Right |
| IA_Interact | E | Face Button Top |

## 3. Input config data asset

Create `DB_InputConfig` (Miscellaneous → Data Asset → `UDBInputConfig`) in
`Content/Dragonbound/Data/`, assign all six actions and add `IMC_OnFoot` to
**On Foot Mapping Contexts**.

## 4. Appearance definitions (male/female Rider)

Create two `UDBRiderAppearanceDefinition` primary data assets in
`Content/Dragonbound/Data/Appearance/`:

| Asset | Sex | Body Mesh | Anim Instance |
| --- | --- | --- | --- |
| `DB_Appearance_Male` | Male | `SKM_Manny` (engine mannequin) | `ABP_Manny` |
| `DB_Appearance_Female` | Female | `SKM_Quinn` | `ABP_Manny` (retargeted) |

> Find the mannequins in the Content Browser by searching "Manny" / "Quinn"
> with **View Options → Show Engine Content** enabled. M1 uses placeholders;
> MetaHuman bodies replace these references later — the data asset is the
> swap point (VISUAL_STYLE.md §2.2).

## 5. Game mode wiring

Either:
- **Blueprint subclass:** `BP_DBRiderGameMode` (parent `ADBRiderGameMode`),
  set `DefaultInputConfig = DB_InputConfig` and
  `DefaultAppearanceDefinition = DB_Appearance_Male` (switch per playtest),
  then set it as the map's **World Settings → GameMode Override**; or
- **Direct:** set the C++ class in World Settings and leave the defaults
  (no input until configured — expect warnings).

## 6. Grey-box map — `DB_Map_M1Greybox` (`Content/Dragonbound/Maps/`)

Create a new level. Use **grey materials only** — matte value/saturation
differences are allowed, but no textures, no marketplace art. M1 is about
scale, composition, lighting direction, and gameplay space (VISUAL_STYLE.md
§1.2: grey-boxing is fine in the grey-box map only).

**Terrain:** Landscape (suggested 505×505, ~1 m/section) for the valley
floor, or BSP if preferred. Keep it editable — it will be replaced by the
art pass in M4.

**Required composition (from DEVELOPMENT.md M1 + GAME_DESIGN.md §4):**

1. **Starting settlement clearing** — flat, ~40×40 m, at the map's lower
   edge; 3–5 simple house masses (cubes with roof prisms) for scale.
2. **Wilderness valley** — the main stretch, ~300 m long, gently rising;
   a stream cutting across it.
3. **Cliffs / rock formations** — the valley's sides, 30–50 m tall,
   irregular; use simple rock-shaped meshes (box/cones) or sculpted
   landscape. Leave climbable ledges — verticality matters.
4. **Waterfall + pool** — ~20 m drop where the stream enters; feed the
   stream below; a simple white-blue material on the fall plane is enough.
5. **Mist/fog** — ExponentialHeightFog with a low, dense bottom band;
   optional Niagara sprite sheet for drifting fog volumes (Niagara plugin
   is enabled).
6. **Large ancient trees** — 25–40 m tall scale references (cylinder
   trunk + cone/sphere canopy clusters). Scale is the point: these must
   read as *ancient*.
7. **Ancient stone / ruin elements** — fallen columns, broken arch
   fragments, a ring of standing stones.
8. **Central landmark — the future Dragon Sanctuary** — a hollow hill or
   elevated stone circle at the valley's far end, on the highest ground.
   This will become the sanctuary in later milestones; it must be the
   composition's focal point.
9. **Open sky space for future flight** — a large clear bowl beyond the
   sanctuary (no tall obstacles), so the flight milestone has room.

**Lighting direction:** one DirectionalLight at a low morning angle
(casting long shadows up-valley), SkyLight, the fog volume. This lighting
rig becomes the slice's exterior-day reference (VISUAL_STYLE.md §2.3).

**One PlayerStart** at the settlement edge facing up-valley.

## 7. Smoke test (M1 exit criteria — DEVELOPMENT.md)

1. PIE: Rider spawns with the configured appearance.
2. WASD + mouse: camera-relative movement; sprint (Shift) works; jump +
   air steering feel weighted.
3. V toggles first/third person mid-sprint and mid-jump: ≤250 ms crossfade,
   no state reset, no camera snap, body hides in first person.
4. Walk the camera into a cliff/ruin: third-person camera pulls in without
   clipping; first-person stays out of walls.
5. Gamepad: movement/look/jump/sprint/toggle all mapped.
6. Frame rate: 60 fps in the grey box (reference PC).

## 8. Run the automation tests (optional)

```
UnrealEditor-Cmd.exe <ProjectPath>\Dragonbound.uproject -ExecCmds="Automation RunTests Dragonbound.M1; Quit"
```

All `Dragonbound.M1.*` tests pass with zero content authored.

---

## 9. Touch controls (phone-first — ADR-0003)

Touch is the **primary** input; KBM/gamepad are secondary schemes that
feed the same actions. The virtual controls are data + one widget
Blueprint.

### 9.1 Widget Blueprint — `WBP_TouchControls`

1. Create a **Widget Blueprint** in `Content/Dragonbound/UI/` with parent
   class `UDBTouchControlsWidget`.
2. Root widget: **Canvas Panel** (required for layout).
3. Add these **Image** children with these exact names (the C++ layer
   looks them up by name; missing ones simply aren't drawn — touch routing
   still works):

| Child (UImage) | Purpose |
| --- | --- |
| `JoystickBase` | Left virtual joystick base circle |
| `JoystickThumb` | Joystick thumb (positions itself) |
| `LookHint` | Faint right-side look-zone hint (optional) |
| `ButtonJump` | Bottom-right cluster |
| `ButtonSprint` | Bottom-right cluster |
| `ButtonInteract` | Bottom-right cluster |
| `ButtonCamera` | Camera toggle (first/third person) |

Use simple circles/rounded shapes with translucent white — placeholders
only (VISUAL_STYLE: no final-art investment in M1).

### 9.2 Touch config data asset — `DB_TouchConfig`

Create a `UDBTouchConfig` data asset in `Content/Dragonbound/Data/` and
assign:

- **Actions:** the same six `IA_*` assets from §2 (this is the shared
  pipeline — do not create new actions).
- **Controls Widget Class:** `WBP_TouchControls`.
- Defaults (zones, joystick geometry, dead zone, sensitivity, safe-area
  insets, button sizes) are sane — retune after playtesting.

### 9.3 Game mode wiring

On `BP_DBRiderGameMode` (or the C++ game mode):
`DefaultTouchConfig = DB_TouchConfig`.

### 9.4 Testing on desktop

The layer auto-activates on phones. On desktop, force it:

1. PIE → open console (`~`) → `DB.TouchControls.Force 1`.
2. The mouse now simulates touch: **left-drag on the right half = look**,
   **left half = joystick**, **click the button cluster = press**.
3. Landscape is the product orientation (platform orientation settings
   are set with the mobile packaging milestone).

### 9.5 Touch smoke test (M1 exit criterion)

- Joystick: 1:1 feel, dead zone forgiving, sprint button + joystick
  together work.
- Drag-look: no jitter, sensitivity sensible, drag-up = look-up.
- Buttons: jump/sprint(hold)/interact/camera all register; camera toggle
  switches perspective mid-sprint without state loss.
- Resize the PIE window (and try a narrow aspect ratio): layout stays in
  the safe area, buttons stay reachable.
