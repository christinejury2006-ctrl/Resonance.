# Dragonbound — Development Plan

> **Status:** Living document — M2 runtime foundation complete; editor content
> and on-machine validation remain.
> **Last updated:** 2026-10-01

---

## Development Philosophy

1. **Vertical slice first.** Prove every pillar (bond, cinematic
   presence, teamwork, earned power, free perspective) in one small,
   high-quality space before building anything wide.
2. **Architecture that survives.** Systems are designed for the full game
   even when the milestone only needs part of them.
3. **Quality is non-negotiable.** No placeholder art or "temporary"
   movement feel in the critical path. Every milestone's playable content
   must clear the VISUAL_STYLE.md bar.
4. **No premature systems.** We do not build data structures, UI, or
   plumbing "just in case" — each system lands with the milestone that
   needs it, in its final architectural shape.
5. **Fail fast on the pillars.** The riskiest pillars (bond communication,
   combined combat, free camera switching) are prototyped early.
6. **Phone-first input.** Touch is the primary control scheme; KBM and
   gamepad are secondary development/testing schemes feeding the same
   pipeline (ADR-0003).

---

## Milestone Roadmap

### M0 — Foundation

Foundation complete enough for M1/M2 development; remaining M0 items are
machine/process checks rather than blockers for the runtime architecture.

### M1 — Movement, Camera, and the Rider Shell

M1 runtime code is complete and committed. UE 5.8 editor validation remains
an on-machine gate.

### M2 — The Dragon Companion: Presence & the First Bond

**Goal:** the dragon exists as a *character*, not a follower capsule.

**Runtime foundation delivered**

- [x] Dragon character with juvenile growth-stage/readability hooks.
- [x] Dragon visual-state component.
- [x] Dragon perception + StateTree runtime bridge, with stage-aware
      perception tuning and a C++ fallback brain.
- [x] Emotion/mood model with presentation event hooks.
- [x] Awakening/Feeling bond stages and mind-link payloads for emotions,
      sensations, and images.
- [x] Call, feed, soothe, and protect interactions with dragon reactions.
- [x] Persistent bond memory events and stage advancement from meaningful
      remembered interactions.
- [x] Rider mind-link receiver for feeling/sensation presentation.
- [x] World-less M2 automation smoke tests.

**Remaining editor/content work**

- [ ] Author `BP_Dragon_Juvenile` from `ADBDragonCharacter`.
- [ ] Author juvenile skeletal mesh/material/rig and minimum locomotion set.
- [ ] Author `ABP_Dragon_Juvenile` and mood-driven presentation states.
- [ ] Author `ST_DragonCompanion` with Idle/Curious/Follow/Protect/React.
- [ ] Add NavMesh and the first playable vale test scene.
- [ ] Add placeholder mind-speech audio/visual presentation.
- [ ] Run UE 5.8 compile and `Dragonbound.M2.*` automation tests.
- [ ] Complete the full-session M2 exit test.

**Architecture decisions**

- [ ] ADR-0004 — Dragon AI architecture.
- [ ] ADR-0009 — Bond data model (ADR-0003 is already the touch-first
      input decision; M3–M4 retain ADR-0005–0008 for combat/dialogue/save/world).

### M3 — Combat Core + Ember

**Goal:** Rider combat, dragon combat, and the first elemental power on
the ground, on the Gameplay Ability System.

**Scope**

- GAS wiring: attribute sets, gameplay abilities, gameplay effects, damage
  pipeline.
- Rider combat: light/heavy attacks, dodge, block, lock-on, three enemy
  archetypes.
- Dragon combat AI: attack selection, positioning, command channel.
- Ember as the template element and first combined Rider+dragon attack.
- Difficulty & accessibility option set.
- VFX/audio pass for combat.

**Exit criteria**

- A 5-minute combat encounter in the slice that shows every Ember layer.
- Damage/ability code review against SYSTEMS.md; no one-off combat hacks.
- Decision record: combat/GAS architecture (ADR-0005).

### M4 — Story Spine, Academy Intro, and the Slice Region

**Goal:** the vertical slice becomes a story.

**Scope**

- Opening sequence and hatch.
- Story/dialogue framework.
- Mind-speech words stage unlock.
- Academy introduction.
- Antagonist first revealed move.
- Slice region art pass.

**Exit criteria**

- New player plays opening → hatch → bond → first mystery reveal.
- ADR-0006 dialogue, ADR-0007 save/quest state, ADR-0008 World Partition.

### M5 — Ember Mastery, Combined Attack Showcase, Slice Polish

**Goal:** the Ember vertical slice is complete and shippable-quality.

**Scope**

- Full Ember mastery path.
- Combined-attack showcase encounter.
- First full dragon growth beat if story requires.
- Audio/performance/UX pass.
- Slice playtest loop.

### M6+ — After the Slice

Roadmap only; re-planned after M5.

---

## Branching & Workflow

- **Trunk:** `main` always compiles and runs. Protected.
- **Feature branches:** `feature/<slug>` for systems and content.
- **Content branches:** `content/<slug>` for single-owner binary assets.
- **Commit hygiene:** atomic commits.
- **Milestone flow:** milestone branch → PR → final review → tag.

## Testing & Quality Gates

- **Editor compile gate:** every merge must compile and pass Automation smoke tests.
- **Functional tests (M2+):** AI smoke, combat smoke, camera smoke.
- **Playtests:** milestone builds are played before exit criteria are declared met.
- **Performance gate (M5+):** documented frame budget.
- **Content review gate:** every asset/name is checked against Originality Policy.

## Documentation Cadence

| Document | Updated when |
| --- | --- |
| GAME_DESIGN.md | End of every milestone; decisions |
| SYSTEMS.md | Whenever architecture changes |
| VISUAL_STYLE.md | Art-direction decisions |
| TODO.md | Continuously |
| ADRs | Every architectural decision |

## Risk Register

| Risk | Impact | Mitigation |
| --- | --- | --- |
| Dragon companion feels like a mount with AI | Fails pillar 2/3 | Bond-first milestone order; memory rule |
| Combined attacks feel scripted | Fails combat identity | GAS-native combined abilities |
| Camera switching breaks contexts | Fails pillar 5 | Camera-mode stack architecture |
| Scope creep on slice | Schedule | One region; exit criteria |
| Visual bar unmet | Fails pillar 2 | Marketplace-verified art + strict visual standards |
| UE5.x churn / future UE6 | Rework | Standardize on 5.8 |
| Binary asset conflicts | Lost work | Single-owner content rules + LFS locking |
| Originality violation | Legal/reputational | Originality Policy |
