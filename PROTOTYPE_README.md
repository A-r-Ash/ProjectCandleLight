# Candlelight — Basic-Shape Prototype

## Run it

1. Open `CandleLightProject.uproject` in UE 5.8.
2. The editor opens on `CL_Level_01_CandleSide` automatically. Press **Play**.
3. Level 2: `Content/Candlelight/Maps/Levels/CL_Level_02_MatchSide`.
4. Level 3 placeholder: `Content/Candlelight/Maps/Levels/CL_Level_03_LanternSide`.

All three maps use `ACLPRoomGameMode`, which preserves placed actors and spawns vessels on `ACLPVesselSpawnPoint` actors. Each level starts at the left (low X) and progresses right (higher X). The old runtime test lane still exists if you open a template map, but it is no longer the default.

## Controls

| Input | Action |
|---|---|
| `A` / `←` | Move left |
| `D` / `→` | Move right |
| `Space` | Jump |
| `E` | Swap spark to the next vessel in the cycle |
| `Esc` / `P` | Pause / resume |
| `R` | Restart the current level (while paused) |
| `Q` | Exit the game (while paused) |

Input uses the project's config-driven bindings in `DefaultInput.ini`, so A/D and E behave the same in PIE and packaged builds. The side camera looks along -Y, so world +X is screen right and `D` moves right. In authored levels, the next vessel appears at the current spark position so swap puzzles stay in place.

A visible `CL_Exit_End` trigger box sits at the far-right end of each level. Touching it with a vessel fades the screen to black, then loads the next map: Level 1 → Level 2 → Level 3 placeholder. The GameMode also adds a runtime trigger as a fallback if a map has none.

Past each exit is a non-playable blockout preview of the next side, so you can see what is coming. Level 1 shows the Match tunnel, Level 2 shows the Lantern platforms, and Level 3 shows a "MORE TO COME" marker. The end wall is moved beyond the preview so it does not block the view.

Swap order: Candle → Match → Lantern → Torch → Wisp → Wax Puddle → Beacon → Ember Stone → Mirror Shard → Incense Stick → Candle.

## Camera and lighting

- Every vessel uses the same orthographic side camera from `ACLPVessel`. It sits at `Y=1500`, looks along `-Y`, and keeps `+X` on screen right.
- The back wall was moved behind the camera to `Y=2200`, so it no longer blocks the view.
- Each map has a warm key light from the camera side, a sky fill, three point-light fills, and a fixed-exposure post-process volume.
- These defaults apply to all ten vessel Blueprints and all three maps. `-PatchMaps` updates existing maps without rebuilding their geometry.

## Level 1 — Candle side

`CL_Level_01_CandleSide` is the first side of the room. It is a basic platformer:

- Default vessel: **Candle**.
- Move, jump, four ascending/descending platforms.
- Goal: the glowing `CL_Level_01_CORNER` marker at the far end.
- No puzzle systems yet — this level teaches movement only.

## Level 2 — Match side

`CL_Level_02_MatchSide` teaches the first swap puzzle:

1. Start as Candle.
2. Walk to the low tunnel. Candle's capsule is too tall to enter.
3. Press `E` to swap into Match. Match has a much smaller capsule and can crawl through.
4. Touch `CL_ReturnToCandle` at the far side of the tunnel. The spark returns to Candle at that spot.
5. Jump the short exit platforms and reach `CL_Level_02_CORNER`.

The corner goal requires Candle, so level 2 is only complete after the swap-back.

## Level 3 — Lantern side placeholder

`CL_Level_03_LanternSide` is a simple placeholder so Level 2's exit has somewhere to load. It uses Lantern, three platforms, and its own end trigger. It is ready to be expanded with the next mechanic.

## Generated assets

| Asset | Use |
|---|---|
| `DA_Candle_Vessel` | Tuning for Candle speed, jump, gravity, light, mass, burn time, collision capsule and visual mesh size. |
| `DA_Match_Vessel` | Tuning reference for Match speed, jump, light, mass, small-gap flag, collision capsule and visual mesh size. |
| `BP_Vessel_*` | Ten openable Blueprint classes, one per vessel. Each shows the mesh, capsule radius, capsule half-height and visual size when opened. |
| `MI_CLP_Floor` / `MI_CLP_Wall` / `MI_CLP_Platform` | Reusable blockout colours for floors, walls and platforms. |
| `MI_CLP_Plate` / `MI_CLP_Gate` / `MI_CLP_Lock` | Materials for later weight-plate and light-lock puzzles. |
| `MI_CLP_CandleWax` / `MI_CLP_Flame` | Materials for Candle body and flame art later. |
| `MI_CLP_Collectible` | Material for light fragments. |
| `MI_CLP_Goal` | Gold material for corner goal markers. |
| `MI_CLP_Return` | Blue material for swap-back volumes. |
| `CL_Level_01_CandleSide` | Playable first side: Candle platformer. |
| `CL_Level_02_MatchSide` | Playable second side: Match crawl + return to Candle. |
| `CL_Level_03_LanternSide` | Lantern placeholder side with an end trigger. |
| `ACLPLevelExitTrigger` | Visible box trigger that starts the level transition on vessel overlap. |
| `ACLPTransitionHUD` | Draws the short black fade before the next level loads and the pause menu overlay. |
| `ACLPPausePlayerController` | Handles Esc/P, restart and quit input, including while paused. |

## Editing vessel size and collision

### Vessel Blueprints — recommended

Open `Content/Candlelight/Blueprints/Vessels/` and double-click any vessel Blueprint:

- `BP_Vessel_Candle`
- `BP_Vessel_Match`
- `BP_Vessel_Lantern`
- `BP_Vessel_Torch`
- `BP_Vessel_Wisp`
- `BP_Vessel_WaxPuddle`
- `BP_Vessel_Beacon`
- `BP_Vessel_EmberStone`
- `BP_Vessel_MirrorShard`
- `BP_Vessel_IncenseStick`

In the Blueprint editor you get a full overview:

- **Components panel**: `CapsuleComponent`, `BodyMesh`, `DetailMesh`, `FlameMesh`, `CollisionPreview`, `FlameLight`, `NameLabel`, `SideCamera`.
- `CollisionPreview` is a translucent capsule shown in the Blueprint viewport only; it is hidden in game.
- Select `CapsuleComponent` to see **Capsule Radius** and **Capsule Half Height**.
- **Class Defaults → Vessel | Collision → Preset Capsule Radius / Preset Capsule Height** are editable. Half-height is derived from full height.
- **Class Defaults → Vessel | Visual Size** has body/flame mesh scale and offset.
- **Class Defaults → Vessel | Movement → Preset Move Speed** controls walk speed. Candle is currently set to `1000`.
- **Class Defaults → Vessel** has mass, light radius and jump feel.

Changes made in a vessel Blueprint are authoritative in game. The GameMode automatically resolves each vessel kind to its Blueprint, so existing maps do not need to be edited.

Current Blueprint sizes:

| Vessel Blueprint | Capsule radius | Capsule half-height | Full height |
|---|---:|---:|---:|
| `BP_Vessel_Candle` | `20` | `44` | `88` |
| `BP_Vessel_Match` | `5` | `20` | `40` |
| `BP_Vessel_Lantern` | `18` | `36` | `72` |
| `BP_Vessel_Torch` | `9` | `44` | `88` |
| `BP_Vessel_Wisp` | `10` | `20` | `40` |
| `BP_Vessel_WaxPuddle` | `26` | `12` | `24` |
| `BP_Vessel_Beacon` | `24` | `40` | `80` |
| `BP_Vessel_EmberStone` | `22` | `26` | `52` |
| `BP_Vessel_MirrorShard` | `6` | `40` | `80` |
| `BP_Vessel_IncenseStick` | `5` | `44` | `88` |

PIE logs the real values, for example:

`CLP Vessel CANDLE class=BP_Vessel_Candle_C source=BlueprintDefaults moveSpeed=1000.0 collision: radius=20.0 halfHeight=44.0 (height=88.0)`

### Tuning data assets

`DA_Candle_Vessel` and `DA_Match_Vessel` remain as seed/reference tuning assets. To make one override a Blueprint at runtime, open that vessel Blueprint, tick **Vessel | Tuning → bUseTuningAsset**, and optionally assign **TuningAsset**.

### Per-placed-vessel override

Select a `CL_Spawn_*` actor in a map. Under **Spawn | Size Override**, tick `bOverrideCollisionAndSize` and edit the capsule/mesh values. This overrides the Blueprint for that one placed vessel. You can also set **Spawn → Vessel Class Override** to a different vessel Blueprint.

## Reusable level actors

| Actor | Use |
|---|---|
| `ACLPVesselSpawnPoint` | Place one per vessel in a level; the GameMode spawns that body there. |
| `ACLGoalCorner` | End-of-side marker; can require a specific vessel. |
| `ACLSparkReturn` | Trigger volume that sends the spark into a chosen vessel. |
| `ACLPLockedGate` | Door that can be opened by plates, light locks, collectibles or goals. |
| `ACLPPressurePlate` | Weight plate; inactive vessels still apply their mass. |
| `ACLLightLock` | Opens a gate while an active Candle's light reaches it. |
| `ACLCollectible` | Light fragment collected only by the active vessel. |
| `ACLPRoomManager` | Counts collectibles and opens the exit gate when the room is complete. |

## Rebuild / regenerate

Build code:

```powershell
& 'J:\UnrealEngine\UE_5.8\Engine\Build\BatchFiles\Build.bat' CandleLightProjectEditor Win64 Development '-Project=J:\UnrealProjects_Team\CandleLightProject\CandleLightProject\CandleLightProject.uproject' -WaitMutex -FromMsBuild
```

Refresh tuning assets/materials and add any missing vessel Blueprints without touching hand-edited maps:

```powershell
& 'J:\UnrealEngine\UE_5.8\Engine\Binaries\Win64\UnrealEditor-Cmd.exe' 'J:\UnrealProjects_Team\CandleLightProject\CandleLightProject\CandleLightProject.uproject' -run=CLPLevelAuthoring -SkipMaps -unattended -nop4 -NoSplash
```

Optional: patch existing maps with the current exit triggers and lighting/camera wall position (the GameMode already resolves vessel classes automatically):

```powershell
& 'J:\UnrealEngine\UE_5.8\Engine\Binaries\Win64\UnrealEditor-Cmd.exe' 'J:\UnrealProjects_Team\CandleLightProject\CandleLightProject\CandleLightProject.uproject' -run=CLPLevelAuthoring -PatchMaps -unattended -nop4 -NoSplash
```

Full regenerate from scratch — this **overwrites hand-edited maps** and only recreates Blueprints that are missing:

```powershell
Remove-Item -LiteralPath 'J:\UnrealProjects_Team\CandleLightProject\CandleLightProject\Content\Candlelight' -Recurse -Force
& 'J:\UnrealEngine\UE_5.8\Engine\Binaries\Win64\UnrealEditor-Cmd.exe' 'J:\UnrealProjects_Team\CandleLightProject\CandleLightProject\CandleLightProject.uproject' -run=CLPLevelAuthoring -unattended -nop4 -NoSplash
```
