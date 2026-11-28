# User stories & Trello cards — Fermin

**Story author:** Fermin (game designer). **Scope/dates:** agreed by the team; Arashoo makes the call if needed.
**Board:** https://trello.com/b/MUOJqRCI/candlelightdevelopment · **Rules:** `GDD.md` §5 · `Board_Setup.md`

Yes — this is a game designer's job. Fermin turns the GDD into **player-facing stories** and puts them on Trello. The team still **pulls** the work; Fermin does not assign people.

## What Fermin does

1. Reads the relevant `GDD.md` section.
2. Writes one story: **"As a player, I want …, so that …"**. For setup/pipeline work: **"As a teammate, I want …, so that …"**.
3. Creates the Trello card with title, labels, due date, description and five-gate checklist.
4. Puts **one suggested owner in the description**, never in the title. Anyone may claim the card.
5. Sends new ideas to `Parking Lot` → weekly triage → `GDD.md` §7 **first** → then a sprint card.
6. Rewrites a story if it is vague, too big, or has no visible player/team benefit.

Fermin does **not**: assign members, put names in card titles, duplicate GDD text, or create MVP cards with no GDD backing.

## The three parts of every card

| Part | What it is |
|---|---|
| **Story** | The player/team benefit — why this exists |
| **Work** | What must be built, with a `GDD.md` section link |
| **Proof** | Five gates: Logic, Visual, Audio, UX, Build |

## Card template

**Title:** `Short task name` — no owner.

**Labels:** one role + one week + one type, e.g. `3D Art`, `W2`, `Must`.
**Due:** end of that week — W1 29 Nov · W2 6 Dec · W3 13 Dec · W4 20 Dec (2026).

**Description to paste:**

```
Story: As a <player / teammate>, I want <action>, so that <benefit>.
Why now: <MVP goal>. GDD.md §...
Suggested owner: <name> — guidance only, anyone may claim this
Helpers: <none / names — helpers are NOT Trello members>
Build verified in: v0._

DONE WHEN…
- Logic — <observable behaviour>
- Visual — <readable at 1280x720>
- Audio — <cue fires once / none required>
- UX — <a cold player/teammate understands>
- Build — <works in the weekly build, not just editor>
```

**Checklist:** Claimed · Logic · Visual · Audio · UX · Build.

## Worked example — Room 01 moving platform

**Title:** `Room 01 moving platform teaches timing`
**List:** `Week 2` · **Labels:** `3D Art`, `Design`, `W2`, `Must` · **Due:** `2026-12-06`
**Suggested owner:** Jacob (lead 3D) + Jon (code) — guidance only
**Helpers:** Fermin (layout), knightsGamer (import)

**Description to paste:**

> Story: As a player, I want to ride a moving platform to the second ledge, so that I can reach the collectible before the candle burns down.
>
> Why now: MVP mechanic 1 in Room 01. It teaches timing without text. GDD.md §2 and §5.
>
> Jacob owns the platform mesh, pivot and final-style material; Jon owns movement logic from the data asset. Fermin owns the layout; knightsGamer handles the import.
>
> DONE WHEN…
> - Logic — platform carries the player without clipping; path and speed come from the data asset
> - Visual — movement is readable against the dark background at 1280×720
> - Audio — movement cue fires once per pass
> - UX — a cold player waits and rides without a text prompt
> - Build — verified in v0.2, not just in the editor

**Checklist:** Claimed · Logic · Visual · Audio · UX · Build.

## 10-second check before Fermin clicks Create

- Does it say **"As a …, I want …, so that …"**?
- Is there **one** outcome only — no "and"?
- Is a **GDD section** linked?
- Is the suggested owner in the **description**, not the title?
- Are the five gates **observable** — not just "done"?
- Does the due date match the week?
- If it is not in the GDD, is it in `Parking Lot` instead?

## Another short example — technical card

**Title:** `Install UE 5.8.4 + repo + LFS`
**Description:**

> Story: As a teammate, I want a shared repo with Git LFS and an exact engine version, so that everyone opens the same project and can push work.
>
> Why now: Week 1 setup. GDD.md §4 and §5.
>
> Suggested owner: Arashoo + Jon — guidance only, anyone may claim this.
>
> DONE WHEN…
> - Logic — UE 5.8.4 opens the project; LFS tracks binary assets
> - Visual — 1280×720 editor screenshot posted in `#general`
> - Audio — no cue required for setup
> - UX — a new member clones and opens without asking anyone
> - Build — every present member has pushed a commit
