# Team Status & Visibility Rules

**Pin this in `#info`.** The goal: any member can answer three questions in under 10 seconds.

## The three-question test

| Question | Where you look |
|---|---|
| What am I doing right now? | Trello → filter **Member = me**, list `Doing` |
| What is everyone else doing? | Trello `Doing` list + the daily team-status pin in `#general` |
| What work is still free to take? | Trello `Week 1` … `Week 4` lists — cards there are unclaimed |
| What did we decide, and why? | `GDD.md` — pinned in `#info` |

If any answer is unclear, the system is broken — say so in `#general` and the lead fixes it that day.

## The rules

1. **Pull, don't push.** Cards in `Week 1`–`Week 4` are **available to anyone**. Claim one by adding yourself as its only Trello member, then move it to `Doing`. **The Trello member field is the owner** — the avatar on the card is how everyone sees who owns it. Helpers go in the description, not as members.
2. **Everything the team needs to know lives in one of three places:** Trello (who/by when), Docs (what/why), Discord (today's status + blockers).
3. **WIP limit 2 cards per person in `Doing`.** Finish or unblock before pulling more.
4. **If blocked at standup**, post the card link + who can help. The lead reassigns or removes the blocker the same day.
5. **No work without a card.** A card in `Doing` must have exactly one member and a due date. Finishing early? Pull the next free card. Don't invent work.
6. **Decisions in chat or voice don't count** until written into Docs within 24 hours. Then pin the link.
7. **New ideas go to Parking Lot**, never straight into a sprint.
8. **Krassus is pending** — shown as inactive until he replies. No cards reserved for him.

## Daily personal status — each member posts in `#general`

```
[Name] — [date]
Done:    [Trello card link] — one line
Doing:   [Trello card link] — one line
Next:    [Trello card link] — one line
Blocked: [none / card link + who can unblock]
```

Post 2–3× per week (more often if asked). Async is fine across timezones.

## Lead's daily team pin — one message, edited each day

```
TEAM STATUS — Week [N] — updated [date/time]

Arashoo:           [card] — one line
Jon:           [card] — one line
knightsGamer:  [card] — one line
Jacob:         [card] — one line
Fermin:        [card] — one line
Shags:         [card] — one line
Krassus:       PENDING — no reply yet

Blockers: [none / card + owner]
Build:    [version] in #builds
Board:    https://trello.com/b/MUOJqRCI/candlelightdevelopment
Docs:     [00_START_HERE.md link]
```

## Weekly rhythm

| When | What | Where |
|---|---|---|
| Start of week | Pull the week's cards, confirm owners | Trello + voice |
| 2–3× per week | Personal status with card links | `#general` |
| Daily | Lead edits the team-status pin | `#general` |
| End of week | Demo the build, move cards to Done, retro card | `#builds` + Trello |
| Anytime | Blockers and decisions | `#general` → Docs |

## Week 1 — exact first task for each person

Copy this into Trello as the current `Doing` cards. Until the board exists, this list is the source of truth.

| Person | Their one task right now | Done when |
|---|---|---|
| Arashoo | Setup UE 5.8.4 + repo + LFS | Every present member has pushed a commit |
| Jon | Move / jump / collect prototype on one platform | All five gates pass in the weekly build |
| knightsGamer | 2.5D look test — orthographic camera + lit 3D candle | Screenshot in `#art`; template decision written into `GDD.md` §4 |
| Jacob | Modular puzzle-room kit blockout (walls, floor, platform) | Pieces snapped together at 1280×720 with final-style materials |
| Fermin | Wireframes + decide D01–D06 | Hub, room HUD, win, lose pinned in `#design`; seven decisions written into `GDD.md` §1 |
| Shags | Cue list confirm + placeholder teleport / collect sounds | `GDD.md` §6 has owners and cue names; placeholders fire in build |
| Krassus | **PENDING — no task** | He replies in `#general`; then the lead assigns one Week 1 card |

## Why no automatic Discord plugin is needed

A bot can only post what someone tells it to. The visibility comes from the habit: one owner per card, one daily pin, one Docs index. Add automation later only if the habit sticks.
