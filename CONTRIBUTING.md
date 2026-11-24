# How we work — Candlelight docs

This repository is the **transparent record of the project**. Everyone can see what was decided, who owns what, and how the game is changing.

- **Docs repo:** https://github.com/A-r-Ash/ProjectCandleLight
- **Trello board:** https://trello.com/b/MUOJqRCI/candlelightdevelopment

## Source of truth

1. **`GDD.md`** — the official game design document. If Discord or an old file disagrees, the GDD wins.
2. **Trello** — who owns which card and by when: https://trello.com/b/MUOJqRCI/candlelightdevelopment
3. **Discord** — discussion only. A decision does not count until it is written into `GDD.md`.

## Rules

- No work without a Trello card.
- Week lists are unclaimed work. Anyone may claim a card by adding themselves as the **only Trello member**, then moving it to `Doing`.
- WIP limit: 2 cards per person in `Doing`.
- Helpers go in the card description, not as Trello members.
- Definition of Done: **Logic + Visual + Audio + UX + Build**, all confirmed in the weekly build.

## How to update docs

1. Make a small, focused change.
2. Use a clear commit message: `docs: decide candle expiry behaviour (D02)`.
3. Pull before you push.
4. Link the Trello card in the commit message when there is one: `docs: update HUD (W1-03)`.
5. If a decision changes the game, update `GDD.md` first — then the card.

## Team

Arashoo (lead) · Jon (code) · Jacob (lead 3D — all 3D assets, rooms, kit and mechanics) · knightsGamer (3D pipeline + look test, supports Jacob) · Fermin (design/UI) · Shags (audio) · Krassus (pending — no tasks until he replies).
