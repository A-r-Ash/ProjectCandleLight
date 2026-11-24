# [GAME TITLE] — GDD Blueprint (3-person team)

Copy this file into the new team's docs folder. Replace every `[bracket]`. Delete the example text once the real answers exist.

`§1` means **Section 1**.

**Status:** DRAFT / OFFICIAL · v0.1 · agreed `[date]`
**Document owner:** `[Your name]`
**Trello:** `[TBD]` · **GitHub:** `[TBD]` · **Google Docs:** `[TBD]`

---

## 0. Read this first — 30 seconds

**What we build:** `[One sentence: genre, player action, goal.]`

**Locked:** `[Engine/version]` · `[Platform]` · `[Resolution]` · `[Deadline]` · `[Sprint length]`

**MVP scope:**

- `[Core loop]`
- `[One environment / hub]`
- `[2–3 mechanics]`
- `[Win + lose + retry]`
- `[Packaged build]`

**Team seats:**

| Person | Owns | Also helps |
|---|---|---|
| `[Your name]` | Core code + builds | Makes the call if the team is stuck |
| `[Person 2]` | `[Art / levels / 3D]` | `[Implementation support]` |
| `[Person 3]` | Design + user stories + UI/audio | Playtesting |

No written team role title. If a decision has to be made, `[Your name]` makes the call and writes it into Section 1 within 24 hours. That is an action, not a status.

**Where things live:**

| Need | Place |
|---|---|
| What / why we build | **This GDD** |
| Who does what by when | Trello |
| Today's status / blockers | `TEAM_STATUS.md` + `#general` |
| Discussion only | Discord — decisions written here within 24 hours |
| Public / transparent record | GitHub docs repo |

---

## 1. Concept and decisions

**Player fantasy:** `[What the player feels: clever, sneaky, fast, calm?]`

**Core loop:** `[Enter] → `[Challenge]` → `[Reward]` → `[Return / next]`

**Win:** `[What ends the level successfully]`
**Lose:** `[What ends it badly]`
**Retry:** `[What resets and what stays]`

**Open decisions:**

| ID | Question | Options | Chosen | Date | Reason |
|---|---|---|---|---|---|
| D00 | Final title | `[…]` | `[TBD]` | | |
| D01 | Required collectibles / goals | `[…]` | `[TBD]` | | |
| D02 | Lose / retry behaviour | `[…]` | `[TBD]` | | |
| D03 | Number of levels / rooms | `[…]` | `[TBD]` | | |
| D04 | MVP mechanics | `[…]` | `[TBD]` | | |
| D05 | Level-select theme | `[…]` | `[TBD]` | | |
| D06 | Naming of key item | `[…]` | `[TBD]` | | |

Rule: a decision in chat or voice does not count until it is written here with a date and reason.

---

## 2. UX

**Screen flow:** `[Title] → `[Hub / level select]` → `[Level]` → `[Win / lose]` → `[Hub]`

**Input map:**

| Action | Input |
|---|---|
| Move | `[A/D or arrows]` |
| Jump | `[Space]` |
| Interact | `[E]` |
| Pause / mute | `[Esc]` |

**HUD priority:** `[Most important]` → `[Second]` → `[Third]`

**Feedback cue points:** `[Goal reached]`, `[Failure warning]`, `[Mechanic used]`, `[Level complete]`.

**Rules:** no overlap at `[resolution]`; every panel has a visible way out.

---

## 3. Art

**Look:** `[One paragraph: camera, lighting, foreground, background.]`

**Palette:**

| Role | Colour | Rule |
|---|---|---|
| Hero / focus | `[hex TBD]` | `[When it appears]` |
| World | `[hex TBD]` | `[When it appears]` |
| Danger | `[hex TBD]` | Reserved for warnings |
| Reward | `[hex TBD]` | Reads instantly |

**Scale / naming:** `1 unit = [X] cm` · pivots at `[base/centre]` · `GG_<Category>_<Name>_<Variant>`.

**Asset list:**

| Asset | Owner | Priority |
|---|---|---|
| `[Hero]` | `[Person 2]` | Must |
| `[Level kit]` | `[Person 2]` | Must |
| `[Key item]` | `[Person 2]` | Must |
| `[UI icons / HUD / panels]` | `[Person 3]` | Must |

**"Looks complete" checklist:** final-style material · silhouette readable · correct scale/pivot · present in weekly build. No "art pass later."

---

## 4. Tech

**Locked:** `[Engine/version]` · `[Platform]` · `[Resolution]` · `[Frame target]` · `[Camera type]`

**Template decision:** `[Template used]` — reference only for `[input / camera]`; final choice recorded here: `[TBD]`.

**Systems:**

| System | Owner | Done when |
|---|---|---|
| Input + movement | `[Your name]` | `[Observable test]` |
| Core mechanic 1 | `[Person 2 + your name]` | `[Observable test]` |
| Core mechanic 2 | `[Person 3 + your name]` | `[Observable test]` |
| Progress / save | `[Your name]` | `[Observable test]` |
| Win / lose / retry | `[Your name]` | `[Observable test]` |

**Data assets:** `[Timer]`, `[Speed]`, `[Counts]`, `[Level config]` — tune values here, not in code.

**Events:** `[Level entered]`, `[Goal collected]`, `[Warning]`, `[Mechanic used]`, `[Level complete]`.

**Build target:** `[Packaged build name]` posted in `#builds` every `[week]`.

---

## 5. Production plan

**Pull board rule:** names below are suggested owners only. Any person may claim any card, add themselves as the only Trello member, and move it to `Doing`.

| Week | Focus | Example cards | Due date |
|---|---|---|---|
| 1 | Setup + prototype + look test | `[Repo]`, `[Input]`, `[Core loop prototype]` | `[date]` |
| 2 | Core mechanic + first level | `[Mechanic]`, `[Level 01]`, `[Audio cues]` | `[date]` |
| 3 | Second mechanic + results | `[Mechanic 2]`, `[Level 02]`, `[Win/lose panels]` | `[date]` |
| 4 | QA + balance + package | `[Bug sweep]`, `[Balance]`, `[Final build]` | `[date]` |

**Standing rules:** cards in week lists are unclaimed · claim by adding yourself as the only member, then move to `Doing` · no work without a card · a `Doing` card has exactly one member and a due date · WIP limit 2 · decisions written into this GDD within 24 hours.

---

## 6. Audio

| Event | Cue | Priority | Owner | Status |
|---|---|---|---|---|
| Level entered | `[whoosh / sting]` | Must | `[Person 3]` | TBD |
| Goal collected | `[chime]` | Must | `[Person 3]` | TBD |
| Failure warning | `[tick / crackle]` | Should | `[Person 3]` | TBD |
| Mechanic used | `[click / rumble]` | Must | `[Person 3]` | TBD |
| Level complete | `[warm resolve]` | Must | `[Person 3]` | TBD |
| UI confirm | `[simple beep]` | Should | `[Person 3]` | TBD |

Rule: each Must cue fires once, at the right moment, in the weekly build.

---

## 7. Backlog

**Parking Lot:** ideas not in the MVP. Promote only after writing them into Section 1 and accepting them in a weekly triage.

- `[Idea]`
- `[Idea]`

**Phase 2 — Later:** accepted after MVP delivery. No work here during MVP sprints.

- `[Idea]`
- `[Idea]`

---

## 8. How to write a user story / card

**Story shape:** `As a [player], I want [action], so that [benefit].`
For setup work: `As a teammate, I want [action], so that [benefit].`

**Card title:** short task name. No person's name in the title.

**Description template:**

```text
Story: As a <player / teammate>, I want <action>, so that <benefit>.
Why now: <MVP goal>. GDD.md §...
Suggested owner: <name> — guidance only, anyone may claim this
Helpers: <none / names — NOT Trello members>
Build verified in: v0._

DONE WHEN…
- Logic — <observable behaviour>
- Visual — <readable at resolution>
- Audio — <cue fires once / none required>
- UX — <a cold player understands>
- Build — <works in weekly build>
```

**Worked example:**

**Title:** `Collect three keys to open the exit`
**Labels:** `Code`, `W2`, `Must` · **Due:** `[date]`
**Suggested owner:** `[Person 2]` — guidance only · **Helpers:** `[Person 3]`

> Story: As a player, I want to collect three keys, so that the exit opens and I can finish the level.
>
> Why now: MVP goal from GDD.md §1 and §5.
>
> DONE WHEN…
> - Logic — three unique keys counted; exit opens at 3/3
> - Visual — counter readable at `[resolution]`
> - Audio — chime fires once per key
> - UX — goal count visible on entry
> - Build — verified in the Week 2 build

**Checklist:** Claimed · Logic · Visual · Audio · UX · Build

**10-second check:** one outcome only · GDD section linked · one suggested owner in the description · five gates are observable · no members assigned yet.

---

## 9. Team operations

**Discord channels for 3 people:** `#general` (standup) · `#info` (pins only) · `#design` (stories + decisions) · `#builds` (weekly builds) · `#qa` (bugs + playtests).

**Trello lists:** `Week 1` · `Week 2` · `Week 3` · `Week 4` · `Parking Lot` · `Phase 2 - Later` · `Doing` · `Review / Playtest` · `Done`.

**Labels (15 max):**

- **Role:** `Design`, `Art`, `Code`, `Audio`, `QA`, `Coordination`
- **Week:** `W1`, `W2`, `W3`, `W4`
- **Type:** `Must`, `Should`, `Bug`, `Decision`, `Phase2`

**Daily personal status in `#general`:**

```text
[Name] — [date]
Done: [card link]
Doing: [card link]
Blocked: [card link + who can help]
```

**Daily team pin — one message, edited each day:**

```text
TEAM STATUS — Week [N] — updated [date/time]

[Your name]:     [card] — one line
[Person 2]:      [card] — one line
[Person 3]:      [card] — one line

Next build: [version] — [date]
Biggest risk: [one line]
Decisions needed: [one line or none]
```

**Definition of Done — five gates:** Logic · Visual · Audio · UX · Build. All five must pass **in the weekly build**, not just the editor.

---

## 10. File set to create for the new team

| File | Purpose |
|---|---|
| `GDD.md` | This document — single source of truth |
| `00_START_HERE.md` | One-page handout for Discord |
| `TEAM_STATUS.md` | Visibility rules + daily status format |
| `USER_STORIES.md` | How stories/cards are written |
| `Board_Setup.md` | Trello lists, labels, card template |
| `Board_Seed.csv` | Master card data |
| `Discord_Setup.md` | Channel topics and pins |
| `README.md` | Docs index and Definition of Done |

First messages to pin:

> **#info:** Official GDD `[link]` · Trello `[link]` · GitHub `[link]` · Engine `[version]` · Every task has one owner. WIP limit 2. Chat decisions count only when written in the GDD.

> **#general:** Trello is a pull board. Pick any card in the current week, add yourself as the only member, and move it to `Doing`. Names in descriptions are suggestions.
