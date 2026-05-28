# Abyss Hollow

Abyss Hollow is a first working C++ iteration of a 2D top-down, sound-focused survival/escape game framework. This build prioritizes clean state management, external assets/configuration, and clear extension points over gameplay content.

## Current Features

- SFML-powered windowing, graphics, input, and audio linkage.
- Main menu with **New Game**, **Continue**, **Settings**, and **Quit**.
- New Game opens a deliberately black gameplay screen.
- Escape in gameplay opens a pause menu with **Return to game**, **Quit to main menu**, **Quit to desktop**, and **Settings**.
- Settings menu supports editable resolution, display mode, and cosmetic sound sliders.
- Main and player volume sliders support mouse dragging and direct numeric entry.
- External asset/config folders are used for future textures, future sounds, settings, and saves; UI text defaults to Segoe UI from the Windows font directory.
- Stub systems are ready for world save/load, player save/load, echolocation, audio emitters, creature sounds, world generation, texture loading, and settings persistence.

## Suggested Folder Structure

```text
AbyssHollow/
├── assets/
│   ├── fonts/       # Optional project fonts; currently empty because Windows Segoe UI is used by default.
│   ├── sounds/      # Future ambience, creature, player, and UI audio.
│   └── textures/    # Future sprites, UI skins, tiles, and heart visuals.
├── configs/         # Editable defaults such as resolution and sound settings.
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
- Linux desktop dependencies required by SFML, such as X11/OpenGL/OpenAL development packages

### Configure and build

By default, CMake downloads SFML 2.6.1 automatically:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build -j2
./build/AbyssHollow
```

If SFML 2.6 is already installed on your system, disable fetching:

```bash
cmake -S . -B build -DABYSS_FETCH_SFML=OFF -DCMAKE_BUILD_TYPE=Debug
cmake --build build -j2
./build/AbyssHollow
```

The executable copies `assets/` and `configs/` beside the binary after each successful build, so runtime assets remain external and editable. No font file is bundled; `configs/settings.ini` points to `C:/Windows/Fonts/segoeui.ttf`, with runtime fallbacks to other standard Windows fonts.

## Controls

- Mouse: activate menu buttons and drag sliders.
- Click a slider number box, type a value, and press Enter for direct numeric input.
- Escape in gameplay: open pause menu.
- Escape in pause/settings: close the current overlay.

## Next Development Hooks

- `src/Player/PlayerSystem.*`: player movement, heart visibility, and echolocation input.
- `src/World/WorldSystem.*`: floors, exits, obstacles, and raycast bounce geometry.
- `src/Systems/AudioSystem.*`: main/player volume routing, creature sounds, and echo playback.
- `src/Systems/SaveSystem.*`: world, player, and settings persistence.
- `src/Systems/ResourceManager.*`: texture atlases, sound buffers, and external asset manifests.
