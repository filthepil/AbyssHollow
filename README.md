# Abyss Hollow

Abyss Hollow is a first working C++ iteration of a 2D top-down, sound-focused survival/escape game framework. This build prioritizes clean state management, external assets/configuration, and clear extension points over gameplay content.

## Current Features

- SFML-powered windowing, graphics, input, and audio linkage.
- Main menu with **New Game**, **Continue**, **Settings**, and **Quit**.
- New Game loads `levels/level_01.json`, draws visible chunks with a temporary grey grid, and follows the player camera.
- JSON levels support tile dimensions, chunks, player spawns, and teleport exits to other level files.
- Escape in gameplay opens a pause menu with **Return to game**, **Quit to main menu**, **Quit to desktop**, and **Settings**.
- Settings menu supports editable resolution, display mode, and cosmetic sound sliders.
- Main and player volume sliders support mouse dragging and direct numeric entry.
- External asset/config folders are used for future textures, future sounds, settings, saves, and the placeholder UI font path.
- Stub systems are ready for world save/load, player save/load, echolocation, audio emitters, creature sounds, world generation, texture loading, and settings persistence.

## Suggested Folder Structure

```text
AbyssHollow/
├── assets/
│   ├── fonts/       # Project UI fonts. The expected placeholder is assets/fonts/main.ttf.
│   ├── sounds/      # Future ambience, creature, player, and UI audio.
│   └── textures/    # Future sprites, UI skins, tiles, and heart visuals.
├── configs/         # Editable defaults such as resolution, sound settings, and font_path.
├── levels/          # JSON-authored test levels, chunks, spawns, and teleport exits.
├── saves/           # Future world, player, slot, and settings save data.
├── src/
│   ├── Core/        # Game loop, settings, and state stack.
│   ├── Player/      # Future movement, visible heart, and echolocation triggers.
│   ├── States/      # Main menu, gameplay, pause, and settings screens.
│   ├── Systems/     # Resource, audio, and save/load boundaries.
│   ├── UI/          # Reusable buttons, menus, sliders, and text input.
│   └── World/       # Future floors, generation, collision, and raycast geometry.
└── CMakeLists.txt
```

## Build Instructions

### Requirements

- CMake 3.22 or newer
- A C++17 compiler
- Git, when using the default SFML FetchContent path
- Platform graphics/audio dependencies required by SFML
  - Windows: Visual Studio Build Tools or Visual Studio with the Desktop development with C++ workload
  - Linux: X11/OpenGL/OpenAL development packages for your distribution

### Configure and build

By default, CMake downloads SFML 2.6.1 automatically:

```text
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --parallel 2
```

If SFML 2.6 is already installed on your system, disable fetching:

```text
cmake -S . -B build -DABYSS_FETCH_SFML=OFF -DCMAKE_BUILD_TYPE=Debug
cmake --build build --parallel 2
```

Run the game from the repository root on Linux/macOS-like shells:

```text
./build/AbyssHollow
```

Run the game from the repository root on Windows PowerShell when using a single-config generator such as Ninja:

```text
.\build\AbyssHollow.exe
```

Run the game from the repository root on Windows PowerShell when using Visual Studio's default multi-config generator:

```text
.\build\Debug\AbyssHollow.exe
```

The executable copies `assets/`, `configs/`, and `levels/` beside the binary after each successful build, so runtime assets and level data remain external and editable. The default configuration expects the future project font at `assets/fonts/main.ttf`; this placeholder file is not included yet.

For a Windows-focused walkthrough, see [BUILD_WINDOWS.md](BUILD_WINDOWS.md).

## Controls

- Mouse: activate menu buttons and drag sliders.
- Click a slider number box, type a value, and press Enter for direct numeric input.
- WASD or Arrow keys in gameplay: move the placeholder player.
- Escape in gameplay: open pause menu.
- Escape in pause/settings: close the current overlay.

## Next Development Hooks

- `src/Player/PlayerSystem.*`: placeholder movement, future heart visibility, and echolocation input.
- `src/World/WorldSystem.*`: JSON level loading, chunk drawing, exits, future obstacles, and raycast bounce geometry.
- `src/Systems/AudioSystem.*`: main/player volume routing, creature sounds, and echo playback.
- `src/Systems/SaveSystem.*`: world, player, and settings persistence.
- `src/Systems/ResourceManager.*`: texture atlases, sound buffers, and external asset manifests.
