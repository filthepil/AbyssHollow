# Windows Build Guide

This guide builds Abyss Hollow on Windows with CMake and a C++17 compiler. Commands are written for PowerShell from the repository root.

## 1. Install prerequisites

1. Install **Git for Windows**.
2. Install **CMake 3.22 or newer** and make sure `cmake` is available on your `PATH`.
3. Install one of these C++ toolchains:
   - **Visual Studio 2022** or **Visual Studio Build Tools 2022** with the **Desktop development with C++** workload.
   - **Ninja** plus the Visual Studio C++ compiler tools, if you prefer a single-config Ninja build.

The default CMake configuration downloads SFML 2.6.1 for you, so a separate SFML install is optional.

## 2. Add the placeholder font asset

The current placeholder configuration expects a project font at:

```text
assets/fonts/main.ttf
```

The real font will be added later. Until then, place any temporary TrueType font at that path if you want to launch the UI locally. Keep the path unchanged unless you also update `font_path` in `configs/settings.ini`.

## 3. Configure and build with Visual Studio

From PowerShell in the repository root:

```powershell
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Debug --parallel 2
```

Run the Debug build:

```powershell
.\build\Debug\AbyssHollow.exe
```

For a Release build:

```powershell
cmake --build build --config Release --parallel 2
.\build\Release\AbyssHollow.exe
```

## 4. Configure and build with Ninja

If Ninja is installed and your Visual Studio developer environment is active:

```powershell
cmake -S . -B build-ninja -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build-ninja --parallel 2
```

Run the Ninja build:

```powershell
.\build-ninja\AbyssHollow.exe
```

## 5. Use an installed SFML instead of downloading

If SFML 2.6 is installed and discoverable by CMake, configure with fetching disabled:

```powershell
cmake -S . -B build -G "Visual Studio 17 2022" -A x64 -DABYSS_FETCH_SFML=OFF
cmake --build build --config Debug --parallel 2
```

If CMake cannot find your SFML install automatically, pass `SFML_DIR` to the folder containing `SFMLConfig.cmake`:

```powershell
cmake -S . -B build -G "Visual Studio 17 2022" -A x64 -DABYSS_FETCH_SFML=OFF -DSFML_DIR="C:\path\to\SFML\lib\cmake\SFML"
```

## 6. Runtime files

After every successful build, CMake copies these folders beside the executable:

- `assets/`
- `configs/`
- `saves/`

This means the game can load the same relative font path, `assets/fonts/main.ttf`, whether it is launched from the repository root or beside the built executable.

## 7. Troubleshooting

- If CMake cannot find Git while fetching SFML, install Git for Windows and reopen PowerShell.
- If CMake cannot find a compiler, run commands from a **Developer PowerShell for VS 2022** window or install the C++ workload.
- If the game starts and immediately reports a missing UI font, confirm that `assets/fonts/main.ttf` exists or update `font_path` in `configs/settings.ini`.
