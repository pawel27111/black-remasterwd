# BLACK: test arena (first playable milestone)

This repository now contains a small standalone Unreal Engine 5.4 C++ project.
It creates a graybox test arena from built-in engine meshes at runtime, so the
first test requires no imported art or binary map assets. The `BlackPrototype`
module is the currently selected game module. The previous `BlackRemastered`
source module and planning documents remain in the repository for later
integration; that older module is not selected by either build target because
it contains unresolved dependencies and Unreal compilation errors.

## Run on a Windows PC

1. Install Unreal Engine 5.4 and Visual Studio 2022 with **Game development
   with C++** and the Windows SDK. Other engine versions may need adjustments.
2. Clone or download this repository, keeping its directory structure. Do not
   copy individual `.cpp` files into another project.
3. Open `BlackRemastered.uproject`. If Unreal asks to build missing modules,
   allow it. If project files are needed, right-click the `.uproject` file and
   select **Generate Visual Studio project files**, then build
   `BlackRemasteredEditor` for your installed engine.
4. In the editor, open the default **Entry** map if needed and press **Play**.
   Click the viewport to capture the mouse.

Controls: WASD move, mouse aim, left mouse button fire, R reload, Shift sprint,
Space jump, F5 restart the arena, Escape pause/resume. Destroy the four hostile
cylinders. Cube covers block sight and can be destroyed. The HUD shows health,
ammo and surviving enemies. Targets can damage the player when they have line
of sight. Death resets the round.

## What this milestone proves

It provides a project file, game and editor targets, an entry module, game
mode, first-person character, hitscan weapon, damageable targets, cover, HUD,
and an arena generated at runtime. This is a simple test scene, **not the eight
campaign missions** described in `Docs/LEVEL_DESIGN.md`. It has no authored
assets, audio, AI navigation, save game, or true fracture/voxel destruction.

The `Docs/PROJECT_STATUS.md` roadmap predates this work. Until the first
successful Unreal build and Play-in-Editor test, treat this milestone as
**implemented in source, not engine-verified**. If the engine reports an error,
keep the first error line and the preceding build context; those details are
needed to correct the exact installed Unreal version.
