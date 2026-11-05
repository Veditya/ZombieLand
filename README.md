# Game Engine

A small C++17 2D game engine and sample game built with SFML and tmxlite.
The project includes a menu, a tile-map-based play scene, entity components,
basic movement and collision handling, and an orthogonal TMX map renderer.

## Requirements

- CMake 3.16 or newer
- A C++17 compiler
- SFML 2.6 with the graphics, window, and system components
- tmxlite

The CMake configuration looks for SFML and tmxlite in the bundled
`external/sfml` and `external/tmxlite` directories. Keep those dependencies
available when building unless you update `CMakeLists.txt` to use other
installations.

## Build

Use a new build directory for the first configure:

```sh
cmake -S . -B build-local
cmake --build build-local --parallel
```

The executable is created at `build-local/Game-Engine`. CMake copies the
contents of `assets/` into the build directory and configures the executable to
load assets from there.

If CMake reports that a `CMakeCache.txt` was created for a different source
directory, configure into another fresh directory (for example,
`build-local-2`). This commonly happens when a build directory is copied from
another checkout. Build directories are ignored by Git.

## Run

```sh
./build-local/Game-Engine
```

## Project layout

- `src/` — engine services, entities, components, scenes, input, and entry point
- `assets/` — TMX maps, tilesets, images, and game textures
- `external/` — bundled SFML and tmxlite dependencies
- `CMakeLists.txt` — engine library, executable, dependencies, and asset setup
- `documentation.txt` — additional architecture and scene-extension notes

CMake builds the reusable `game_engine` static library separately from the
`Game-Engine` application executable.

## Adding a scene

Implement a `Scene` subclass, then register a factory with
`GameEngine::registerScene` in `src/main.cpp`. Construct each scene with the
engine reference and its configured name. Sources under `src/` are discovered
by CMake automatically.

## Configuration

`EngineConfig` in `src/EngineConfig.hpp` centralizes window dimensions, input
bindings, player defaults, asset names and paths, and map object identifiers.
Pass a customized configuration to `GameEngine` to change those defaults
without editing engine systems.
