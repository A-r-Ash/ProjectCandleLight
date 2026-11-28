# Wax — Mechanic Spec (ARCHIVED EXAMPLE)

**Do not build from this file.** Current rules are in `GDD.md` §1, §6 and §8. This file is only a formatting example.

Owner: Fermin · Status: **ARCHIVED — old concept, not in the MVP**.

## Purpose

Wax is the single resource that makes every player decision a tradeoff: it is the timer, the cost of mobility, and the character's life. The player is always asking "spend it now or save it?"

## Data (tunable — no magic numbers in code)

| Field | Proposed value | Unit | Notes |
|---|---|---|---|
| Starting wax | 100 | units | Fill at level start |
| Drain rate | 5 | units/sec | Confirm: real time or movement only? |
| Double-jump cost | 15 | units | Failed double jump = no jump + denial cue |
| Pickup restore | 30 | units | Phase 2 pickup |
| Low-wax threshold | 25 | units | Triggers music state + red edge glow |
| Checkpoint refill | 50 | units | Phase 2 — or full refill? Decision |

## States

`Full (100) → Draining → Low (<25) → Empty → Extinguished → Respawn`

- Empty and Extinguished are separate only if you choose a recoverable dim state (see `GDD.md` §1 decision D02). If extinguish = instant death, merge them.
- Pause drain in menus, at checkpoints, and on game over.

## Events

| Event | Raised by | Heard by | Payload |
|---|---|---|---|
| WaxChanged | WaxSystem | HUD, LightController | current, max |
| WaxLowEntered | WaxSystem | Audio, HUD, PostFX | none |
| WaxLowExited | WaxSystem | Audio, HUD | none |
| DoubleJumpUsed | PlayerController | WaxSystem, VFX | cost |
| WaxDenied | WaxSystem | HUD, Audio | none |
| LightExtinguished | WaxSystem | GameState, Audio, HUD | none |

## Feedback table

| Event | Visual | Audio | UI |
|---|---|---|---|
| Wax draining | light radius shrinks continuously | faint crackle at low wax | meter decreases |
| Wax low | warm → red edge glow | tension music layer | meter pulses |
| Double jump | wax burst VFX | flare + cost tick | meter drops visibly |
| Wax denied | brief red flash on meter | soft denial click | meter shakes |
| Extinguished | screen fades to black | sharp puff + descending tone | game-over panel |

## Edge cases

1. Double jump attempted with exactly 0 wax → no jump, denial cue, no state change.
2. Pause menu opened while draining → timer frozen, resumes on unpause.
3. Death at a checkpoint → wax resets to the agreed refill value.
4. Wax pickup collected at 100 → no overflow; cue still plays or is suppressed? [decision]
5. Restart from win/lose screen → wax fully reset, no leftover event listeners.
6. Low-wax threshold crossed twice rapidly → `WaxLowEntered` must fire once, not twice.

## Acceptance tests — five gates

| Gate | Test |
|---|---|
| Logic | Drain, double-jump cost, denial, and pickup values match the data table; edge cases 1–6 pass |
| Visual | Light radius visibly shrinks; low-wax colour shift readable at 1280×720 |
| Audio | Crackle, flare, denial, and extinguish cues each fire exactly once |
| UX | A cold player can say how much wax they have without reading a number |
| Build | All of the above confirmed in this week's build, not the editor |

## Open decisions for Fermin

| Question | Options | Chosen | Date |
|---|---|---|---|
| Drain in real time or only while moving? | Real time / movement only | | |
| Extinguish = instant death or recoverable dim state? | Instant / dim | | |
| Minimal wax in 4-week MVP? | Yes / no | | |
| Checkpoint refill amount | 50 / full | | |

## Tuning log

| Date | Field | Before | After | Reason + playtest evidence |
|---|---|---|---|---|
| | | | | |
