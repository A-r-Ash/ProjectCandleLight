# Rat — Enemy Spec (ARCHIVED EXAMPLE)

**Do not build from this file.** Enemies are parked in `GDD.md` §7. This file is only a formatting example.

Owner: Fermin · Status: **ARCHIVED — old concept, not in the MVP**.

## Role

A ground chaser. It denies the player the option to stand still, so every jump and platform choice costs something. No other MVP enemy covers this pressure.

## State machine

| State | Trigger | Behaviour | Player-visible signal |
|---|---|---|---|
| Patrol | Spawn | Walks a short path | Slow skitter, tail twitch |
| Alert | Player within detect radius | Stops, turns toward player | Squeak + rears up |
| Windup | Alert confirms | Holds for windup time | Clear pose, audio rises |
| Attack | Windup ends | Lunge / contact hit | Fast dash |
| Recover | Attack complete | Brief pause, vulnerable | Slumps |
| Dead / Reset | Player combat action or respawn | Removed or reset to patrol | Death cue |

## Data

| Field | Proposed value | Unit | Notes |
|---|---|---|---|
| Detect radius | 400 | cm | Confirm by playtest, not guesswork |
| Patrol speed | 80 | cm/s | |
| Chase speed | 220 | cm/s | Must be beatable by a normal jump |
| Windup time | 0.4 | sec | Never shorter — reaction time |
| Recover time | 1.0 | sec | Window for counterattack |
| Wax damage / extinguish | TBD | | Depends on the extinguish decision in `GDD.md` §1 D02 |

## Telegraph — most important section

The player must know it is about to strike **before** it strikes.

| Sense | Signal | Duration |
|---|---|---|
| Visual | Rears up, red-tinted eyes | 0.4 sec minimum |
| Audio | Rising squeak | Starts with the visual |

If a player dies without seeing the telegraph, the telegraph failed, not the player.

## Interaction with combat

| Combat option (Fermin's decision) | Result | Cost to player |
|---|---|---|
| Stomp | Rat dies | Landing precision |
| Light pulse costing wax | Rat dies | Wax |
| Dodge-only | Cannot kill; must avoid | Time + wax drain |

## Events

| Event | Raised by | Heard by | Payload |
|---|---|---|---|
| RatAlerted | Rat AI | Audio, VFX | rat id |
| RatAttacked | Rat AI | PlayerController | none |
| RatKilled | Combat | Audio, GameState | rat id |
| LightExtinguished | Player hit | GameState | none |

## Feedback table

| Event | Visual | Audio | UI |
|---|---|---|---|
| Alert | rear-up pose, eye glint | rising squeak | none — read the world |
| Attack | lunge trail | skitter burst | none |
| Killed | squash / wax burst | death chirp | none |
| Player hit | screen flash | puff + descending tone | game-over panel |

## Edge cases

1. Rat spawns within detect radius on top of the player → grace period before first alert.
2. Two rats alert at once → telegraphs must remain distinguishable.
3. Rat walks off a platform → reset to patrol point, do not fall forever.
4. Player stomps during recover → kill registers; during attack → hit registers first.
5. Player dies → all rats reset on respawn.

## Acceptance tests — five gates

| Gate | Test |
|---|---|
| Logic | All states cycle; telegraph always precedes hit; edge cases 1–5 pass |
| Visual | Telegraph readable at 1280×720 against the dark basement |
| Audio | Squeak, skitter, death, and hit cues each fire once |
| UX | 3 cold players dodge the first attack after seeing it once |
| Build | Confirmed in this week's build |

## Dependencies

Art: knightsGamer (mesh, telegraph pose) · Audio: Shags (4 cues) · Code: Jon + you (AI, collision)

## Open decisions

| Question | Options | Chosen | Date |
|---|---|---|---|
| Combat approach | stomp / light pulse / dodge-only | | |
| Extinguish behaviour | instant death / dim state | | |
