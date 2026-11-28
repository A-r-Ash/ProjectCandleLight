# Trello setup — pull board

**Board:** https://trello.com/b/MUOJqRCI/candlelightdevelopment

The board is a **pull system**: anyone may choose a card, claim it, and do it. Titles are task-only on purpose. The suggested owner is written in the card description, not forced onto the card.

Trello has **no full CSV importer**. It only turns pasted lines into card titles. `Board_Seed.csv` stays the master data for labels, due dates, suggested owners, and descriptions.

## Steps

1. Create these lists in order:
   `Week 1`, `Week 2`, `Week 3`, `Week 4`, `Parking Lot`, `Phase 2 - Later`, `Doing`, `Review / Playtest`, `Done`.
2. Open `Week 1.txt` → **Ctrl+A** → **Ctrl+C**.
3. In Trello, go to the **Week 1** list → **+ Add a card** → **Ctrl+V**. Each line becomes one card.
4. Repeat for `Week 2.txt`, `Week 3.txt`, `Week 4.txt`, `Parking Lot.txt`, `Phase 2 - Later.txt`.
5. For each card, copy the matching **Description** and **Due Date** from `Board_Seed.csv`.
6. Add the labels from `Board_Setup.md`. Do **not** assign members yet — cards stay unclaimed.
7. Invite the team. **Krassus: no card and no invite until he replies.**

## How a member claims work

1. Look in the current week's list.
2. Pick any card you can do — suggested owner is guidance, not permission.
3. **Add yourself as the only Trello member** on that card. That avatar is the owner — everyone can see it at a glance.
4. Put other contributors in the description as `Helpers: [names]`. Do not add them as members, or the "Member = me" filter becomes wrong.
5. **Move the card to `Doing`.**
6. If two people want the same card, agree in `#general`, then one person keeps it.
7. WIP limit: **max 2 cards per person in `Doing`.**

Now the three-question test works: unclaimed work is visible in the week lists; active work is visible in `Doing` with a member; done work is in `Done`.
