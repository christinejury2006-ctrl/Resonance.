# Dragonbound — Visual Style & Quality Requirements

> **Status:** Living document — Milestone 0 (foundation)
> **Last updated:** 2026-09-13
>
> This document defines the visual target, the technical bar, and the
> standards every piece of content must meet. It is the reference for all
> art direction, lighting, VFX, animation, and environment work. When a
> milestone ships, its content is judged against this document.

---

## 1. Visual Direction

**Target:** *cinematic, realistic 3D fantasy.* A grounded, believable
world — medieval in technology, fantastical in its creatures and magic —
photographed like a film.

The mood reference: **earned wonder.** The world is beautiful and lived-in,
with moments of awe (first dragon flight, elemental reveals) that are
*presented cinematically*, never cheaply.

### 1.1 Explicit targets

- Believable, high-quality **human characters** (MetaHuman-quality facial
  fidelity for hero characters).
- **Dragons that feel like animals**: weight, breath, muscle, eye
  movement, natural locomotion — realism of *behavior and materials* over
  realism of anatomy.
- Materials that hold up in close-up: skin, cloth, leather, metal, stone,
  scale, and feather micro-detail via Nanite/high-quality LODs.
- Environments with a sense of place: layered composition, set dressing,
  environmental storytelling everywhere the player looks.
- **Dynamic lighting** as the primary mood tool: time of day, weather,
  interiors lit by fire and candle.
- **Atmosphere**: volumetric light shafts, fog, dust, weather particles —
  the air itself is part of the scene.
- VFX that read **physically grounded** (fire burns, water wets, wind
  moves cloth and leaves) with magic layered on top — magic should feel
  like a force *in* the world, not a sprite attached to it.
- Animation that is **natural and reactive**: weight shifts, turns,
  secondary motion; no sliding feet, no weightless turns.
- **Cinematic camera work**: motivated framing, depth of field, camera
  shake with purpose, smooth moves. Even gameplay cameras are "shot", not
  just positioned.

### 1.2 Explicitly rejected

- Mobile-game presentation: flat lighting, low-poly silhouettes for hero
  assets, UI-forward composition, sprite-based magic everywhere.
- Stylized-chibi/cartoon proportions for characters or the dragon.
- Neon/over-saturated elemental effects. Elemental color is strong but
  must sit *inside* a realistic lighting model.
- Placeholder grey capsules or default mannequins in any
  milestone-reviewed scene (grey-boxing is fine in the grey-box map only).

---

## 2. Technical Quality Bar

### 2.1 Rendering baseline (UE 5.8)

- **Lumen** global illumination and reflections: **on** (default quality
  target). Hardware Lumen where supported; software fallback verified for
  lower tiers.
- **Nanite**: enabled for static world geometry and hero meshes that
  benefit; traditional LODs where Nanite is inappropriate (skeletal
  meshes, masked foliage early on).
- **Virtual Shadow Maps** for directional light.
- **Temporal Super Resolution (TSR)** as the upscaler; alternatives
  verified per platform.
- Tone mapper, color grading, and post-process defined by the **scene
  bible** (§5) — never per-map random settings.
- 60 fps target on the reference PC (High); **scalability settings**
  (Low→Epic) tuned so lower hardware tiers stay playable without
  betraying the art direction (see §6).

### 2.2 Model & material standards

| Asset class | Standard |
| --- | --- |
| Hero characters | MetaHuman pipeline (face/body); unique wardrobe per culture; 4K UDIM textures where the camera gets close |
| The dragon | Hero asset, highest priority in the project. Multi-UDIM (8K master), subsurface on membranes, anisotropic scale sheen; individual scale cards or displacement at close range; robust rig (see SYSTEMS.md → Dragon) |
| Creatures/enemies | Skeletal mesh + physics-driven cloth/fur where applicable; readable silhouettes at combat distance |
| Environment | Nanite static meshes; material functions shared (world blend, moss, snow, damage); trim sheets and modular kits for architecture; photogrammetry/scan-quality texture sets where affordable |
| VFX | Niagara; GPU particles where sensible; no visible sprite edge-on artifacts; VFX reacts to world (collision, wind, wetness) |

### 2.3 Lighting & atmosphere standards

- One **lighting rig per scene class** (exterior day, exterior dusk,
  interior fire-lit, cave, storm) defined in the scene bible; levels
  instance it rather than reauthoring.
- Local exposition is **motivated** (windows, fires, torches, dragon
  glow) — not invisible fill light.
- Weather states (clear, overcast, rain, fog, storm) are real systems
  (sky, particles, ground wetness, wind) — not a skybox swap.
- Night scenes must remain readable; moonlight + firelight design is
  planned, not discovered.

### 2.4 Animation standards

- Human locomotion: motion-capture-based or MetaHuman animation base;
  blend spaces with proper foot-planting (Motion Warping); no sliding.
- Dragon locomotion: quadruped walk/run with weight, wing fold/unfold
  states, takeoff/landing cycles — treated as a signature-quality asset
  from M2.
- Face/emotion: MetaHuman facial animation; the dragon's emotional
  range uses eye, head, posture, and vocalization — a readable face is a
  core companion requirement.
- In-game + cinematic animation share the same rig and locomotion set
  (no "cinematic-only" versions that break identity).

---

## 3. Art Direction per Domain

### 3.1 Characters

- **Human cultures** (slice: the Vale people — name TBD): grounded
  medieval fantasy dress, distinct silhouette per role (villager, guard,
  scholar, Academy figure). Material story: wool, linen, leather, light
  mail for guards. Palette per culture, never neon.
- **The Rider**: player's avatar, male/female, customizable at
  MetaHuman level (face, hair, build); default wardrobe designed to look
  right in both first-person (hands, sleeves) and third-person.
- **Enemies** (slice's three archetypes): silhouette-first design —
  readable at combat distance; damage/armor states readable in motion.

### 3.2 The Dragon

The single most important visual asset in the game. Requirements:

- **Believability:** quadruped anatomy with weight; natural gait; wings
  that fold/rest plausibly; eyes with life.
- **Material identity:** layered scale pattern (hero scales, fine
  scales, soft belly plates); subsurface glow *only* where magic demands
  it; dirt/wear from living in the world.
- **Growth stages** (hatchling → juvenile → young adult → adult) are
  **distinct silhouettes**, not uniform scale-ups. Each stage is its own
  designed model; identity continuity via shared palette, scale pattern
  motifs, and eye color.
- **Elemental states** (Ember first): breath glow, scale heat-vents,
  ember motes — power must be visible *in* the dragon's body, not only in
  projectiles.
- The dragon never reads as a "mount model with a rider saddle slapped
  on." Riding tack (if any) is designed as part of the dragon's wardrobe
  language and story-justified.

### 3.3 Environments

- **Slice region**: one vale — starting settlement (small, human,
  lived-in), wilderness valley (forest, stream, cliffs), hatching site
  (ancient, partially ruined, *charged*). Verticality everywhere
  (climbable cliffs, ruin ledges) because flight comes later.
- **Full world (later)**: each kingdom/culture gets an architectural
  identity kit (materials, trim, props) before any level production;
  biomes follow the same rule.
- **Environmental storytelling rule:** every location answers
  "who lived here and what happened?" with props, wear, and layout —
  before any NPC says a word.
- Weather and time-of-day are **designed into** locations (a vale that
  only works at noon is not finished).

### 3.4 VFX Language

- **Elemental identity:** each element has a canonical visual language
  (color, motion, light behavior, sound) defined in its own mini-style
  page before production (Ember first, at M3).
- Magic reads as **force + light + matter**: Ember is not "red
  particles" — it is heat shimmer, ignition, ember trails, smoke, and
  light thrown on the world.
- **Combined attacks** get the most care: the moment Rider and dragon
  channel together must look like the game's signature.
- All VFX pass the **world-interaction test**: they cast/respond to
  light, collide with geometry, and affect the environment (scorch
  marks, wet surfaces, wind-blown foliage).

---

## 4. UI / Presentation

- Diegetic-first HUD: minimal screen chrome; state communicated in-world
  where possible (dragon's posture, weapon stance, environmental cues).
- Menu/typography: an original fantasy identity (custom font direction:
  TBD at M2), consistent with the world's material language (parchment,
  metal, leather) **without** copying any existing franchise's look.
- Mind-speech presentation (M2): a distinct, tasteful audio/visual
  signature — not generic subtitles.

---

## 5. Scene Bible (process)

Before any environment or cinematic goes into production, it gets a
**one-page scene bible**: intent, time of day, weather, palette, lighting
rig reference, atmosphere, reference stills. The bible is checked in with
the scene. This keeps the whole project visually coherent with a small
team. *(First bibles: slice region (M1 grey-box → M4 art pass), opening
sequence (M4).)*

---

## 6. Performance & Scalability

- **Reference PC (High):** 60 fps in the slice with Lumen/Nanite on.
- **Scalability ladder:** Epic → High → Medium → Low tuned so Medium/Low
  remain true to art direction (darker scenes stay readable; VFX keep
  silhouettes; LOD pops are masked).
- **Console profiles** (later): 30 fps quality / 60 fps performance via
  Device Profiles; TSR per profile.
- Performance budgets are **per scene class** (exterior, interior,
  combat arena, storm), tracked in the milestone PR; regressions block
  merges (M5+ gate).
- Nanite/Lumen **disallowed** where they cost more than they return
  (e.g., foliage may prefer traditional pipeline + wind; measured, not
  assumed).

---

## 7. Asset Sourcing Rules

- **Marketplace content is welcome** for environment kits, foliage,
  props, VFX, and animation packs — it is how a small team reaches this
  bar fast. Rules:
  1. Verifiable license; pack name/version recorded in the asset
     manifest.
  2. Must pass the Originality Policy (GAME_DESIGN.md §2) — no
     franchise-tied or "inspired-by-<franchise>" content.
  3. Must be reworked to Dragonbound identity (materials, palette,
     set dressing) — never dropped in raw.
- **Hero assets are custom**: the dragon, the Rider, key NPCs, and the
  antagonist are original work, full stop.
- Marketplace content is installed to `Content/External/` (git-ignored,
  reinstallable); migrated into `Content/Dragonbound/` only when modified
  or renamed. See README → Project Structure.

---

## 8. Sign-off Criteria (every milestone with visual content)

1. Content meets §1 targets (no mobile-presentation tells).
2. Meets §2 technical standards (models, materials, lighting, animation).
3. Passes Originality Policy review.
4. Passes the scene bible (if applicable).
5. Hits the milestone's frame budget.
