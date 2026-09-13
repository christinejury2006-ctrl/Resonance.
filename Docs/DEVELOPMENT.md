# Dragonbound — Development Plan

> **Status:** Living document — Milestone 0 (foundation)
> **Last updated:** 2026-09-13
>
> This is the staged development roadmap. Milestone boundaries are chosen
> so that **every milestone ends with a playable build** and every
> milestone's architecture survives the next one without a rewrite.
> Each milestone lists goals, deliverables, exit criteria, and the
> documentation/decision artifacts it produces.

---

## Development Philosophy

1. **Vertical slice first.** Prove every pillar (bond, cinematic
   presence, teamwork, earned power, free perspective) in one small,
   high-quality space before building anything wide.
2. **Architecture that survives.** Systems are designed for the full
   game (flight, seven elements, open world) even when the milestone only
   needs part of them. See SYSTEMS.md.
3. **Quality is non-negotiable.** No placeholder art or "temporary"
   movement feel in the critical path. Every milestone's playable content
   must clear the VISUAL_STYLE.md bar.
4. **No premature systems.** We do not build data structures, UI, or
   plumbing "just in case" — each system lands with the milestone that
   needs it, in its final architectural shape.
5. **Fail fast on the pillars.** The riskiest pillars (bond
   communication, combined combat, free camera switching) are prototyped
   early in the milestones where they first appear.

---

## Milestone Roadmap

### M0 — Foundation (current)

**Goal:** a clean, scalable, documented project skeleton. No gameplay.

**Deliverables**

- [x] Engine decision: Unreal Engine 5.8 (ADR-0001)
- [x] Project skeleton: uproject, Config, Source module, .gitignore,
      .gitattributes (Git LFS)
- [x] Documentation suite: README, GAME_DESIGN, DEVELOPMENT, VISUAL_STYLE,
      SYSTEMS, TODO
- [ ] Remote repository synced with this structure
- [ ] Editor opens and compiles cleanly on a team machine (UE 5.8)

**Exit criteria**

- Fresh clone → engine association prompts once → editor opens → game
  module compiles → PIE runs (empty level).
- Every doc listed in the README map exists and is current.

**No code beyond the empty module is shipped in M0.** First gameplay
classes arrive in M1, in their final architectural shape.

---

### M1 — Movement, Camera, and the Rider Shell

**Goal:** a premium-feeling character controller with free first/third
person switching, and the first playable map.

**Scope**

- Rider character (placeholder-avatar at first; MetaHuman integration
  when art pipeline lands — see VISUAL_STYLE.md) with male/female option
  wired at the asset level.
- Third-person over-shoulder movement: walk/jog/sprint, acceleration
  curves, camera-relative movement, jump.
- First-person movement with body awareness (hands/weapon visible).
- **Instant perspective switching** via Enhanced Input; camera blend
  rules (see SYSTEMS.md → Camera).
- Grey-box test level using the slice's real terrain proportions
  (settlement → wilderness valley → hatching site).
- Controller + mouse/keyboard parity.

**Architecture deliverables**

- [x] `DBRiderCharacter`, `DBRiderPlayerController`, `DBRiderGameMode`
      classes; `DBInputConfig` (Input Mapping Contexts); `DBCameraMode`
      plugin (ViewTarget stack) — per SYSTEMS.md.
- [x] Content namespace established: `Content/Dragonbound/Core`,
      `.../Characters`, `.../Maps`, `.../Data`.

**Exit criteria**

- [ ] 60 fps grey-box run; switching camera mid-sprint/mid-jump is seamless
      with no state loss.
- [ ] Movement feel sign-off by the team (no float, no ice-skating, no
      camera snap).
- [x] Decision record: input/camera architecture (ADR-0002).

> **M1 code status:** complete and committed. The remaining exit criteria
> are on-machine checks (UE 5.8 editor) — procedure in
> [EDITOR_SETUP.md](EDITOR_SETUP.md) §7. Automation tests
> (`Dragonbound.M1.*`) cover camera blend math, movement defaults, and tag
> registration in a world-less environment.

---

### M2 — The Dragon Companion: Presence & the First Bond

**Goal:** the dragon exists as a *character*, not a follower capsule.

**Scope**

- Hatchling/juvenile dragon placeholder-avatar (skeleton + locomotion
  pass; final art later — placeholder must still read as a living
  creature, never as a capsule).
- Dragon AI foundation: **perception** (sight/hearing), companion
  follow with natural offsets, curiosity states, idle life (look-at,
  sniff, investigate, rest). See SYSTEMS.md → Dragon.
- **Bond system v1:** bond state, bond events, stage-gated mind-speech
  — emotions/sensations/images only (no dialogue yet).
- Mind-speech UX: subtle audio/visual layer (direction, intensity),
  image-flash moments.
- Rider ↔ dragon interaction: call, feed, soothe, protect — with
  dragon reactions.
- First bond-affecting choice (prototype the "the dragon remembers"
  rule).

**Exit criteria**

- The dragon reads as alive for a full session: it reacts to the
  player, the environment, and events without scripting per moment.
- A tester can describe the dragon's mood after a session and be
  right.
- Decision record: AI architecture (ADR-0003) + Bond data model
  (ADR-0004).

---

### M3 — Combat Core + Ember (Ground Phase)

**Goal:** Rider combat, dragon combat, and the first elemental power on
the ground, on the Gameplay Ability System.

**Scope**

- GAS wiring: attribute sets (health/stamina), gameplay abilities,
  gameplay effects, damage pipeline (see SYSTEMS.md → Combat).
- Rider combat: light/heavy attacks, dodge, block, lock-on, three
  enemy archetypes (light skirmisher, shielded brute, ranged harrier).
- Dragon combat AI: attack selection, positioning, enemy evaluation,
  command channel (defend/attack/stay) at early bond.
- **Ember** as the template element: dragon abilities (flame breath),
  Rider abilities, Rider combat moves, dragon combat moves, **first
  combined Rider+dragon attack**, and the elemental mastery table.
- Difficulty & accessibility option set (per GAME_DESIGN §8.2).
- VFX/audio pass for combat at VISUAL_STYLE.md quality.

**Exit criteria**

- A 5-minute combat encounter in the slice that shows every Ember
  layer (dragon, Rider, combined) and feels like the target fantasy.
- Damage/ability code review against SYSTEMS.md; no one-off combat
  hacks outside GAS.
- Decision record: combat/GAS architecture (ADR-0005).

---

### M4 — Story Spine, Academy Intro, and the Slice Region

**Goal:** the vertical slice becomes a *story*: opening, bond growth,
and the first shadow of the mystery.

**Scope**

- Opening sequence: ordinary life → discovery → hatch (cinematic
  quality bar; Sequencer + in-game systems).
- Story/dialogue framework (Dialogue plugin from SYSTEMS.md):
  protagonist voice, NPC dialogue, quest/dialogue data assets.
- Mind-speech **words stage** unlock event; first dragon dialogue
  lines (voice: TBD per design).
- Academy introduction (slice-scale: one building, key NPCs, hub
  function).
- The antagonist's first revealed move (identity depth TBD).
- Slice region art pass to VISUAL_STYLE.md bar (Lumen/Nanite
  settings, weather, atmosphere).

**Exit criteria**

- New player plays the slice opening → hatch → bond → first mystery
  reveal in one sitting; no placeholder art in the critical path.
- Decision records: dialogue architecture (ADR-0006), save/quest state
  (ADR-0007), world-partition strategy (ADR-0008).

---

### M5 — Ember Mastery, Combined Attack Showcase, Slice Polish

**Goal:** the Ember vertical slice is complete and shippable-quality.

**Scope**

- Full Ember mastery path (upgrades, techniques, mastery UI).
- Combined-attack showcase encounter (set-piece quality).
- First full dragon growth beat if story requires (juvenile→young
  adult decision point).
- Audio mix pass, performance pass (frame budget, scalability
  settings for lower hardware tiers), controller + keyboard UX pass.
- Slice playtest loop; fix-all-polish-all.

**Exit criteria**

- The slice is demo-ready: every pillar lands in one sitting at
  target quality and frame rate.
- Post-slice review: what carries into the full game, what gets
  rebuilt.

---

### M6+ — After the Slice (roadmap, not commitments)

The slice buys the license to build wide. Order is a **proposal**,
re-planned after the M5 review:

- **M6 — Flight:** takeoff, flight model, landing, aerial combat,
  aerial camera (architecture already specified in SYSTEMS.md).
- **M7 — Second element (Tide),** proving the multi-element template
  and element switching.
- **M8 — World production:** World Partition open region beyond the
  slice; wilderness, first dungeon.
- **M9 — Academy hub + quest content** at full scale.
- **M10+ — Remaining elements (Gale, Storm, Verdance, Umbral,
  Aether), antagonist campaign, dragon growth to adult, Unison bond
  stage, ending.** Element order is story-driven, not a menu.

---

## Branching & Workflow

- **Trunk:** `main` always compiles and runs. Protected.
- **Feature branches:** `feature/<slug>` for systems and content;
  short-lived (target ≤ 1 week); PR required to merge.
- **Content branches:** art/level work uses `content/<slug>`; because
  `uasset`/`umap` cannot merge, content branches are strictly
  single-owner — **one person per map, one person per asset at a time**.
  For larger teams: `git lfs lock` on in-progress assets (see
  `.gitattributes`).
- **Commit hygiene:** atomic commits; message style
  `[System] Imperative summary` (e.g., `[Camera] Add first-person
  shoulder collision`).
- **Milestone flow:** milestone branch `m<N>/<name>` cut from `main`
  when work starts → PR-merge continuously → final review pass → tag
  `v0.<N>`.

## Testing & Quality Gates

- **Editor compile gate:** every merge must compile (editor build) and
  pass `Automation` smoke tests (M1+).
- **Functional tests (M2+):** AI smoke (dragon follows/returns),
  combat smoke (damage pipeline), camera smoke (both perspectives in
  every context).
- **Playtests:** milestone builds are played by the team; findings are
  logged against the milestone before exit criteria are declared met.
- **Performance gate (M5+):** frame budget (target: 60 fps on
  reference PC at High; 30 fps console profile), documented in the
  milestone PR.
- **Content review gate:** every asset/name is checked against the
  Originality Policy (GAME_DESIGN.md §2) — blocking if violated.

## Documentation Cadence

| Document | Updated when |
| --- | --- |
| GAME_DESIGN.md | End of every milestone; any time a "TBD" becomes decided |
| SYSTEMS.md | Whenever architecture changes; must match the code at each merge |
| VISUAL_STYLE.md | When art direction decisions are made or standards change |
| TODO.md | Continuously — it is the team's working queue |
| ADRs (Docs/decisions/) | Every architectural decision (numbered, dated, status-tracked) |

## Risk Register

| Risk | Impact | Mitigation |
| --- | --- | --- |
| Dragon companion feels like a mount with AI | Fails pillar 2/3 | Bond-first milestone order (M2); "the dragon remembers" rule; no mount-only features |
| Combined attacks feel scripted | Fails combat identity | GAS-native combined abilities from M3; no bespoke quick-time-event combat |
| Camera switching breaks contexts | Fails pillar 5 | Camera-mode stack architecture from M1; context switch tests in every milestone |
| Scope creep on the slice | Schedule | One small region; exit criteria per milestone; "no" to non-pillar features |
| Visual bar unmet with small team | Fails pillar 2 | Marketplace-verified art + MetaHuman + strict VISUAL_STYLE standards; UE5 baseline rendering |
| UE5.x churn / future UE6 | Rework | Standardized on 5.8 (last major UE5 line); track UE6, migrate only with a plan (ADR) |
| Binary asset conflicts | Lost work | Single-owner content rules + Git LFS locking |
| Originality violation slips in | Legal/reputational | Originality Policy in design doc; blocking content review; name search before commit |
