# Candlelight — Task Board Setup

## Tool choice

| Tool | Verdict |
|---|---|
| **Trello** | **Recommended.** Visual, zero training, free tier fits 7 people. Best fit for artists, sound, and beginners. |
| GitHub Projects | Good alternative **if** your repo is on GitHub — tasks link to commits/PRs. More friction for non-programmers. |
| Notion | Only if you want docs + board in one place. Risk: it duplicates the Docs folder, which breaks one-source-of-truth. |
| Linear / Jira | Overkill for 4 weeks and 7 people. |

Pick Trello unless the repo is already on GitHub and everyone is comfortable with issues.

## Board name

`Candlelight — MVP (4 weeks)`

## Lists

| List | Purpose |
|---|---|
| `Parking Lot` | Unscheduled ideas — mirrors `GDD.md` §7 |
| `Week 1` … `Week 4` | **Available work. Unassigned. Anyone may claim a card.** |
| `Doing` | Claimed work. Exactly one member per card. **WIP limit: max 2 cards per person** |
| `Review / Playtest` | Someone else verifies the five gates |
| `Done` | All five gates passed **in the sprint build** |
| `Phase 2 — Later` | Accepted post-MVP work. Not allowed in a sprint |

Card flow: `Week N` (unclaimed) → `Doing` (claimed) → `Review / Playtest` → `Done`.

## Card template

**Title:** `Task name` — no owner in the title. Suggested owner goes in the description.

**Description:**
```
Suggested owner: [name] — guidance only, anyone may claim this
Helpers: [none / names — helpers are NOT Trello members]
Plan task: GDD.md §5
Spec: GDD.md §2 / §8
DONE WHEN… (five gates): see GDD.md §5 — do not copy it here
Build verified in: v0._
Blocked by: [card link or none]
```

**Checklist (one item per gate):**
- Logic
- Visual
- Audio
- UX
- Build

**Member field = OWNER.** None while available; the claimer adds themselves as the **only Trello member**. Other contributors go in the description as `Helpers:` — do not add them as members, or the "Member = me" filter becomes wrong. **Due date:** end of the week.

## Labels (keep it under 15)

- **Role:** `Design`, `2D Art`, `3D Art`, `Pipeline`, `Code`, `Audio`, `QA`, `Lead`
- **Week:** `W1`, `W2`, `W3`, `W4`, `Phase2`
- **Type:** `Must`, `Should`, `Bug`, `Blocked`, `Decision`

## Cadence

- **Week start:** each person browses that week's list and **pulls** any card they can do — suggested owner is guidance, not permission. Add yourself as member, then move the card to `Doing`.
- **3D cards:** `3D Art` cards are suggested to **Jacob first** (lead 3D). `Pipeline` / look-test cards are suggested to **knightsGamer**, who supports Jacob. Anyone may still claim a card.
- **Conflict:** if two people want the same card, agree in `#general`; one person keeps it.
- **Standup 2–3×/week in `#general`:** each person posts Done / Doing / Blocked **with card links**.
- **Week end:** demo the build; a card moves to `Done` only when all five gates pass. Add one retro card with what to keep/change.
- **New ideas:** `Parking Lot` → weekly triage → write into `GDD.md` §7 **first** → then create the card.
- **Docs are the what/why; board is the who/by-when.** Never copy GDD text into cards — link to the doc.
- **Krassus:** no cards until he replies. Don't reserve work for him.

## Seed cards

Trello has no full CSV importer. Use `Trello_Import/HOW_TO_IMPORT.md` and the paste-ready `.txt` files there for card titles. `Board_Seed.csv` is the master data; `Trello_Import/CARD_DETAILS.md` gives every card's labels, due date, full description, and tailored five-gate checklist.

`Board_Seed.csv` contains all 25 MVP tasks plus decision cards and backlog/phase-2 seeds. Due dates are set: W1 = 2026-11-29, W2 = 2026-12-06, W3 = 2026-12-13, W4 = 2026-12-20.

## Discord

The board URL is pinned in `#info`: https://trello.com/b/MUOJqRCI/candlelightdevelopment. Don't automate card notifications into Discord until the team is used to checking the board — otherwise it becomes noise.
