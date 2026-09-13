# Dragonbound — Game Design Specification

> **Status:** Living document — Milestone 0 (foundation)
> **Last updated:** 2026-09-13
>
> This is the current design specification. It is deliberately **not** a
> full production GDD: systems are specified to the depth needed to build
> the next milestones safely, and expanded as their milestones approach.
> Anything not yet decided is marked **TBD** so nobody assumes a decision
> exists.
>
> **See also:** VISUAL_STYLE.md (how it looks) · SYSTEMS.md (how it is
> built) · DEVELOPMENT.md (when it is built).

---

## 1. Vision & Pillars

**One-line pitch:** *You are the first human in an age to bond with a
dragon — and the bond will change both of you.*

**Experience pillars.** Every design decision is weighed against these:

1. **The Bond** — The relationship between the Rider and the dragon is the
   heart of the game. Everything else (powers, flight, story, progression)
   exists to deepen it.
2. **Cinematic Presence** — Premium-feel presentation: believable
   characters, natural animation, dynamic lighting, expressive camera work.
   The game must never look or feel like a generic mobile RPG.
3. **Rider & Dragon, Together** — The dragon is a co-protagonist, never a
   tool. Combat, traversal, and exploration are designed around two
   characters acting as one team.
4. **Earned Power** — Every elemental power, upgrade, and ability is
   unlocked through story and exploration, never handed out or bought.
5. **Freedom of Perspective** — The player may switch between first-person
   and third-person at any time, in any context (ground, combat, riding,
   flight), without breaking the experience.

**Anti-pillars (what Dragonbound is not):**

- Not a live-service/gacha game. No stamina-energy meters, no daily-login
  reward loops, no loot boxes.
- Not a mobile-first game. High-quality PC/console is the bar; scalability
  down comes later, not first.
- Not a multiplayer-first game. Designed and balanced single-player;
  multiplayer is explicitly out of scope until the single-player pillars
  are proven.

---

## 2. Originality Policy (mandatory for all contributors)

Dragonbound is an **original IP**. The broad fantasy concepts of dragon
riders, magical bonds, medieval fantasy settings, and elemental magic are
common cultural material and are fine to use. The following are **not**
fine, and are prohibited in any code, asset, document, or discussion:

- Names, characters, or terminology from existing fantasy franchises
  (dragons' names, school/academy names, spell names, location names, etc.).
- Art, animations, models, or audio derived from or traced from existing
  franchises.
- Dialogue, prose, or lore text copied or closely paraphrased from
  existing works.
- "Legally distinct" knockoffs of identifiable creatures or characters.

**Working rule:** when adding a name or term, search it. If it is strongly
associated with another franchise, rename it. Generic English terms
(ember, tide, gale, storm, verdance, umbral, aether) are safe as elemental
names; specific invented nouns (place names, character names, organizations)
must be original and checked.

This policy is enforced in code review and content review. Violations are
blocking.

---

## 3. Core Concept & Story Premise

### 3.1 The situation

For centuries, dragons lived openly alongside humans — some wild, some
bound to Riders through ancient pacts. Something changed. The dragons began
to be **sealed away**, vanished from the world, one lineage at a time. No
one remembers exactly how or why — history itself seems to have been
quietly rewritten. Dragons are now the stuff of children's tales.

The player character is an **ordinary person** — no prophecy, no hidden
royal blood, no chosen-one birthmark. They live in a small settlement and
have never done anything heroic.

### 3.2 The inciting event

The protagonist discovers the **final surviving egg** of an endangered
dragon lineage — sent away as the last act of a people being sealed. The
egg hatches. The hatchling imprints on the protagonist. And the hatchling
is not silent: it reaches into the protagonist's mind with emotions,
sensations, and images.

Bonding with a dragon is not a ceremony — it is something the two of them
**become**, gradually, in front of the player's eyes.

### 3.3 Story spine (long-form)

The core narrative is a **mystery**: *who sealed the dragons, how, and
why — and can it be undone?* The player's investigation weaves through:

- The seven **elemental lineages** of dragon (see §5), and what each
  lineage's sealing reveals.
- **Ancient dragon civilizations** — ruins of the age when humans and
  dragons built together.
- The **Academy** — an institution for Riders and magic-users, respected
  and compromised, where the protagonist's bond is both studied and
  feared.
- A **major antagonist** with a coherent motivation, revealed in layers.
  (Identity, name, and motive: **TBD** — designed with the first story
  milestone, M4.)

### 3.4 The protagonist

- Player-selectable **male or female**, appearance-customizable (MetaHuman
  pipeline planned — see SYSTEMS.md).
- Fully voice-acted (first-person) with a defined personality: curious,
  pragmatic, sometimes out of their depth, increasingly confident.
  *Actor casting: TBD.*
- The protagonist's background is fixed enough to ground the story
  (ordinary person from a small settlement), open enough to allow player
  expression through choice.

### 3.5 The dragon (co-protagonist)

- **Hatchling → juvenile → young adult → adult** growth stages, each a
  milestone of story and gameplay capability (see §7.3).
- Full companion character: own **personality, memories, reactions, and
  story arc**. (Personality direction: **TBD** at M3 dragon-identity
  milestone; will be shaped by the voice/character design process.)
- **Name:** not fixed by the story — **TBD** how the player relates to it
  (player-named vs. discovered-name vs. dragon-chosen). Design goal: the
  name must feel earned, never assigned by a menu.
- The dragon is a **primary character with dialogue**, not a mount that
  talks. See the Bond system (§6) for the full companion arc.

---

## 4. World

### 4.1 Scope

The **vertical slice** contains **one small, high-quality region**: a
mountain-adjacent vale with the protagonist's starting settlement, a
nearby wilderness valley, and the **hatching site** — enough space to sell
the pillars (movement, camera, bond, first power, combat) at full quality.
The full world — kingdoms, cities, ruins, academy, dungeons — is built
after the slice proves the pillars.

### 4.2 The full world (eventual)

- **Medieval fantasy kingdoms** with distinct cultures, architecture, and
  biomes; villages, towns, and cities with functional economies.
- **Wilderness** between settlements: forests, mountains, coasts, plains —
  built for ground exploration now, aerial exploration later.
- **Ancient ruins and dragon civilizations**: vertical, vertical,
  vertical — the best ruins reward climbing, gliding, and eventually
  flight.
- **The Academy**: a hub institution for Riders and magic — training,
  quests, characters, politics. Functionally a major quest hub and
  progression venue.
- **Dungeons**: handcrafted encounters that combine Rider and dragon
  abilities; puzzle and combat varieties.
- **Antagonist forces**: the mystery's opposition — patrols, garrisons,
  and agents of the sealing. (Faction name: **TBD**, original.)

### 4.3 World structure principles

- Built with **World Partition** and data layers for streaming from the
  start of world production (M4+), so the vertical-slice region can grow
  into the open world without a rebuild.
- Environment storytelling is a first-class tool: ruins, murals, and
  relics of the sealed age carry story the dialogue doesn't.
- Fast travel is **earned and in-fiction** (TBD mechanism) — never a
  loading-screen menu the game pretends doesn't exist.

---

## 5. Power System — The Seven Elements

Powers unlock **progressively through story and exploration**, not at
character creation. Design target: each element changes how the team
fights *and* moves *and* solves problems, not just the color of the
fireball.

| Element | Domain | Flavor |
| --- | --- | --- |
| **Ember** | fire | The first power — warmth, spark, courage, consuming flame |
| **Tide** | water/ice | Flow, pressure, stillness, the cold deep |
| **Gale** | wind | Speed, lift, breath, open sky |
| **Storm** | lightning | Sudden force, conduction, thunder's edge |
| **Verdance** | life/nature | Growth, healing, roots, renewal |
| **Umbral** | shadow | Concealment, silence, the unseen path |
| **Aether** | raw arcane energy | The substance of magic itself; the final, deepest power |

Each element, at full maturity, delivers **six capability layers**:

1. **Dragon abilities** — e.g., Ember: flame breath (dragon).
2. **Rider abilities** — e.g., Ember: the Rider's torchlight/cauterize
   utility and fire-infused tools.
3. **Rider combat moves** — e.g., Ember: sword-and-ignition combos,
   burning arrows, fire-infused dodges.
4. **Dragon combat moves** — e.g., Ember: sweeping claw ignition, aerial
   dive-bomb flame trails.
5. **Combined Rider + Dragon attacks** — e.g., Ember: the Rider channels
   the bond mid-combo and the dragon ignites the Rider's blade
   mid-swing; synchronized breath + arrow volleys.
6. **Upgrade / mastery progression** — each element has its own mastery
   path (see §7.2) with meaningful choices, never pure stat inflation.

**Design rule:** combined attacks are the *identity* of Dragonbound
combat. Every element is judged by how well it creates team-up moments.

**Ember (M3–M5)** is designed first and sets the template every other
element follows. All six layers are prototyped for Ember before any other
element starts production.

---

## 6. The Bond & Mind Communication

### 6.1 Bond progression

The bond is the game's central relationship system and progression
spine. It advances through shared experience — story beats, battles,
exploration, and choices — **not** through grinding or gifts.

Stages (working model, tuned in M2/M3):

1. **Awakening** — imprinting at the hatch. Raw, overwhelming emotion
   transfer.
2. **Language of Feeling** — the dragon sends emotions, sensations, and
   images. The player *interprets*, and the dragon learns whether it is
   understood.
3. **Words** — first fragments of speech; the dragon's voice emerges.
4. **Full Dialogue** — complete conversation; the dragon's personality,
   memory, and opinions are fully expressed.
5. **Unison** — the deepest stage: synchronized combat, flight, and
   perception. Reserved for the late game and the story's emotional peaks.

The bond also gates and is gated by the power system: elements unlock
through story events *and* bond depth — the dragon cannot safely channel
what the bond cannot carry.

### 6.2 Communication mechanics

- **Dragon → Rider:** mind-speech (stage-gated as above), emotional
  pulses, shared imagery (brief stylized vision moments), and physical
  behavior (the dragon is always *telling* you something if you look).
- **Rider → Dragon:** player dialogue choices (branching, bond-aware),
  in-world actions the dragon reacts to (feeding, protecting, exploring),
  and combat commands (contextual, bond-gated — see SYSTEMS.md).
- **Reactivity rule:** the dragon remembers. Major story choices,
  moments of care or neglect, and battle outcomes change the dragon's
  dialogue, reactions, and trust — this is designed in from the start,
  not bolted on.

---

## 7. Progression

### 7.1 Rider progression

- **Combat skill:** unlocked moves and improved fundamentals (movement
  tech, combos, timing) — skill-based, earned through the Academy and
  world teachers.
- **Rider powers:** mirror the bond; each element adds Rider abilities.
- **Equipment:** light gear customization (weapon types, armor sets) with
  visual identity per culture — **TBD** full itemization depth (kept
  deliberate; Dragonbound is not a looter).

### 7.2 Dragon progression

- **Growth stages** (story-gated, irreversible, dramatic):
  1. **Hatchling** — carried, curious, fragile; the tutorial companion.
  2. **Juvenile** — follows, fights alongside on the ground, still
     learning to be a dragon.
  3. **Young adult** — first flight; the open sky becomes a game space.
  4. **Adult** — full combat and flight mastery; the story's endgame
     partner.
- **Elemental mastery:** per-element upgrade paths (see §5.6) shaped by
  exploration finds (dragon-civilization relics) and story choices.
- **Bond depth:** see §6.1.

### 7.3 Unlock philosophy

- Story events unlock **capabilities** (a new element, flight).
- Exploration rewards **mastery** (upgrades, techniques, lore).
- Nothing core is gated behind repeated content. Optional depth can be.

---

## 8. Combat

### 8.1 Feel targets

- Rider combat is **readable, weighty, and fluid** — deliberate action
  combat, not cooldown-rotation MMO combat.
- The dragon fights **alongside** the Rider by default (combat AI), and
  under **player direction** at higher bond levels (targeting, combined
  attacks, positioning).
- Combined attacks are the payoff: earned, dramatic, effective — and
  never required as button-mash filler.

### 8.2 Systems view

- Damage, attributes, status effects, and abilities run on the **Gameplay
  Ability System** (see SYSTEMS.md → Combat).
- Enemy variety starts small and high-quality: **three hand-tuned enemy
  archetypes** for the slice (light skirmisher, shielded brute, ranged
  harrier — internal names TBD) rather than a wide shallow roster.
- Difficulty: designed default; accessibility options (reduced damage
  taken, slowed attack tells, etc.) designed in from M3 — **TBD** exact
  set.

---

## 9. Camera & Controls

- **Third-person** (default): over-shoulder action camera with cinematic
  framing; tight combat camera with target lock-on.
- **First-person**: full HUD-less-or-diegestic-feel option; body
  awareness (visible hands/weapon; the dragon is fully present beside you).
- **Free switching**: a single button swaps perspective instantly, in
  every context — exploration, combat, riding, flight. No context may
  silently disable the player's chosen view. (Technical rules for how each
  context honors this: SYSTEMS.md → Camera.)
- **Controls (target, PC gamepad+mouse/keyboard):** standard action-RPG
  layout; dragon commands on contextual command buttons; mount/dismount
  contextual. Full control map ships with M1; remapping via Enhanced
  Input from M2.

---

## 10. Content Outline — Vertical Slice (M0–M5)

The slice is the playable proof of every pillar:

1. **Opening** — the ordinary life, the discovery, the hatch. (M3/M4)
2. **Bond forming** — feeding, protecting, and understanding the
   hatchling; the first words of mind-speech. (M2/M3)
3. **First flight of Ember** — the Ember unlock event. (M4/M5)
4. **First combined combat** — a set-piece encounter built around a
   Rider+dragon team attack. (M5)
5. **The first shadow of the mystery** — a reveal that the sealing is
   real, ongoing, and closer than it should be. (M4/M5)

Exit criteria: a new player experiences all five pillars — the bond,
cinematic presence, teamwork, earned power, and free perspective — inside
one sitting, with zero placeholder art in the critical path.

---

## 11. Open Design Questions (tracked, not forgotten)

1. Dragon's name — player-chosen, discovered, or self-chosen? (M3)
2. Antagonist identity, motivation, and faction name. (M4)
3. Academy's role in the slice vs. full game; Academy name. (M4)
4. Equipment/itemization depth. (M5+)
5. Fast-travel fiction. (M5+)
6. Difficulty/accessibility option set. (M3)
7. Protagonist voice direction and dragon voice direction. (M2/M3)
8. Save/story-state architecture details (solved in SYSTEMS.md at M3).
9. Controller remapping UX. (M2)
10. Photo mode. (M5+)

---

## 12. Document Maintenance

- This document is updated at the **end of every milestone** to reflect
  what was decided, built, and cut.
- Feature requests that conflict with a pillar require a written
  rationale in the milestone PR.
- "TBD" is a legitimate state. Unmarked TBDs in reviewed milestones are
  defects.
