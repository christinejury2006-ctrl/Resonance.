# Dragonbound — TODO

> **Status:** Milestone 0 (foundation) — updated 2026-09-13
>
> Working queue for the team. Ordered by milestone. Mark items `[x]` when
> done, delete them at each milestone review, and keep the next milestone
> fully specified at the top.
>
> **Rule:** TODO items reference the doc that specifies them
> (SYSTEMS.md class names, DEVELOPMENT.md exit criteria). If an item isn't
> specified yet, the task is "specify it first."

---

## NOW — Milestone 0 completion

### Repository & engine

- [x] Push this foundation to `https://github.com/christinejury2006-ctrl/Resonance..git` (repo name ends with a dot: `Resonance.`; pushed via deploy key; no repository created or renamed)
- [ ] `git lfs install` on every team machine; verify `.gitattributes` LFS rules
- [ ] Open the project in Unreal Engine 5.8 on a team machine; confirm the
      empty `Dragonbound` module compiles and PIE runs in an empty level
- [ ] Set up CI (optional but recommended): editor build + compile gate on PR
      (GitHub Actions with UE runner)
- [ ] Add `CONTRIBUTING.md` with code style and commit conventions (needed
      before M1 code reviews)
- [ ] Confirm UE 5.8 version alignment across the team (Epic launcher
      installs, exact point release) — pin it in README if a specific
      patch release matters

### Design review (before M1 starts)

- [ ] Team read-through of GAME_DESIGN.md / SYSTEMS.md / VISUAL_STYLE.md —
      collect objections as issues, resolve or defer
- [ ] Decide slice region name ("the Vale" working name) — original name,
      Originality Policy check (GAME_DESIGN.md §2)
- [ ] Assign milestone owners: M1 (engineering), M2 (AI + bond), M3
      (combat + GAS), M4 (story + world art)

---

## Milestone 1 — Movement, Camera, Rider Shell

*(Classes per SYSTEMS.md §1 & §3; exit criteria per DEVELOPMENT.md M1;
architecture per ADR-0002)*

- [x] Create `DragonboundCamera` plugin module (mode stack, base mode)
- [x] `ADBRiderPlayerController` + `UDBCameraDirectorComponent`: ThirdPerson and
      FirstPerson modes, toggle input, ≤250 ms crossfade through head position
- [x] `ADBRiderCharacter` + `UDBRiderMovementComponent`: walk/jog/sprint,
      acceleration curves, camera-relative input, jump/land
- [x] First-person body awareness (body-hide hook + FP mesh override hook;
      FP arm mesh asset supplied with the art milestone)
- [x] Male/female Rider option wired at asset level (`UDBRiderAppearanceDefinition`;
      placeholder avatars per Docs/EDITOR_SETUP.md)
- [x] `UDBInputConfig` data assets + Input Mapping Contexts (OnFoot;
      controller + KBM parity; authored in-editor per EDITOR_SETUP.md)
- [ ] Grey-box map authored in-editor (composition spec in Docs/EDITOR_SETUP.md §6)
- [x] ADR-0002: input/camera architecture
- [x] Automation tests added (`Dragonbound.M1.*`, world-less)
- [ ] Exit tests on a UE 5.8 machine: seamless switching mid-sprint/mid-jump;
      60 fps grey-box; movement feel sign-off (EDITOR_SETUP.md §7)

---

## Milestone 2 — Dragon Companion & Bond v1

*(Classes per SYSTEMS.md §2 & §6)*

- [ ] `ADBDragonCharacter` (juvenile stage placeholder-avatar, M2 quality)
- [ ] `UDBDragonAIController`: perception + StateTree brain (follow,
      curiosity, idle life, protect)
- [ ] `UDBDragonEmotionComponent`: mood model + animation/vocal expression
- [ ] `UDBBondComponent` + `UDBMindLinkComponent`: stages Awakening/Feeling;
      emotion/sensation/image payloads; mind-speech audio/visual signature
- [ ] Rider interactions: call, feed, soothe, protect — dragon reactions
- [ ] First bond-affecting choice + memory event ("the dragon remembers")
- [ ] ADR-0003 (AI architecture), ADR-0004 (bond data model)
- [ ] Exit test: dragon reads as alive for a full session; mood is readable

---

## Milestone 3 — Combat Core + Ember

*(Classes per SYSTEMS.md §4 & §5)*

- [ ] GAS wiring: `UDBAbilitySystemComponent`, attribute sets, gameplay
      effects, damage pipeline, `UDBCombatComponent`
- [ ] Rider combat: light/heavy, dodge, block, lock-on
- [ ] Dragon combat AI: attack selection, positioning, command channel
- [ ] Ember package (all six layers incl. first combined attack,
      `UDBCombinedAbility` protocol)
- [ ] `UDBElementalPowerDefinition` (Ember) + `UDBMasteryComponent` +
      mastery table + `UDBProgressionComponent` + `UDBSaveGame` v1
- [ ] Three enemy archetypes (data-driven)
- [ ] Combat VFX/audio at VISUAL_STYLE bar; difficulty/accessibility options
- [ ] ADR-0005 (combat/GAS)
- [ ] Exit test: 5-minute slice encounter showing all Ember layers

---

## Milestone 4 — Story Spine, Academy Intro, Slice Region

- [ ] `DragonboundDialogue` plugin: `UDBDialogueAsset` graph, runtime,
      mind-speech speaker channel, CommonUI widget
- [ ] Opening sequence (ordinary life → discovery → hatch) at cinematic bar
- [ ] Words-stage unlock event; first dragon dialogue lines
- [ ] Academy introduction (slice-scale hub) + key NPCs
- [ ] Antagonist first reveal; slice region art pass (Lumen/Nanite,
      weather, atmosphere; scene bibles)
- [ ] ADR-0006 (dialogue), ADR-0007 (save/quest state), ADR-0008
      (World Partition strategy)

---

## Milestone 5 — Ember Mastery, Showcase, Slice Polish

- [ ] Full Ember mastery UI + upgrade path
- [ ] Combined-attack showcase encounter (set-piece)
- [ ] Audio mix, performance/scalability pass, UX pass
- [ ] Slice playtest loop + fix-all
- [ ] Post-slice review → re-plan M6+ roadmap in DEVELOPMENT.md

---

## Backlog (post-slice, M6+ — roadmap only, see DEVELOPMENT.md)

- [ ] M6 Flight system (takeoff, model, landing, aerial combat, aerial
      camera) — spec in SYSTEMS.md §8 is ready
- [ ] M7 Tide element (multi-element template, element switching)
- [ ] M8 World production (World Partition, wilderness, first dungeon)
- [ ] M9 Academy hub at full scale + quest content
- [ ] M10+ remaining elements (Gale, Storm, Verdance, Umbral, Aether),
      antagonist campaign, dragon adult stage, Unison, ending
- [ ] Open design questions from GAME_DESIGN.md §11 (tracked there)

---

## Standing rules

- No system is built before its milestone (no placeholder stubs).
- Every merge compiles; content passes Originality Policy review.
- Docs updated at each milestone end (DEVELOPMENT.md cadence table).
