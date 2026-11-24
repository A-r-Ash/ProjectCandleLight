# CANDLELIGHT — Game Design Document

**Status: OFFICIAL · v1.0 · agreed 24 Nov 2026.**
**Document owner: Arashoo (lead).** This is the single source of truth. The former `00`–`07` documents were merged into this file and deleted. If an old file or Discord message disagrees with this file, **this file wins**.

**Live GDD link:** `[TBD — paste the Google Docs URL here and pin it in #info]`

**Trello board:** https://trello.com/b/MUOJqRCI/candlelightdevelopment

**GitHub docs repo:** https://github.com/A-r-Ash/ProjectCandleLight

**Team:** Arashoo (lead + core code) · Jon (systems code) · **Jacob (lead 3D — owns all 3D art/assets, rooms, kit and mechanics)** · knightsGamer (3D pipeline + look test — imports, units, materials and template decision; supports Jacob) · Fermin (design + 2D art + UI) · Shags (audio) · Krassus (pending — no tasks until he replies).

Working title: **Candlelight** (Fermin may confirm the final name — D00).

---

## 0. READ THIS FIRST — 30 seconds

**What we build:** A Captain Toad-style 2.5D puzzle-platformer. From a hub, the player teleports through a power outlet / air canal into a small puzzle room, collects the required light items before the candle burns down, then returns to the hub for the next room. **The candle is the timer.**

**Locked:** Unreal 5.8.4 · Windows PC · 1280×720 · fixed 2D camera + lit 3D assets · 4-week playable MVP · playable build only, no report.

**MVP scope:** Hub + 2 puzzle rooms + 3 mechanics + candle timer + win/lose + packaged build.

**Owners:** Arashoo = lead + core code · Jon = systems code · **Jacob = lead 3D (all 3D art/assets, rooms, kit, portals, mechanics)** · **knightsGamer = 3D pipeline + look test, supports Jacob** · Fermin = design + 2D art + UI (decides open questions) · Shags = audio · **Krassus = pending, no tasks until he replies**.

**Where things live:**

| Need | Place |
|---|---|
| What/why we build | **This GDD** |
| Who does what by when | Trello — one owner per card |
| Today's status / blockers | `TEAM_STATUS.md` + `#general` |
| Discussion only | Discord — decisions written here within 24 hours |

**Definition of Done — all five gates, every time:** Logic works · Visual is final-style · Audio cue fires · UX readable at 1280×720 · confirmed in the weekly build, not just the editor.

---

## 1. CONCEPT

### Logline

A tiny flame lives in a dark house. The player steps into a portal and appears inside a small puzzle room. The light hidden there must be collected before the candle burns down, then the player jumps back to the hub and chooses the next room.

### Core loop

`Hub → portal → room starts / candle lights → solve mechanics → collect required items → exit portal opens → results → hub`

If the candle expires: room resets. Exact retry behaviour = D02.

### Pillars

1. **The candle is the clock.** Time is visible as shrinking light, not only a number.
2. **The room is the puzzle.** Obstacles *are* the mechanics. No enemy system in the MVP.
3. **The hub is the promise.** Every portal is a new small challenge chosen by the player.

### Locked decisions

| Decision | Agreed value |
|---|---|
| Level shape | Self-contained puzzle rooms, Captain Toad style |
| Goal | Collect a required number of items |
| Timer | The little candle itself |
| Level select | Teleport hub — power outlet / air canals |
| Obstacles | Mechanics, not enemies |
| Camera | Fixed 2D side view; 3D assets → 2.5D |
| Engine / platform | Unreal 5.8.4, Windows PC, 1280×720 |
| Timeline | 4-week playable MVP |

### The three MVP mechanics (D04 — Fermin confirms)

1. **Moving platform** — timing.
2. **Pressure switch → gate** — cause and effect.
3. **In-room teleport pad** — distinct from the hub portals.

### Win / lose

- **Win:** required collectibles gathered → exit portal opens → return to hub; room shows "complete".
- **Lose:** candle expires → screen fades → lose panel → retry or hub, per D02.

### Open decisions — Fermin, end of Week 1

| ID | Question | Options | Chosen |
|---|---|---|---|
| D00 | Final title | Candlelight / Candlesticks | |
| D01 | Collectibles per room | 3 / 5 — readable in HUD | |
| D02 | Candle expiry | Retry room / back to hub | |
| D03 | MVP rooms | 2 / 1 if schedule slips | |
| D04 | Three mechanics | Confirm or swap from §7 | |
| D05 | Portal theme | Outlet / canals / both | |
| D06 | Collectible name | Wax drop / light fragment | |

Team's own words: *"a game like captain toad... go inside a level... puzzle... collect certain amount of things... timer is the little candle itself... teleport system between levels... power outlet or air canals... obstacles are the mechanics... fixed 2d platformer camera... assets are 3d so 2.5d."*

---

## 2. UX

Owner: **Fermin**. Wireframes before pixels.

### Screen flow

```
Title → Hub (portals)
          ├─ Portal → Puzzle Room
          │    ├─ Required collectibles → Exit portal → Win panel → Hub
          │    └─ Candle expires → Lose panel → Retry / Hub
          └─ Pause / mute
```

Wireframes needed: **title, hub, room HUD, win, lose, pause** — pin in `#design`.

### HUD priority — exactly three things

1. **Candle timer** — shrinking light + visible flame, primary.
2. **Collectibles X / Y** — always visible.
3. **Mechanic state** — switch on/off, gate open/closed, platform moving.

Nothing else in the HUD.

### Input

| Action | Key |
|---|---|
| Move | A/D or arrows |
| Jump | Space |
| Interact with portal / switch | E |
| Pause / mute | Esc |

Gamepad optional. Keyboard/mouse default.

### Feedback contract — one signal per event

| Event | Visual | Audio | UI |
|---|---|---|---|
| Room entered | portal flash, candle lights | teleport whoosh | counter 0/Y |
| Collectible collected | flare + sparkle | chime | counter increments |
| Candle low (<25%) | warm → red edge glow, light shrinks | tension layer | candle pulses |
| Candle expired | fade to black | puff + descending tone | lose panel |
| Switch activated | switch glows, gate reacts | click + rumble | none — read the world |
| Exit portal open | warm beam, different from entry portals | rising tone | "exit open" hint |
| Level complete | candle flares | warm resolve | win panel |

### Onboarding — first 30 seconds, no text

1. Room 1 starts with candle lit and 3 collectibles visible — the goal reads itself.
2. One easy collectible on the starting platform — instant reward.
3. One moving platform to the second — teaches timing.
4. One switch + gate to the third — teaches cause/effect before time gets tight.

Test with 3 cold players. If anyone asks "what do I do?", the room failed. Fix the top 3 confusions.

### Rules

- Every panel has a visible way out.
- No HUD overlaps at 1280×720.
- Portals are visually distinct and labelled with room number; cleared rooms show "complete".
- Hub has no running timer.

---

## 3. ART

Owners: **Jacob** = lead 3D — candle, collectible, teleport pad, hub, modular room kit, portals, gate, all 3D meshes/materials · **knightsGamer** = 3D pipeline and look test (imports, units, materials setup, template decision), supporting Jacob · **Fermin** = UI, parallax, cover art.

### The look

Orthographic side camera, lit 3D meshes in the foreground, 2D/parallax dark house behind. One small warm light against a cold room. **The light shrinking IS the mechanic.**

### Palette

| Role | Colour | Rule |
|---|---|---|
| Candle light | warm amber `[hex TBD]` | only warm light source |
| House | cold blue-grey `[hex TBD]` | low contrast, reads as depth |
| Danger | red `[hex TBD]` | reserved only for low/expiring candle |
| Collectible | bright yellow-white `[hex TBD]` | reads instantly |

No other accent colours. If everything is bright, nothing is urgent.

### Scale / naming

- 1 Unreal unit = 1 cm · pivots at base · `CL_<Category>_<Name>_<Variant>`.
- Modular pieces so Jacob can build all rooms from one kit.
- Final candle height and platform grid size: `[TBD in look test]`.

### Asset list

| Asset | Owner | Priority |
|---|---|---|
| Candle character (body + flame) | Jacob | Must |
| Collectible (wax drop / light fragment) | Jacob | Must |
| In-room teleport pad | Jacob | Must |
| Hub floor + props | Jacob | Must |
| Power outlet portal | Jacob | Must |
| Air canal portal | Jacob | Should |
| Modular room kit (walls, floor, platforms) | Jacob | Must |
| Moving platform | Jacob | Must |
| Pressure switch + gate | Jacob | Must |
| Exit portal — distinct from entry portals | Jacob | Must |
| UI icons / HUD / result panels | Fermin | Must |
| Parallax layers | Fermin | Should |
| Cover art / screenshot framing | Fermin | Should |

Enemies and checkpoints from the old concept are parked in §7.

### Look test — Jacob (lead 3D) + knightsGamer (pipeline), before final art

Orthographic camera + one lit 3D candle in a dark room at 1280×720. Screenshot in `#art`. If it doesn't read as 2.5D, change the approach before anyone makes final assets.

### "Looks complete" checklist

Final-style mesh/material, silhouette readable against dark background, danger state distinct, correct scale/pivot, present in the weekly build. **No "art pass later."**

---

## 4. TECH

Owners: **Arashoo** = core code · **Jon** = systems · **Jacob** = lead 3D (all 3D assets, rooms, kit, mechanics) · **knightsGamer** = pipeline/import + look test, supporting Jacob.

### Locked

Unreal **5.8.4** for everyone · Windows PC · 1280×720 · 60 fps target · fixed orthographic camera · lit 3D meshes.

### UE template decision

UE's **2D Platformer template** is Paper2D sprite-based and unlit — it fights the shrinking light. Use it as a **reference for input/camera only**. Start from **Third Person or Blank** with an orthographic camera. Final choice recorded in the Week 1 look test (Jacob leads the 3D asset, knightsGamer leads the pipeline decision): `[TBD after look test]`.

### Hub / teleport

Separate maps — `CL_Hub`, `CL_Room_01`, `CL_Room_02` — with trigger-volume portals using `Open Level`. A `GameInstance` stores cleared rooms. Sublevel streaming is phase 2 only if transitions feel slow.

### Data-driven values — no magic numbers

| Config | Fields |
|---|---|
| Room | required collectibles, candle duration, destination, room number |
| Mechanic | platform path/speed, switch → gate link, teleport pad target |
| Candle | starting wax, drain rate, low-wax threshold |

Log before/after values when tuning.

### Events

`RoomEntered` · `CollectibleCollected` · `CandleLowEntered` · `CandleExpired` · `SwitchActivated` · `GateOpened` · `ExitPortalOpened` · `LevelComplete` · `TeleportRequested`

Systems raise events; UI listens. **UI never calls gameplay systems.** Register/unregister on begin/end play. Any new system documents what it reads, raises, and hears here before coding.

### Content folders

```
Content/Candlelight/
  Maps/    Hub/, Rooms/
  Hub/     Portals/, Props/
  Rooms/   Room_01/, Room_02/
  Mechanics/ MovingPlatform/, SwitchGate/, TeleportPad/
  Characters/ Candle/
  Pickups/ Collectible/
  UI/  Audio/  Data/
```

### Pre-production checklist

UE 5.8.4 + repo + Git LFS, everyone pushes · template decision recorded · look test screenshot · Trello live with URL in `#info` · room config data asset with placeholder values · one event verified end-to-end, then test code deleted.

---

## 5. PRODUCTION PLAN

Goal: **playable Windows build** — hub + 2 rooms + 3 mechanics + candle + win/lose. 1 sprint = 1 week. Weekly build in `#builds`.

This is a **pull board**: names below are **suggested owners only**. Any member may claim any card, add themselves as its only Trello member, and move it to `Doing`.

### Week 1 — setup, look test, prototype

| Task | Suggested owner | Done when |
|---|---|---|
| UE 5.8.4 + repo + LFS | Arashoo + Jon | Every present member pushed a commit |
| Discord topics + `#info` pins + Trello live | Arashoo | Board URL pinned, matches Discord setup |
| Wireframes + input map at 1280×720 | Fermin | Hub/HUD/win/lose pinned in `#design` |
| Decide D00–D06 | Fermin | Answers written into §1 |
| 2.5D look test + template decision | Jacob + knightsGamer | Screenshot in `#art`, decision in §4 |
| Prototype: move, jump, collect | Jon | All five gates pass in build |
| Weekly build + playtest | All | Top 3 issues in `#qa` |

### Week 2 — candle, hub, Room 01

| Task | Suggested owner | Done when |
|---|---|---|
| Candle timer + shrinking light + data asset | Arashoo | Drains, shrinks, low cue fires |
| Collectibles + X/Y counter | Jon + Fermin | Required amount opens exit |
| Hub map + portal level select | Jacob + Arashoo | Portal → Room 01 → hub works |
| Room 01 blockout + moving platform | Jacob + Fermin | Collectible reachable, final-style kit |
| Room 01 audio cues | Shags | Teleport, collect, low candle fire once |
| Weekly build + playtest | All | Cold player clears Room 01 |

### Week 3 — Room 02, mechanics, results

| Task | Suggested owner | Done when |
|---|---|---|
| Pressure switch → gate | Jacob + Jon | Visually readable at 1280×720 |
| In-room teleport pad | Arashoo | Distinct from hub portals |
| Room 02 layout + collectibles | Jacob + Fermin | Solvable without text |
| Win / lose / pause panels | Fermin + Jon | Every panel has a way out |
| Candle expiry + retry (D02) | Arashoo | Three clean retries |
| Weekly build + playtest | All | Full hub → room → results → hub works |

**Feature freeze: end of Week 3.** Only bugs, balance, polish after this.

### Week 4 — QA and delivery

| Task | Suggested owner | Done when |
|---|---|---|
| Bug sweep, crashes first | All | Zero open crashes in a full run |
| Balance pass via data assets | Arashoo + Fermin | 3 cold players clear both rooms |
| Onboarding check | Fermin | Nobody asks "what do I do?" |
| Packaging + clean-machine test | Arashoo or Jon | Runs without the engine installed |
| Final build in `#builds` | Arashoo | Versioned, changelog, tested start to finish |
| Demo + phase 2 feedback | All | Feedback logged in `#qa` |

Standing rules: cards in `Week 1`–`Week 4` are unclaimed and free to take · claim by adding yourself as the only Trello member, then move to `Doing` · **the Trello member field is the owner**; helpers go in the description · no work without a card · a `Doing` card needs exactly one owner and a due date · WIP limit 2 · standup 2–3×/week with Trello links · Krassus gets no card until he replies · chat/voice decisions written here within 24 hours.

---

## 6. AUDIO

Owner: **Shags**. Every "Must" cue must be audible in the weekly build.

### SFX

| Event | Cue | Priority | Status |
|---|---|---|---|
| Room entered / teleport | whoosh | Must | TBD |
| Collectible collected | bright chime, satisfying rise | Must | TBD |
| Jump | soft scuff | Must | TBD |
| Candle low (<25%) | faint tick / wax crackle | Should | TBD |
| Switch activated | click + gate rumble | Must | TBD |
| Exit portal opens | rising tone | Must | TBD |
| Candle expired | sharp puff + descending tone | Must | TBD |
| Level complete | warm resolve | Must | TBD |
| UI confirm | simple, consistent | Should | TBD |

### Music

| State | Description | Priority |
|---|---|---|
| Hub / house ambience | quiet, cold, sparse | Must |
| Low candle (<25%) | tension layer, no lyric | Should |
| Win | short resolve | Should |

Rules: pickup and expire must be identifiable with eyes closed · no sound fires twice for one event · credited free/licensed sources tracked in this file · test in `#qa`.

---

## 7. BACKLOG / PARKING LOT

Nothing here is in the MVP. Promotion rule: propose in `#design` → add row here → team accepts → write into §1 → **then** create a Trello card.

| Idea | Status | Note |
|---|---|---|
| More hub portals / rooms | Phase 2 | After the 2-room MVP works |
| Lasers, ice, wind, shadow bridges | Phase 2 | Swap candidates if D04 changes |
| Moving hazards | Phase 2 | Different from mechanics-as-obstacles |
| Rat / spider / water droplet / mousetrap | Parked | Old concept; obstacles are mechanics now |
| Ghost of killed enemies / haunted objects | Parked | Needs systems we don't have |
| Checkpoints inside rooms | Parked | Rooms are short; candle reset instead |
| Double jump costing wax | Parked | Jump is free; candle is the timer, not a combat resource |
| Second character / co-op | Later | Not in scope |

The old `Specs/MECH_Wax.md` and `Specs/ENEMY_Rat.md` are **format examples only**, not current design instructions.

---

## 8. DESIGNER BLUEPRINT

For **Fermin**. Use this every time he designs something.

### Five rules

1. Write it down **here**. Discord and voice don't count.
2. Every design has 5 blocks: **Data, States, Events, Feedback, Done test**.
3. UX before code — wireframe first.
4. Done = Logic + Visual + Audio + UX + Build. All five, in the sprint build.
5. New ideas go to §7, never straight into the build.

### Categories and what to add

| Category | Use for | Extra block |
|---|---|---|
| Movement | run, jump | feel numbers + edge cases |
| Mechanic | candle, collectibles, teleport | tuning table |
| Level | rooms, platforms | pacing in seconds |
| Game State | title, win, lose, pause | screen flow |
| UI / Screen | HUD, panels | 1280×720, no overlaps |
| Enemy | parked, not MVP | telegraph required |
| Feedback *(cross-cutting)* | what player sees/hears per event | one row per event |
| Balance *(cross-cutting)* | values and difficulty | before/after + playtest evidence |
| Onboarding *(cross-cutting)* | first 30 seconds | 3 steps, no text |

**Two meanings of feedback:** in a spec it means signals the **game gives the player**; in `#qa` it means notes **players give the team**. Never write player opinions into a spec.

Camera and lighting belong in §3/§4, not a design spec.

### Spec template — copy into `Specs/<Name>.md`

```
# <Name> — <Category>
Owner: Fermin | Status: DRAFT / LOCKED

## Data        | Field | Value | Why |
## States      A -> B -> C  (write each trigger)
## Events      | Event | Raised by | Heard by |
## Feedback    | Event | Visual | Audio | UI |
## Edge cases  At least 3: "what if the player does X?"
## DONE WHEN   | Logic | Visual | Audio | UX | Build |
## Open decisions | Question | Options | Chosen | Date |
## Tuning log | Date | Field | Before | After | Evidence |
```

Good test: *"Timer hits zero **and** light changes **and** sizzle fires **and** it reads at 1280×720 **and** it is in this week's build."* Not: "Timer works."

### Deciding when Arashoo isn't here

Ask: (1) What decision does this give the player? If none, cut it. (2) Does it serve the pillars in §1? (3) Can it be done this week? Escalate only if it changes pillars, scope, architecture, or deadline.

### Week 1 checklist

Read §1, §2, §5 · decide D00–D06 and write them into §1 · wireframe title/hub/HUD/win/lose/pause and pin in `#design` · fill the feedback table in §2 · spec the three MVP mechanics with the template above · confirm no enemies in the MVP.

### Decision log

| ID | Decision | Chosen | Date | Why |
|---|---|---|---|---|
| D00–D06 | See §1 open decisions | | | |
