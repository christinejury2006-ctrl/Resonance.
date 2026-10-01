# Dragonbound — TODO

> **Status:** Milestone 2 — runtime foundation complete; editor/content
> validation remains.
>
> Working queue for the team. Ordered by milestone. Items are checked off
> when complete; editor-only tasks remain until validated in UE 5.8.

## M2 — Dragon Companion & Bond v1

- [x] `ADBDragonCharacter` (juvenile stage placeholder-avatar, M2 quality)
- [x] `UDBDragonVisualComponent` (growth stage + readable visual state)
- [x] `UDBDragonAIController`: perception + StateTree runtime bridge
- [x] `UDBDragonEmotionComponent`: mood model + presentation hooks
- [x] `UDBBondComponent` + `UDBMindLinkComponent`: Awakening/Feeling,
      emotion/sensation/image payloads
- [x] Rider interactions: call, feed, soothe, protect
- [x] Bond-affecting interaction + persistent memory event
- [x] Rider mind-link receiver for feeling/sensation presentation

### M2 editor/content queue

- [ ] `BP_Dragon_Juvenile`
- [ ] Juvenile mesh, material, skeleton/rig, locomotion/reaction animation set
- [ ] `ABP_Dragon_Juvenile` with readable mood states
- [ ] `ST_DragonCompanion` with Idle/Curious/Follow/Protect/React
- [ ] NavMesh + first playable vale scene
- [ ] Mind-speech audio/visual presentation
- [ ] UE 5.8 compile
- [ ] Run `Dragonbound.M2.*` automation tests in-editor
- [ ] M2 full-session exit test

### Architecture decisions

- [ ] ADR-0004 — Dragon AI architecture
- [ ] ADR-0009 — Bond data model
- [ ] Keep ADR-0003 reserved for touch-first input; ADR-0005–0008 remain
      reserved for M3–M4 decisions.

## M1 — Movement, Camera, Rider Shell

M1 runtime code is complete. Remaining editor validation:
- [ ] Touch widget Blueprint + touch config
- [ ] Grey-box map
- [ ] UE 5.8 exit tests (camera switching, movement feel, 60 fps, touch)

## M3 — Combat Core + Ember

- [ ] GAS wiring and combat pipeline
- [ ] Rider combat
- [ ] Dragon combat AI
- [ ] Ember package + first combined attack
- [ ] Enemy archetypes
- [ ] Combat VFX/audio
- [ ] ADR-0005
- [ ] 5-minute combat exit test

## M4 — Story Spine, Academy Intro, Slice Region

- [ ] Dialogue plugin
- [ ] Opening/hatch sequence
- [ ] Words-stage unlock
- [ ] Academy intro
- [ ] Antagonist reveal
- [ ] Slice region art pass
- [ ] ADR-0006/0007/0008

## M5 — Ember Mastery, Showcase, Slice Polish

- [ ] Ember mastery UI
- [ ] Combined-attack showcase
- [ ] Audio/performance/UX pass
- [ ] Slice playtest + post-slice review

## Backlog M6+

- [ ] Flight
- [ ] Tide
- [ ] World production
- [ ] Academy full scale
- [ ] Remaining elements, campaign, adult dragon, Unison

## Standing rules

- No system is built before its milestone.
- Every merge compiles; content passes Originality Policy.
- Docs stay synchronized with implementation.
