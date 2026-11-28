# Vessels — Prototype Roster

Owner: Arashoo + Fermin · Status: **DRAFT** · Date: 27 Nov 2026

## Purpose

The spark/flame is the player. A vessel is the body the spark is currently living in. Every vessel gives one strong ability and takes away something else. The room is solved by swapping the spark into the body whose trade fits the obstacle.

## Scope note

The HTML prototype has 10 rooms, but only 7 unique vessel bodies so far: Candle, Match, Lantern, Torch, Wisp, Wax Puddle, Beacon. This roster adds 3 more — Ember Stone, Mirror Shard, Incense Stick — so the game has **10 prototype vessels**.

---

## Universal prototype rules

- All **10 vessel bodies exist in the room at the same time**. The spark lives in one of them; the others are dark but still physical.
- Every body has a **floating nameplate above it**. The active nameplate glows; inactive nameplates stay dim but readable.
- **Interact / E:** "SWAP SPARK → NEXT VESSEL." The spark arcs from the current body to the next in the cycle. Old body darkens, new body lights.
- **A/D or ←/→:** move the active vessel on the X vector. **Space:** jump. Every vessel responds to both inputs; the vessel changes speed, height, and weight feel.
- Inactive vessels keep their mass. A dark Candle can still hold a plate; a dark Beacon can still hold a ledge.
- Light radius, burn drain, and checkpoints follow the **spark**, not the body.

### Swap cycle

`Candle → Match → Lantern → Torch → Wisp → Wax Puddle → Beacon → Ember Stone → Mirror Shard → Incense Stick → Candle`

First prototype: **E only cycles forward**. Reverse cycling is a later decision.

---

## The 10 vessels

| # | Vessel | Unique visual / silhouette | Name label placement | Power / ability | Cost / tradeoff | X-move + jump read | Puzzle verb |
|---|---|---|---|---|---|---|---|
| 1 | **Candle** | Tall thick wax cylinder with drips and a fat wick | Brass tag above the flame | Biggest light, long burn, heavy body holds plates, acts as the clock/checkpoint | Slow, tall, cannot fit crawl gaps; high, lumbering hop | Heavy walk; short, thumping jump | **hold / illuminate** |
| 2 | **Match** | Thin wooden stick with red tip and pinprick flame | Tiny tag on the stick | Precise ignition, fits hairline gaps, fastest X speed | Burns out quickly, tiny light, cannot hold plates; fragile | Quick scurry; light, sharp hop | **thread / ignite** |
| 3 | **Lantern** | Glass box with metal cap, handle, and shielded flame | Hanging tag from the handle | Draft-proof flame, can hang on hooks, medium light | Bulky, cracks if dropped, slower than the match | Careful walk; controlled hop | **cross wind / hang** |
| 4 | **Torch** | Long handle with a wide burning head | Tag wrapped around the handle | Long reach — lights fuses and hits switches from a distance | Burns fast, too wide for tight slots | Long stride; high reach-y jump | **reach** |
| 5 | **Wisp** | Bare glowing spark with a short trailing tail | Floating label above the halo | Flies through hairline cracks, rises on drafts, no weight | Dim, dies on water, cannot press plates or push | Drifts on X; flutter jump / lift | **fly / rise** |
| 6 | **Wax Puddle** | Low amber pool with a slow ripple | Label floating on the surface | Flows under sealed doors and through grates | No light while liquid, slow, cannot push upward | Slithers on X; squash-hop only | **seep in darkness** |
| 7 | **Beacon** | squat pedestal with a rotating brass lens/head | Tag above the lens | Aimed beam stays latched after the spark leaves; opens light-locks | Very heavy, short hop, cannot light fine fuses | Slow shuffle; heavy stomp-jump | **aim / latch light** |
| 8 | **Ember Stone** | Rounded dark stone/cube with glowing cracks | Band around the stone | Survives water, sinks, melts ice, triggers heat switches | Almost no light, slow, low jump | Rolling walk; low thud jump | **melt / sink / heat** |
| 9 | **Mirror Shard** | Thin angled silver shard on a tiny foot | Etched tag on the edge | Redirects an existing beam around corners and through grates | No flame of its own, needs a light source, shatters on hard impact | Skitters on X; light, skinny hop | **reflect** |
| 10 | **Incense Stick** | Thin dark stick with ember tip and curling smoke | Tag on the smoke ribbon | Smoke reveals hidden vents, trips air switches, travels up shafts | Very dim, fragile, smoke can stir drafts that push you | Tiptoe walk; soft, floaty hop | **trace / signal** |

---

## Four-sided room structure

One **room** contains four **levels**, one per side. The puzzle on each side runs along that wall toward a corner; the corner advances the player to the next side.

```
             CORNER 1 → SIDE B
        ┌───────────────────────┐
 SIDE D │                       │ SIDE B
        │                       │
        └───────────────────────┘
             CORNER 0 ← SIDE A
```

- **Side A / Level 1:** teach move, jump, swap. Candle ↔ Match.
- **Side B / Level 2:** add Lantern, Torch. Wind and reach puzzles.
- **Side C / Level 3:** add Wisp, Wax Puddle, Ember Stone. No-weight, no-light, and heat/water puzzles.
- **Side D / Level 4:** add Beacon, Mirror Shard, Incense Stick. Combine aimed light, reflection, and smoke/air switches. Final corner = room exit.
- The spark **carries over** between sides in whatever vessel ended the previous side. That makes the swap state matter across the whole room.
- Corner nodes are checkpoints and also the visible "next level" portals.

---

## Feedback / swap contract

| Event | Visual | Audio | UI |
|---|---|---|---|
| E pressed | spark arc to next vessel, old flame shrinks, new flame flares | short zap + ignition pop | prompt "E · SWAP SPARK → NEXT VESSEL" pulses once |
| Swap denied / no next vessel | arc fizzles | soft denial click | prompt shakes |
| Vessel made active | nameplate glows, body gets possession ring | low hum changes pitch | active name stays lit |
| Inactive vessel used as weight | plate depresses, gate reacts | click + rumble | none — read the world |

---

## Edge cases

1. Player spams E during an arc: queue at most one extra swap; do not skip bodies.
2. Player jumps during a swap: motion continues in the destination body, scaled to that body's jump height.
3. Wax Puddle or Wisp enters water: Wisp extinguishes; Wax Puddle keeps flowing; Ember Stone sinks safely.
4. Beacon leaves a latched beam and the spark moves away: beam remains for the tuned duration, then fades.
5. Candle is dark but on a plate: plate stays pressed because mass is body-based, not light-based.
6. Mirror Shard has no source beam: it shows a "needs light" shimmer and cannot solve a lock.

---

## DONE WHEN

| Gate | Test |
|---|---|
| Logic | E cycles through all 10 bodies in order; X move + jump work for every body; mass and light rules match the table |
| Visual | All 10 silhouettes are distinct and every nameplate is readable at 1280×720 |
| Audio | Swap zap, ignition pop, denial click, and possession hum each fire exactly once per event |
| UX | A cold player can name the active vessel and predict one trade without reading a tooltip |
| Build | Above confirmed in the weekly build, not just the editor |

## Open decisions

| Question | Options | Chosen | Date |
|---|---|---|---|
| Reverse swap with Shift+E? | Yes / forward only | Forward only for first prototype | 27 Nov 2026 |
| Nameplate style | Brass tag / floating text / etched label | Mixed by vessel, per table | 27 Nov 2026 |
| Ember Stone, Mirror Shard, Incense Stick names final? | Keep / rename | Keep for prototype | 27 Nov 2026 |

## Tuning log

| Date | Field | Before | After | Evidence |
|---|---|---|---|---|
| 27 Nov 2026 | Vessel count | 7 unique bodies | 10 unique bodies | Prototype roster session |
