# Dragonbound — TODO

> **Status:** Milestone 2 — runtime foundation complete; editor/content
> validation remains.

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

### M2 phone-only preparation

- [x] Git LFS rules cover GLB, Unreal, texture, audio, and animation binaries
- [x] Dragon and rider external-asset folders reserved
- [x] Dragon/rider import contracts documented
- [x] UE 5.8 editor handoff sequence documented in `Docs/M2_PHONE_PREP.md`
- [x] C++ fallback AI follows once M2 bond depth reaches 10; narrative bond stages remain story-driven
- [ ] Place the two exact GLB binaries in their LFS-tracked folders when computer/file access is available

### M2 editor/content queue

- [ ] Add exact `war_dragon_rigged.glb` source to `Content/External/Dragon/` via Git LFS
- [ ] Import dragon GLB and validate 57-joint skeleton, 4 materials, and 7 supplied animations
- [ ] `BP_Dragon_Juvenile`
- [ ] `ABP_Dragon_Juvenile` using supplied Idle/Walk/Flap clips
- [ ] Author Alert/Comforted/Distressed/Protective reaction clips
- [ ] `ST_DragonCompanion` with Idle/Curious/Follow/Protect/React
- [ ] NavMesh + first playable vale scene
- [ ] Mind-speech audio/visual presentation
- [ ] Add exact `adventurer_rigged.glb` source to `Content/External/Human/` via Git LFS
- [ ] Import human GLB and validate 28-joint skeleton, 6 materials, and 4 supplied animations
- [ ] Rider visual Blueprint + `ABP_Rider`
- [ ] Connect Rider Idle/Walk/Run to the existing locomotion context
- [ ] UE 5.8 compile
- [ ] Run `Dragonbound.M2.*` automation tests in-editor
- [ ] M2 full-session exit test

## M3 — Combat Core + Ember

- [ ] GAS wiring and combat pipeline
- [ ] Rider combat
- [ ] Dragon combat AI
- [ ] Ember package + first combined attack
- [ ] Enemy archetypes
- [ ] Combat VFX/audio
- [ ] `SwordSwing` animation activation
- [ ] `Bite` animation activation

## Standing rules

- No system is built before its milestone.
- Every merge compiles; content passes Originality Policy.
- Docs stay synchronized with implementation.
