# START HERE — Candlelight

**Agreed by the whole team, 24 Nov 2026. Pin this in `#general` and paste it in Discord now.**

**Full rules:** `GDD.md` is the single source of truth. This is just the 30-second version.

## What we are building

A small puzzle-platformer: from a hub, the player teleports into a self-contained 2.5D room, collects a required number of light items before the candle burns down, then returns to the hub to pick the next room. **The candle is the timer.**

## Locked decisions

- Puzzle **rooms**, not one long level — Captain Toad style
- Collect a set number of items to clear a room
- Timer = the little candle itself
- **Hub with teleport level select** (power outlet / air canals)
- **The obstacles are the mechanics** — moving platforms, switches, gates, teleport pads
- Fixed 2D camera + 3D assets = **2.5D**
- **Unreal 5.8.4 · Windows PC · 1280×720 · 4-week playable MVP**
- No report or presentation for now — playable build only

## 4-week scope

Hub + 2 puzzle rooms + 3 mechanics + candle timer + win/lose + a packaged build.

## Owners

| Person | Owns |
|---|---|
| Arashoo | Lead + core code |
| Jon | Systems code |
| knightsGamer | 3D pipeline + **look test** |
| Jacob | 3D environments / puzzle kit |
| Fermin | Design, 2D art, UI — decides the open questions |
| Shags | Audio |
| Krassus | **Pending** — no tasks until he replies |

## Week 1

*Names below are suggested owners. The Trello board is a pull system — anyone can claim any card.*

1. UE 5.8.4 + repo/LFS — everyone pushes a commit
2. Discord topics + `#info` pins + Trello board (board link: https://trello.com/b/MUOJqRCI/candlelightdevelopment)
3. Fermin: wireframes at 1280×720 + decide the open questions
4. knightsGamer: **2.5D look test** — orthographic camera + lit 3D candle
5. Prototype: move, jump, collect on one platform
6. Template decision — see `GDD.md` §4: UE's 2D Platformer template is a **reference only**, not our project base

## Fermin must decide by end of week 1

D00 final title · D01 collectible count · D02 candle expiry · D03 room count · D04 the three mechanics · D05 portal theme · D06 collectible name

## Links

**Full GDD (single source of truth):** `GDD.md`
**GitHub docs repo:** https://github.com/A-r-Ash/ProjectCandleLight
Docs: `J:\UnrealProjects_Team\Project November 2026\Docs\`
Board: https://trello.com/b/MUOJqRCI/candlelightdevelopment
