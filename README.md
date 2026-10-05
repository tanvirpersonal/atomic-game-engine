# Atomic Game Engine

A performance-first C++20 game-engine foundation designed to grow into a reusable 2D/3D engine for sandbox, voxel, procedural-world, physics and space-simulation projects.

## Current status

The repository now has a real core-runtime foundation rather than only an architecture document:

- C++20 + CMake build
- `atomic_core` static library
- application lifecycle and main-loop foundation
- frame timing / fixed-step timing
- logging
- ECS module foundation
- runnable `atomic_core_smoke` example
- warning-enabled builds on GCC/Clang and MSVC

## Architecture direction

```text
Application
    |
    +-- Core
    |   +-- Time
    |   +-- Logging
    |   +-- Events
    |   +-- Memory
    |   +-- Jobs
    |
    +-- Platform
    +-- ECS
    +-- Scene / World
    +-- Renderer
    +-- Assets
    +-- Physics
    +-- Animation
    +-- Audio
    +-- Scripting
    +-- Editor
```

The implementation follows a data-oriented approach: predictable memory access, low allocation pressure, modular subsystems, fixed-step simulation and profiling-driven optimization.

## Build

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --parallel
```

Run the smoke test/example:

```bash
./build/atomic_core_smoke
```

## Roadmap

1. Core runtime and platform abstraction
2. ECS and archetype storage
3. Job system and parallel workloads
4. Renderer abstraction + first 3D backend
5. Scene/world + asset pipeline
6. Physics
7. Editor and profiler
8. Animation/audio
9. Scripting
10. Networking after the single-player runtime is stable

See [`PLAN.md`](PLAN.md) for the detailed architecture and long-term roadmap.
