# Trello Card Details — Candlelight

**Board:** https://trello.com/b/MUOJqRCI/candlelightdevelopment
**Master data:** `Docs/Board_Seed.csv` · **Rules:** `Docs/GDD.md` §5 · `Docs/Board_Setup.md`

## How to use this file

1. Cards in `Week 1`–`Week 4` are **available to anyone**. Suggested owner is guidance only.
2. To claim: add yourself as the **only Trello member** on the card, then move it to `Doing`. The member avatar = owner.
3. Other contributors go in the card description as `Helpers: [names]` — not as Trello members.
4. Copy each **Description to paste** into the card. Add the listed **Labels**, **Due date**, and **Checklist**.
5. A card is `Done` only when all five gates pass **in the weekly build**, not just the editor.

Due dates: **W1 = 2026-11-29 · W2 = 2026-12-06 · W3 = 2026-12-13 · W4 = 2026-12-20**. Parking Lot and Phase 2 have no due date.

---

# WEEK 1 — Setup, look test, prototype

## W1-01 — Setup UE 5.8.4 + repo + LFS

- **List:** `Week 1`
- **Labels:** `Code`, `W1`, `Must`
- **Due:** 2026-11-29
- **Suggested owner:** Arashoo + Jon
- **Helpers:** everyone

**Description to paste:**

> Install **Unreal Engine 5.8.4** exactly — no other version. Create the project repo with **Git LFS** configured for `.uasset`, `.fbx`, `.png`, `.wav`, `.mp3`. Add a short `README` with clone/open/build steps and the exact engine version. Every present member must clone, open the project, and push at least one commit. Record the repo URL in the `#info` pin. Suggested owner: Arashoo + Jon. Helpers: everyone.

**Checklist:**

- [ ] Claimed — owner added as the only Trello member
- [ ] Logic — UE 5.8.4 opens the project; LFS tracks binary assets; commit + push works for each member
- [ ] Visual — editor screenshot at 1280×720 posted in `#general`
- [ ] Audio — no cue required for setup; confirmed no audio gate is blocked
- [ ] UX — a new member can clone and open without asking anyone
- [ ] Build — clean clone opens and every present member has a pushed commit

## W1-02 — Discord + Trello + #info pins

- **List:** `Week 1`
- **Labels:** `Lead`, `W1`, `Must`
- **Due:** 2026-11-29
- **Suggested owner:** Arashoo
- **Helpers:** none

**Description to paste:**

> Set Discord channel topics to match `Discord_Setup.md`. Pin the GDD link, repo link, board link, engine version, timezones, and visibility rules in `#info`. Board: https://trello.com/b/MUOJqRCI/candlelightdevelopment. Import the Trello cards from `Trello_Import/HOW_TO_IMPORT.md`. New ideas go to `#design` → `parking-lot`, not straight into the board. Suggested owner: Arashoo.

**Checklist:**

- [ ] Claimed — owner added as the only Trello member
- [ ] Logic — channels, pins, threads, and board lists match `Discord_Setup.md` and `Board_Setup.md`
- [ ] Visual — pinned messages readable; board URL visible without scrolling
- [ ] Audio — no cue required; confirmed
- [ ] UX — a new member can find GDD, board, and repo in under 10 seconds
- [ ] Build — `#builds` channel exists and weekly build format is pinned

## W1-03 — Wireframes + input map at 1280×720

- **List:** `Week 1`
- **Labels:** `Design`, `2D Art`, `W1`, `Must`
- **Due:** 2026-11-29
- **Suggested owner:** Fermin
- **Helpers:** Arashoo

**Description to paste:**

> Draw wireframes at **1280×720** for: title, hub, puzzle-room HUD, win, lose, pause. Lock the input map: A/D or arrows = move, Space = jump, E = interact, Esc = pause/mute. Mark the feedback cue points from `GDD.md` §2. Pin all wireframes in `#design`. Suggested owner: Fermin. Helpers: Arashoo.

**Checklist:**

- [ ] Claimed — owner added as the only Trello member
- [ ] Logic — screen flow covers hub → room → win/lose → hub; every panel has a way out
- [ ] Visual — no overlaps at 1280×720; HUD priority is candle → collectibles → mechanic state
- [ ] Audio — cue points for teleport, collect, low candle, expire, exit open are marked
- [ ] UX — a cold reader can identify the goal without explanation
- [ ] Build — wireframes pinned in `#design` and linked in `GDD.md` §2

## W1-04 — Decide D00–D06

- **List:** `Week 1`
- **Labels:** `Decision`, `Design`, `W1`, `Must`
- **Due:** 2026-11-29
- **Suggested owner:** Fermin
- **Helpers:** whole team

**Description to paste:**

> Answer all seven decisions in `GDD.md` §1: D00 final title, D01 collectibles per room, D02 candle-expiry behaviour, D03 room count, D04 the three mechanics, D05 portal theme, D06 collectible name. Write each answer into the GDD with date and reason. A decision in chat or voice does not count until written there. Suggested owner: Fermin. Helpers: whole team.

**Checklist:**

- [ ] Claimed — owner added as the only Trello member
- [ ] Logic — D00–D06 all have a chosen value, date, and short reason
- [ ] Visual — D01 count and D06 name match HUD wireframe labels
- [ ] Audio — D02 expiry and D05 portal theme match cue list in `GDD.md` §6
- [ ] UX — decisions are readable to a cold player at 1280×720
- [ ] Build — `GDD.md` §1 updated before this card moves to Done

## W1-05 — 2.5D look test + template decision

- **List:** `Week 1`
- **Labels:** `Pipeline`, `3D Art`, `W1`, `Must`
- **Due:** 2026-11-29
- **Suggested owner:** knightsGamer
- **Helpers:** Jacob, Arashoo

**Description to paste:**

> Import one lit 3D candle mesh into a dark room with an **orthographic side camera** and a point light attached to the flame. Set units (1 unit = 1 cm), material, and pivot at base. Screenshot at **1280×720** and post in `#art`. Confirm whether UE's 2D Platformer template can be used as a base or only as an input/camera reference. Record the final choice in `GDD.md` §4. If the candle does not read as 2.5D, change the approach before final art. Suggested owner: knightsGamer. Helpers: Jacob, Arashoo.

**Checklist:**

- [ ] Claimed — owner added as the only Trello member
- [ ] Logic — orthographic camera, lit mesh, and point light all present in test level
- [ ] Visual — screenshot in `#art`; candle reads clearly against dark room at 1280×720
- [ ] Audio — low-candle light change has a placeholder cue identified
- [ ] UX — shrinking light communicates timer without a number
- [ ] Build — template decision written in `GDD.md` §4 and confirmed in weekly build

## W1-06 — Prototype: move, jump, collect

- **List:** `Week 1`
- **Labels:** `Code`, `W1`, `Must`
- **Due:** 2026-11-29
- **Suggested owner:** Jon
- **Helpers:** Arashoo

**Description to paste:**

> One platform, one candle character, one collectible. Implement move (A/D or arrows), jump (Space), overlap collect, and X/Y counter starting at 0/1. Use placeholder art but final-style silhouette. No timer yet. Suggested owner: Jon. Helpers: Arashoo.

**Checklist:**

- [ ] Claimed — owner added as the only Trello member
- [ ] Logic — move, jump, collect, and counter work; no double-collect
- [ ] Visual — candle and collectible readable at 1280×720, no grey cube
- [ ] Audio — jump and collect cues fire once each
- [ ] UX — a cold player collects the item without instruction
- [ ] Build — all above confirmed in the Week 1 build

## W1-07 — Weekly build + playtest

- **List:** `Week 1`
- **Labels:** `QA`, `W1`, `Must`
- **Due:** 2026-11-29
- **Suggested owner:** all
- **Helpers:** everyone

**Description to paste:**

> Package or zip the Week 1 build, version it `v0.1`, add a changelog, and post in `#builds`. Every member plays for 5 minutes. Log the top three issues in `#qa` with card links and who can fix them. Suggested owner: all. Helpers: everyone.

**Checklist:**

- [ ] Claimed — one owner added as the only Trello member for the build post
- [ ] Logic — build launches with no crash
- [ ] Visual — no missing meshes or broken materials
- [ ] Audio — jump and collect cues fire at the right moment
- [ ] UX — cold player can move, jump, and collect
- [ ] Build — `v0.1` posted in `#builds` with changelog; top three issues in `#qa`

---

# WEEK 2 — Candle, hub, Room 01

## W2-01 — Candle timer + shrinking light

- **List:** `Week 2`
- **Labels:** `Code`, `W2`, `Must`
- **Due:** 2026-12-06
- **Suggested owner:** Arashoo
- **Helpers:** Fermin, Shags

**Description to paste:**

> Implement the candle as the visible timer: data asset for starting wax, drain rate, low-wax threshold. Point light radius shrinks with remaining wax. Fire `CandleLowEntered` once below 25% and `CandleExpired` at zero. Pause drain in menus. See `GDD.md` §1, §4, §6. Suggested owner: Arashoo. Helpers: Fermin, Shags.

**Checklist:**

- [ ] Claimed — owner added as the only Trello member
- [ ] Logic — drain, low threshold, expiry, and pause all work; `CandleLowEntered` fires once, not twice
- [ ] Visual — light radius visibly shrinks; warm → red shift readable at 1280×720
- [ ] Audio — low tick and expire puff/descending tone fire once each
- [ ] UX — cold player can tell time is short without reading a number
- [ ] Build — verified in Week 2 build, not editor only

## W2-02 — Collectibles + X/Y counter

- **List:** `Week 2`
- **Labels:** `Code`, `Design`, `W2`, `Must`
- **Due:** 2026-12-06
- **Suggested owner:** Jon + Fermin
- **Helpers:** knightsGamer

**Description to paste:**

> Spawn the required number of collectibles from room config. Overlap collects, increments HUD X/Y, fires `CollectibleCollected`, and prevents double-collect. Reaching the required amount fires `ExitPortalOpened`. Collectible name per D06. Suggested owner: Jon + Fermin. Helpers: knightsGamer.

**Checklist:**

- [ ] Claimed — owner added as the only Trello member
- [ ] Logic — correct count, no double-collect, counter resets on room entry
- [ ] Visual — flare + sparkle readable; HUD counter never overlaps at 1280×720
- [ ] Audio — chime fires exactly once per pickup
- [ ] UX — goal count readable on entry (0/Y)
- [ ] Build — required amount opens exit in Week 2 build

## W2-03 — Hub map + teleport portals

- **List:** `Week 2`
- **Labels:** `Code`, `3D Art`, `W2`, `Must`
- **Due:** 2026-12-06
- **Suggested owner:** Arashoo + Jacob
- **Helpers:** Fermin

**Description to paste:**

> Build `CL_Hub` with power outlet and/or air canal portals per D05. Portal trigger uses `Open Level` to `CL_Room_01`; `GameInstance` stores cleared rooms. Portals are labelled with room number and show a "complete" state after clearing. No timer runs in the hub. Suggested owner: Arashoo + Jacob. Helpers: Fermin.

**Checklist:**

- [ ] Claimed — owner added as the only Trello member
- [ ] Logic — portal → Room 01 → hub works; cleared state persists across level opens
- [ ] Visual — portal types visually distinct and labelled at 1280×720
- [ ] Audio — teleport whoosh fires on entry and return
- [ ] UX — player understands portals are level choices without text
- [ ] Build — full transition verified in Week 2 build

## W2-04 — Room 01 + moving platform

- **List:** `Week 2`
- **Labels:** `3D Art`, `Design`, `W2`, `Must`
- **Due:** 2026-12-06
- **Suggested owner:** Fermin + Jacob
- **Helpers:** Jon

**Description to paste:**

> Build `CL_Room_01` from Jacob's modular kit. First 30 seconds teach through level design: easy collectible on start platform, one moving platform to the second, one switch + gate to the third if D01 = 3. Use final-style materials, not greybox. Suggested owner: Fermin + Jacob. Helpers: Jon.

**Checklist:**

- [ ] Claimed — owner added as the only Trello member
- [ ] Logic — all collectibles reachable; moving platform path/speed from data asset
- [ ] Visual — final-style kit; silhouettes readable against dark background
- [ ] Audio — platform movement and switch cues have placeholders
- [ ] UX — cold player asks no "what do I do?" in first 30 seconds
- [ ] Build — Room 01 clearable in Week 2 build

## W2-05 — Room 01 audio cues

- **List:** `Week 2`
- **Labels:** `Audio`, `W2`, `Must`
- **Due:** 2026-12-06
- **Suggested owner:** Shags
- **Helpers:** Jon

**Description to paste:**

> Deliver placeholder/final cues for **teleport whoosh**, **collectible chime**, and **low-candle tick** per `GDD.md` §6. Each fires exactly once, at the right event. No sound fires twice. Credited sources tracked in `GDD.md` §6. Suggested owner: Shags. Helpers: Jon.

**Checklist:**

- [ ] Claimed — owner added as the only Trello member
- [ ] Logic — cues bound to `RoomEntered`, `CollectibleCollected`, `CandleLowEntered`
- [ ] Visual — no UI/audio desync visible in build
- [ ] Audio — three cues identifiable and each fires once
- [ ] UX — pickup and low-candle cues readable with eyes closed
- [ ] Build — verified in Week 2 build

## W2-06 — Weekly build + playtest

- **List:** `Week 2`
- **Labels:** `QA`, `W2`, `Must`
- **Due:** 2026-12-06
- **Suggested owner:** all
- **Helpers:** everyone

**Description to paste:**

> Version `v0.2`, changelog, post in `#builds`. A cold player must complete hub → Room 01 → results → hub. Log top three issues in `#qa`. Suggested owner: all. Helpers: everyone.

**Checklist:**

- [ ] Claimed — one owner added as the only Trello member for the build post
- [ ] Logic — full loop works without developer help
- [ ] Visual — portal, candle, collectibles all readable
- [ ] Audio — teleport, collect, low candle fire once each
- [ ] UX — cold player clears Room 01
- [ ] Build — `v0.2` posted with changelog; top three issues in `#qa`

---

# WEEK 3 — Room 02, mechanics, results

## W3-01 — Switch → gate mechanic

- **List:** `Week 3`
- **Labels:** `Code`, `3D Art`, `W3`, `Must`
- **Due:** 2026-12-13
- **Suggested owner:** Jon + Jacob
- **Helpers:** Fermin

**Description to paste:**

> Pressure switch opens a connected gate. Switch glows when active; gate reacts visibly. Link configured in mechanic data asset. Fire `SwitchActivated` and `GateOpened`. Must be readable at 1280×720 without text. Suggested owner: Jon + Jacob. Helpers: Fermin.

**Checklist:**

- [ ] Claimed — owner added as the only Trello member
- [ ] Logic — switch → gate link works and resets on room entry
- [ ] Visual — on/off states distinct at 1280×720
- [ ] Audio — click + gate rumble fire once each
- [ ] UX — cold player understands cause and effect without prompt
- [ ] Build — verified in Week 3 build

## W3-02 — In-room teleport pad

- **List:** `Week 3`
- **Labels:** `Code`, `W3`, `Must`
- **Due:** 2026-12-13
- **Suggested owner:** Arashoo
- **Helpers:** knightsGamer, Jon

**Description to paste:**

> Add a teleport pad **inside** a room, visually distinct from hub portals. Target configured in mechanic data asset. Fires `TeleportRequested`. Destination must be obvious before stepping on it. Suggested owner: Arashoo. Helpers: knightsGamer, Jon.

**Checklist:**

- [ ] Claimed — owner added as the only Trello member
- [ ] Logic — pad → destination works; no loop-back collision bug
- [ ] Visual — in-room pad clearly different from hub portal
- [ ] Audio — short teleport cue fires once
- [ ] UX — destination readable before use
- [ ] Build — verified in Week 3 build

## W3-03 — Room 02 + collectibles

- **List:** `Week 3`
- **Labels:** `3D Art`, `Design`, `W3`, `Must`
- **Due:** 2026-12-13
- **Suggested owner:** Fermin + Jacob
- **Helpers:** Jon

**Description to paste:**

> Build `CL_Room_02` using all three MVP mechanics: moving platform, switch → gate, in-room teleport pad. Place the D01 collectible count. Solvable without text prompts. Use final-style kit. Suggested owner: Fermin + Jacob. Helpers: Jon.

**Checklist:**

- [ ] Claimed — owner added as the only Trello member
- [ ] Logic — room solvable and all collectibles reachable
- [ ] Visual — final-style materials; mechanics distinguishable from decoration
- [ ] Audio — mechanic cues present
- [ ] UX — no text needed to solve; pacing fair with candle running
- [ ] Build — Room 02 clearable in Week 3 build

## W3-04 — Win / lose / pause panels

- **List:** `Week 3`
- **Labels:** `Design`, `Code`, `W3`, `Must`
- **Due:** 2026-12-13
- **Suggested owner:** Fermin + Jon
- **Helpers:** none

**Description to paste:**

> Build win, lose, and pause panels at 1280×720. Every panel has a visible way out: win → hub, lose → retry/hub per D02, pause → resume/mute. No overlaps. Suggested owner: Fermin + Jon. Helpers: none.

**Checklist:**

- [ ] Claimed — owner added as the only Trello member
- [ ] Logic — all buttons work; no stuck state; input released on panel close
- [ ] Visual — no overlaps at 1280×720; consistent style
- [ ] Audio — UI confirm cue fires once
- [ ] UX — every panel has an obvious exit
- [ ] Build — all panels verified in Week 3 build

## W3-05 — Candle expiry + retry (D02)

- **List:** `Week 3`
- **Labels:** `Code`, `W3`, `Must`
- **Due:** 2026-12-13
- **Suggested owner:** Arashoo
- **Helpers:** Fermin, Shags

**Description to paste:**

> Implement the chosen D02 behaviour when the candle expires: retry the room or return to hub. Fade to black, show lose panel, reset candle/collectibles/mechanics cleanly. Three consecutive retries must work without leftover event listeners. Suggested owner: Arashoo. Helpers: Fermin, Shags.

**Checklist:**

- [ ] Claimed — owner added as the only Trello member
- [ ] Logic — three retries clean; counters and mechanics reset
- [ ] Visual — fade + lose panel readable
- [ ] Audio — puff + descending tone fires once per expiry
- [ ] UX — player understands they failed and how to retry
- [ ] Build — verified three times in Week 3 build

## W3-06 — Weekly build + playtest

- **List:** `Week 3`
- **Labels:** `QA`, `W3`, `Must`
- **Due:** 2026-12-13
- **Suggested owner:** all
- **Helpers:** everyone

**Description to paste:**

> Version `v0.3`, changelog, post in `#builds`. Full loop must work: hub → room → results → hub. **Feature freeze at end of Week 3** — after this, only bugs, balance, and polish. Log top three issues in `#qa`. Suggested owner: all. Helpers: everyone.

**Checklist:**

- [ ] Claimed — one owner added as the only Trello member for the build post
- [ ] Logic — both rooms and all mechanics work end-to-end
- [ ] Visual — final-style assets everywhere
- [ ] Audio — all Must cues fire once
- [ ] UX — cold player completes a room without help
- [ ] Build — `v0.3` posted; feature freeze announced in `#general`

---

# WEEK 4 — QA and delivery

## W4-01 — Bug sweep — crashes first

- **List:** `Week 4`
- **Labels:** `QA`, `W4`, `Must`
- **Due:** 2026-12-20
- **Suggested owner:** all
- **Helpers:** everyone

**Description to paste:**

> Play both rooms for 30 minutes each. Log every crash, soft-lock, and duplicated event in `#qa` with steps to reproduce. Crashes and soft-locks are fixed before anything cosmetic. No open crash in a full run. Suggested owner: all. Helpers: everyone.

**Checklist:**

- [ ] Claimed — owner added as the only Trello member
- [ ] Logic — zero crashes and zero soft-locks in a full run
- [ ] Visual — no missing assets after repeated level loads
- [ ] Audio — no cue fires twice after repeated retries
- [ ] UX — no stuck panels or trapped player positions
- [ ] Build — verified in Week 4 build

## W4-02 — Balance pass

- **List:** `Week 4`
- **Labels:** `Code`, `Design`, `W4`, `Must`
- **Due:** 2026-12-20
- **Suggested owner:** Arashoo + Fermin
- **Helpers:** testers

**Description to paste:**

> Tune candle duration, platform speed, collectible placement, and switch/gate timing **in data assets only**. Three cold players must clear both rooms. Log before/after values with playtest evidence in `GDD.md` §1/§5. Suggested owner: Arashoo + Fermin. Helpers: testers.

**Checklist:**

- [ ] Claimed — owner added as the only Trello member
- [ ] Logic — no code changes, data values only
- [ ] Visual — timing still readable at 1280×720
- [ ] Audio — low-candle cue still has time to be heard
- [ ] UX — three cold players clear both rooms
- [ ] Build — tuned values verified in Week 4 build

## W4-03 — Onboarding check

- **List:** `Week 4`
- **Labels:** `Design`, `W4`, `Must`
- **Due:** 2026-12-20
- **Suggested owner:** Fermin
- **Helpers:** testers

**Description to paste:**

> Watch three cold players enter Room 01. Nobody may ask "what do I do?" in the first 30 seconds. Fix the top three confusions through level layout, not text tutorials. See `GDD.md` §2. Suggested owner: Fermin. Helpers: testers.

**Checklist:**

- [ ] Claimed — owner added as the only Trello member
- [ ] Logic — goal understood without instruction
- [ ] Visual — first collectible visible immediately
- [ ] Audio — first collect chime heard within 10 seconds
- [ ] UX — zero "what do I do?" questions from three cold players
- [ ] Build — verified in Week 4 build

## W4-04 — Packaging + clean-machine test

- **List:** `Week 4`
- **Labels:** `Code`, `W4`, `Must`
- **Due:** 2026-12-20
- **Suggested owner:** Arashoo or Jon
- **Helpers:** all

**Description to paste:**

> Package the Windows build. Run it on a PC **without Unreal installed**. Confirm 1280×720, input, audio, and both rooms work. If no clean machine is available, document the closest test and who verified it. Suggested owner: Arashoo or Jon. Helpers: all.

**Checklist:**

- [ ] Claimed — owner added as the only Trello member
- [ ] Logic — packaged exe launches and reaches both rooms
- [ ] Visual — correct resolution and no missing assets
- [ ] Audio — all cue assets included in package
- [ ] UX — input works with keyboard/mouse from a cold start
- [ ] Build — clean-machine result recorded in `#builds`

## W4-05 — Final playable build in #builds

- **List:** `Week 4`
- **Labels:** `Code`, `W4`, `Must`
- **Due:** 2026-12-20
- **Suggested owner:** Arashoo
- **Helpers:** Jon

**Description to paste:**

> Final versioned build, changelog, playtested start to finish. Post in `#builds` and pin it. Include the exact engine version, controls, and known issues. Suggested owner: Arashoo. Helpers: Jon.

**Checklist:**

- [ ] Claimed — owner added as the only Trello member
- [ ] Logic — full hub → room → results → hub loop passes
- [ ] Visual — final-style assets everywhere
- [ ] Audio — all Must cues fire once
- [ ] UX — onboarding check passed
- [ ] Build — final build posted in `#builds` and pinned

## W4-06 — MVP demo + phase 2 feedback

- **List:** `Week 4`
- **Labels:** `QA`, `Design`, `W4`, `Must`
- **Due:** 2026-12-20
- **Suggested owner:** all
- **Helpers:** everyone

**Description to paste:**

> Team plays the final build together. Log what to keep, change, and park in `#qa`. Move accepted phase 2 ideas into `Phase 2 - Later` on Trello and into `GDD.md` §7. Suggested owner: all. Helpers: everyone.

**Checklist:**

- [ ] Claimed — one owner added as the only Trello member for the feedback log
- [ ] Logic — feedback sorted into keep / change / park
- [ ] Visual — screenshots of best and worst framing posted
- [ ] Audio — missing or duplicate cues logged
- [ ] UX — cold-player notes logged from `#qa`
- [ ] Build — accepted ideas written into `GDD.md` §7 before cards move to Phase 2

---

# PARKING LOT — not in the MVP

These have **no due date**. Promotion rule: propose in `#design` → write into `GDD.md` §7 → team accepts → create a sprint card.

Use this checklist for every parking-lot card:

- [ ] Idea written in `GDD.md` §7 with proposer and date
- [ ] Team accepts it in `#design`
- [ ] Updated in `GDD.md` §1 if it changes the game
- [ ] Trello card created in a sprint list
- [ ] Claimed by one member before moving to `Doing`

## P-01 — More hub portals / rooms

- **Labels:** `Design`, `Should`
- **Due:** none
- **Suggested owner:** anyone
- **Description to paste:** > Phase 2 after the 2-room MVP works. Add more portals in the hub and more self-contained puzzle rooms. Each new room needs new room config data and a solvable layout.

## P-02 — New mechanics: lasers, ice, wind, shadow bridges

- **Labels:** `Design`, `Should`
- **Due:** none
- **Suggested owner:** anyone
- **Description to paste:** > Swap candidates if D04 changes. Any new mechanic must be readable at 1280×720 and teach itself without text. Needs its own spec using `GDD.md` §8 before a sprint card exists.

## P-03 — Rat / spider / droplet / mousetrap

- **Labels:** `3D Art`, `Should`
- **Due:** none
- **Suggested owner:** anyone
- **Description to paste:** > Parked from the old concept. Obstacles are mechanics in the MVP, not enemies. Revisit only if a room needs a threat. Requires a telegraph (visual + audio) before any hit.

## P-04 — Double jump costing wax

- **Labels:** `Design`, `Should`
- **Due:** none
- **Suggested owner:** anyone
- **Description to paste:** > Parked from the old concept. Jump is free in the MVP; the candle is the timer, not a combat resource. Would need a data value, denial cue, and edge-case spec.

---

# PHASE 2 — LATER — accepted post-MVP work

These have **no due date** and must not enter a sprint until after MVP acceptance.

Use this checklist for every Phase 2 card:

- [ ] MVP final build accepted first
- [ ] Idea written in `GDD.md` §7
- [ ] Owner claimed before work starts
- [ ] Five gates planned in the card
- [ ] Not started during MVP Weeks 1–4

## PH-01 — Additional puzzle rooms

- **Labels:** `3D Art`, `Design`, `Phase2`
- **Due:** none
- **Suggested owner:** anyone
- **Description to paste:** > After MVP acceptance. Add rooms 03+ using the same modular kit and room config data. Each room needs one new puzzle idea, collectibles, and a clean hub portal state.

## PH-02 — Enemy / threat system

- **Labels:** `3D Art`, `Code`, `Phase2`
- **Due:** none
- **Suggested owner:** anyone
- **Description to paste:** > Only if rooms need a threat beyond mechanics. Requires a telegraph, state machine, and clear feedback table in `GDD.md` §8. Not part of the MVP.

## PH-03 — Music states + light-radius polish

- **Labels:** `Audio`, `2D Art`, `Phase2`
- **Due:** none
- **Suggested owner:** anyone
- **Description to paste:** > Low-candle tension layer, portal stings, and post-process darkness polish. Must stay readable at 1280×720 and not duplicate existing cues.
