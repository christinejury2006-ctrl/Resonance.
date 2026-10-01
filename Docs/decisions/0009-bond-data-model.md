# ADR-0009 — Bond Data Model

- **Status:** Accepted
- **Date:** 2026-10-01
- **Scope:** M2 Dragon Companion & Bond v1

## Context

The first bond must communicate before full dialogue exists. The system needs
to represent emotional impressions, physical sensations, and brief mental
images while also remembering meaningful Rider choices.

## Decision

Use a small runtime bond model centered on `UDBBondComponent` and
`UDBMindLinkComponent`.

### Bond progression

M2 implements two stages:

1. **Awakening** — the imprint is raw and overwhelming.
2. **Feeling** — communication can carry interpretable emotional and sensory
   cues.

Later stages (Words, Full Dialogue, Unison) remain outside M2.

### Persistent memory

Meaningful interactions create `FDBondEvent` records containing an event
identifier, interaction type, valence, and a timestamp. The component keeps
these events for the lifetime of the bond component so the dragon's reactions
can account for what happened earlier in the session.

M2 meaningful interactions are Feed, Soothe, and Protect. A successful
remembered interaction contributes bond depth; reaching the configured
threshold advances Awakening to Feeling.

### Mind-link payloads

`FDBMindLinkPayload` is intentionally small:

- **Type:** Emotion, Sensation, or Image.
- **Cue:** authored/semantic cue name.
- **Intensity:** normalized presentation strength.
- **Duration:** presentation lifetime.

The payload is delivered through the dragon's mind-link component and a Rider
receiver component. Presentation systems can later turn the same payload into
directional audio, visual flashes, animation reactions, or dialogue-stage
communication without changing the bond model.

## Consequences

- M2 can prove the bond loop before voice/dialogue technology exists.
- The data model is extensible without coupling gameplay to a particular UI,
  audio asset, or animation system.
- Memory is represented as gameplay events rather than hard-coded one-off
  flags.

## Out of scope

Words-stage dialogue, full conversational AI, save-game persistence across
sessions, combat commands, and Unison are later milestones.
