# Candlelight — Discord Setup

## Final channels and topics

Paste these into each channel's **Topic** field:

| Channel | Topic |
|---|---|
| `#general` | Team chat + daily standup (Done / Doing / Blocked). Decisions go to Docs, not chat. |
| `#info` | READ FIRST. Repo, board, docs, UE version, timezones, commit rules. Pins only. |
| `#design` | Locked design + link to `GDD.md` §1/§2. Threads: `parking-lot` for ideas. |
| `#levels` | Hub + puzzle-room layouts, blockout screenshots, pacing. Threads: `Room 01`, `Room 02`. Jacob (lead 3D) + Fermin (design). |
| `#coding` | Implementation, PRs, architecture questions. Arashoo + Jon + knightsGamer. |
| `#art` | All visual work; Jacob leads 3D. Thread: `3d-pipeline` — Jacob + knightsGamer for scale/units/materials/imports. |
| `#audio` | Shags. Cue list (`GDD.md` §6), music states, sources. |
| `#qa` | Threads: `bugs` (one thread per bug) and `playtests` (one per session). |
| `#builds` | Weekly build drop: version + changelog + "play this". |
| `#off-topic` | Everything else. Keep `#general` usable. |

Voice: `🔊 Lounge` (drop-in, standups) · `🎧 Focus / Pairing` (silent co-work or pair programming).

Removed/merged: `concepts` → `#design` + `parking-lot` thread · `timezones` → pinned table in `#info` · `music` + `sfx` → `#audio` · `story` → not needed · `bugs` + `playtesting` → `#qa`.

## Pinned `#info` text template

```
CANDLELIGHT — READ FIRST

Repo: https://github.com/A-r-Ash/ProjectCandleLight  ·  Board: https://trello.com/b/MUOJqRCI/candlelightdevelopment  ·  Docs: J:\UnrealProjects_Team\Project November 2026\Docs\
Official GDD (single source of truth): [Google Docs link]
Engine: Unreal 5.8.4  ·  Platform: Windows PC @ 1280×720  ·  MVP: 4 weeks (hub + 2 puzzle rooms + candle timer, playable only)  ·  Sprint length: 1 week
Concept: Captain Toad-style puzzle rooms. Collect light before the candle burns out. Teleport hub via power outlet / air canals.
30-second handout: Docs/00_START_HERE.md

TEAM / TIMEZONES (all times local)
Arashoo (lead, gameplay) — [TZ, hours]
Jon (programmer) — [TZ, hours]
Jacob (lead 3D — all 3D art/assets, rooms, kit, mechanics) — [TZ, hours]
knightsGamer (3D pipeline + look test, supports Jacob) — [TZ, hours]
Fermin (design, 2D, UI) — [TZ, hours]
Shags (audio) — [TZ, hours]
Krassus (all-rounder) — [TZ, hours] — STATUS: [CONFIRMED / UNCONFIRMED]

RULES
1. Push at least once a day. No laptop-only work.
2. Standup daily in #general: Done / Doing / Blocked.
3. Weekly build in #builds with version + changelog.
4. Discord is not the GDD. Decisions get written into Docs and linked here.
5. Voice decisions don't exist until posted in text.
6. New ideas -> #design parking-lot thread, never straight into the build.
```

## Thread rules

- **Channel** = ongoing work area with daily traffic.
- **Thread** = something with a finish line: a bug, a playtest, a design decision, an asset review.
- **Pin** = information that never expires.
- Archive threads that go quiet for two days. Don't create a new channel to escape a busy one.
- Optional: `submission` thread in `#general` for the presentation page, screenshots, trailer, deadline checklist.

## Report formats

**Bug (own thread in `#qa`)**
```
Title: [Build 0.3] Falls through platform at room 2 start
Build:
Steps:
Expected:
Actual:
Severity: Crash / Blocking / Minor
Reporter:
```

**Playtest (thread in `#qa`)**
```
Tester: (cold player?)
Build:
Top 3 confusion moments:
One thing they liked:
One thing they disliked:
Actions entered into #qa bugs: yes/no
```
