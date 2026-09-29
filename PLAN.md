# Atomic Game Engine — Development Plan

## Vision

Atomic Game Engine (AGE) is intended to become a reusable, high-performance game engine rather than a single game codebase.

Primary goals:

- Data-oriented architecture
- Cache-friendly ECS
- Multithreaded job system
- Modern GPU rendering
- Efficient memory management
- Async asset streaming
- Large-world support
- Physics
- Animation
- Audio
- Scripting
- Networking later
- Editor tooling
- Built-in profiling and diagnostics

The engine should be capable of powering small 3D games, Minecraft-style voxel worlds, open worlds, procedural worlds, physics-heavy simulations, and space/sandbox games.

---

## 1. Engineering Principles

### Performance First

Performance is a design requirement from day one.

Priorities:

1. CPU cache efficiency
2. Low allocation frequency
3. Predictable frame time
4. Multithreading
5. GPU utilization
6. Minimal synchronization
7. Efficient memory layout
8. Culling
9. Streaming
10. Profiling-driven optimization

### Data-Oriented Design

Prefer:

- contiguous arrays
- packed component storage
- Structure of Arrays where useful
- batch processing
- predictable memory access

Avoid unnecessary:

- per-object heap allocation
- deep inheritance hierarchies
- virtual calls in hot loops
- pointer-heavy object graphs
- global mutable state

### Modular Architecture

Subsystems must have clear boundaries.

Core → ECS → Scene → Physics/Animation/Audio → Renderer

Rendering must not contain game-specific logic.

### Deterministic Where Practical

Fixed timestep simulation, physics, replay, procedural generation, and future networking should be designed with deterministic behavior where practical.

---

# 2. High-Level Architecture

    Atomic Game Engine
    │
    ├── Platform
    │   ├── Window
    │   ├── Input
    │   ├── File System
    │   ├── Threads
    │   └── Time
    │
    ├── Core
    │   ├── Application
    │   ├── Game Loop
    │   ├── Logging
    │   ├── Events
    │   ├── Memory
    │   ├── Job System
    │   └── Configuration
    │
    ├── ECS
    │   ├── Entity
    │   ├── Component
    │   ├── Archetype
    │   ├── Query
    │   └── Systems
    │
    ├── Scene
    │   ├── World
    │   ├── Transform
    │   ├── Camera
    │   ├── Light
    │   └── Serialization
    │
    ├── Renderer
    │   ├── Render Device
    │   ├── Render Graph
    │   ├── GPU Resources
    │   ├── Materials
    │   ├── Meshes
    │   ├── Shaders
    │   ├── Culling
    │   ├── Batching
    │   ├── Instancing
    │   └── Post Processing
    │
    ├── Physics
    │   ├── Broad Phase
    │   ├── Narrow Phase
    │   ├── Collision
    │   ├── Rigid Body
    │   └── Constraints
    │
    ├── Animation
    ├── Audio
    │
    ├── Assets
    │   ├── Asset Manager
    │   ├── Asset Cache
    │   ├── Mesh
    │   ├── Texture
    │   ├── Material
    │   ├── Shader
    │   └── Async Loading
    │
    ├── World
    │   ├── Spatial Partitioning
    │   ├── Streaming
    │   ├── LOD
    │   ├── Procedural Generation
    │   └── Large World Coordinates
    │
    ├── Scripting
    ├── Networking
    │
    └── Editor
        ├── Scene View
        ├── Inspector
        ├── Entity Hierarchy
        ├── Asset Browser
        └── Profiler

---

# 3. Runtime Loop

Target frame pipeline:

    Input
      ↓
    Event Processing
      ↓
    Fixed Simulation
      ↓
    Gameplay Systems
      ↓
    Animation
      ↓
    Visibility / Culling
      ↓
    Render Preparation
      ↓
    Render Graph
      ↓
    GPU Submission
      ↓
    Audio
      ↓
    Present

Simulation should use a fixed timestep such as 1/60 second while rendering may run at 30, 60, 120, 144, 240 FPS or other rates.

---

# 4. ECS

ECS is a core performance system.

## Entity

Use lightweight identifiers, conceptually:

    Entity = index + generation

This prevents stale entity references.

## Components

Components contain data only.

Examples:

- Transform
- Velocity
- Health
- Camera
- MeshRenderer
- RigidBody
- Collider
- Light
- AudioSource

## Systems

Systems operate on batches of component data.

Examples:

- MovementSystem
- PhysicsSystem
- AnimationSystem
- RenderPreparationSystem

---

# 5. Archetype Storage

Use archetype-oriented storage.

Entities with the same component composition share storage.

Example:

    Position + Rotation + Velocity

can live in one archetype while:

    Position + Rotation + MeshRenderer

uses another.

Benefits:

- cache-friendly iteration
- fast queries
- fewer pointer indirections
- efficient batch processing

Future optimization:

- chunk-based archetypes
- SIMD-friendly layouts
- parallel queries

---

# 6. Job System

The engine must eventually scale across CPU cores.

Architecture:

    Main Thread
        │
        ├── Job Queue
        ├── Worker 1
        ├── Worker 2
        ├── Worker 3
        └── Worker N

Planned features:

- worker threads
- work stealing
- job dependencies
- job groups
- fences
- parallel-for
- task graphs

Example:

    ParallelFor(entities, UpdateTransform)

should divide work automatically between workers.

The main thread should not become the bottleneck.

---

# 7. Memory System

Avoid frequent general-purpose heap allocations in hot paths.

Planned allocators:

- Linear allocator
- Stack allocator
- Pool allocator
- Frame allocator
- Object pool
- Persistent allocator

Frame memory should be reset efficiently every frame.

---

# 8. Renderer

The renderer should be designed around a modern GPU pipeline.

Core components:

- Render Device
- Command Queue
- Command Buffer
- Buffer Manager
- Texture Manager
- Shader Manager
- Pipeline Manager
- Material Manager
- Render Graph

---

# 9. Render Graph

Represent rendering as a dependency graph.

Example:

    Shadow Pass
         ↓
    Depth Pass
         ↓
    GBuffer
         ↓
    Lighting
         ↓
    Transparent
         ↓
    Post Processing
         ↓
    UI
         ↓
    Present

The graph should eventually manage:

- pass order
- resource dependencies
- resource lifetime
- synchronization
- transient resources

---

# 10. Rendering Optimizations

Required/long-term systems:

### Frustum Culling

Skip objects outside the camera frustum.

### Distance Culling

Skip objects beyond configurable ranges.

### Occlusion Culling

Avoid rendering geometry hidden behind other geometry.

### Instancing

Render many copies of a mesh efficiently.

### Batching

Reduce draw-call overhead.

### LOD

Use lower-detail representations at distance.

### GPU-Driven Rendering

Long-term target:

    CPU
      ↓
    Visibility Data
      ↓
    GPU Culling
      ↓
    Indirect Drawing

---

# 11. Materials and Shaders

Separate:

    Material Definition
          ↓
        Shader
          ↓
    GPU Pipeline

Features:

- shader caching
- pipeline caching
- material instances
- texture bindings
- constant/uniform buffers
- permutation management

Avoid unnecessary shader/pipeline recompilation.

---

# 12. Asset System

Use handles instead of raw ownership where practical.

Examples:

    AssetHandle<Texture>
    AssetHandle<Mesh>
    AssetHandle<Material>

Pipeline:

    File
      ↓
    Import
      ↓
    Validate
      ↓
    Process
      ↓
    Cache
      ↓
    Runtime Asset

Support asynchronous loading so asset I/O does not freeze the main loop.

---

# 13. World System

Target large-world capabilities:

- spatial partitioning
- grid/chunk worlds
- BVH/octree where appropriate
- streaming
- LOD
- origin rebasing
- large-world coordinates
- procedural generation hooks

Concept:

    Player Position
         ↓
    World Cell
         ↓
    Load Nearby Cells
         ↓
    Unload Distant Cells

This is important for sandbox, voxel, and space-simulation projects.

---

# 14. Physics

Physics uses a fixed timestep.

Pipeline:

    Broad Phase
        ↓
    Candidate Pairs
        ↓
    Narrow Phase
        ↓
    Contact Generation
        ↓
    Constraint Solver
        ↓
    Integration

Future features:

- rigid bodies
- static bodies
- colliders
- triggers
- character controller
- joints
- raycasts
- sweep tests
- spatial acceleration structures

Physics remains decoupled from rendering.

---

# 15. Input

Provide a unified abstraction for:

- keyboard
- mouse
- gamepad
- controller
- touch where supported

Prefer action-based input:

    Jump
    MoveForward
    Shoot
    Interact

Gameplay should not depend directly on platform-specific APIs.

---

# 16. Audio

Audio subsystem:

- audio device
- sound assets
- music
- effects
- sources
- listener
- spatial audio
- volume groups

Audio loading should be asynchronous where practical.

---

# 17. Animation

Target pipeline:

    Skeleton
       ↓
    Animation Clip
       ↓
    State Machine
       ↓
    Blend Tree
       ↓
    Pose
       ↓
    Skinning

GPU skinning can be added later.

---

# 18. Scripting

Expose a high-level scripting API only after the core runtime is stable.

Requirements:

- entity access
- component access
- events
- timers
- asset access
- safe API boundary
- hot reload where practical

Scripting must not compromise the performance of the native/core runtime.

---

# 19. Networking

Networking is deliberately a later subsystem.

Target:

    Client
      ↕
    Network Transport
      ↕
    Server

Future:

- snapshots
- replication
- interpolation
- client prediction
- server authority
- lag compensation
- entity ownership

Do not implement multiplayer before the core single-player runtime is stable.

---

# 20. Editor

Build editor tooling after the runtime foundation.

Planned windows:

- Scene View
- Game View
- Entity Hierarchy
- Inspector
- Asset Browser
- Console
- Profiler
- World/Scene settings

Features:

- entity creation
- component editing
- transform tools
- asset drag/drop
- scene save/load
- gizmos
- play/pause
- frame stepping

---

# 21. Profiler

Profiling is a first-class engine feature.

Track:

- total frame time
- CPU frame time
- GPU frame time
- simulation time
- render preparation
- draw calls
- triangles
- entities
- active jobs
- memory usage
- asset loading time
- physics time

Example display:

    Frame: 8.2 ms
    CPU:   4.1 ms
    GPU:   5.8 ms
    Draws: 412
    Triangles: 1.2M
    Entities: 48,230

---

# 22. Debugging

Built-in diagnostics:

- logging
- assertions
- graphics validation in development
- entity inspection
- physics debug drawing
- render debug views
- GPU markers
- memory statistics
- job-system diagnostics

Debug systems should be disabled or stripped in release builds where possible.

---

# 23. Build Configurations

At minimum:

    Debug
    Development
    Release

Debug:

- maximum validation
- assertions
- verbose logging
- debug visualization

Development:

- profiling
- diagnostics
- optimized runtime

Release:

- aggressive optimization
- minimal logging
- stripped debug features

---

# 24. Platform Strategy

Platform-specific code must stay isolated.

Possible directions:

### Browser-first

    Rust/C++
       ↓
    WebAssembly
       ↓
    WebGPU
       ↓
    Browser

### Native-first

    C++/Rust
       ↓
    Native Platform Layer
       ↓
    Vulkan / DirectX / Metal

### Hybrid

    Shared Engine Core
       ├── Native Backend
       └── Web Backend

The architecture must avoid locking the entire engine to one graphics API.

---

# 25. Language Decision

Candidates:

## Rust

Advantages:

- memory safety
- strong concurrency model
- good WASM support
- modern tooling
- fewer memory-safety bugs

## C++

Advantages:

- mature game-engine ecosystem
- maximum low-level control
- broad graphics API support
- extensive existing engine knowledge

## TypeScript

Advantages:

- excellent browser integration
- fast iteration
- straightforward WebGPU development

Disadvantage:

- less suitable for the deepest performance-critical engine core at very large scale.

Final language/backend selection must happen before implementation.

---

# 26. Target Repository Structure

    atomic-game-engine/
    │
    ├── engine/
    │   ├── core/
    │   ├── ecs/
    │   ├── platform/
    │   ├── renderer/
    │   ├── physics/
    │   ├── animation/
    │   ├── audio/
    │   ├── assets/
    │   ├── world/
    │   ├── scripting/
    │   └── networking/
    │
    ├── editor/
    ├── examples/
    ├── tests/
    ├── benchmarks/
    ├── tools/
    ├── assets/
    ├── docs/
    ├── PLAN.md
    └── README.md

The exact layout can evolve as implementation reveals better module boundaries.

---

# 27. Development Phases

## Phase 0 — Architecture

- [x] Create repository
- [x] Create master development plan
- [ ] Select language
- [ ] Select first platform
- [ ] Select graphics backend
- [ ] Define module boundaries

## Phase 1 — Core Runtime

Build:

- application
- platform abstraction
- window
- timing
- logging
- events
- configuration
- main loop

Milestone: a minimal executable starts, updates, renders, and shuts down cleanly.

## Phase 2 — Memory + Containers

Build:

- allocators
- handles
- IDs
- optimized containers
- memory tracking

Milestone: predictable ownership and reduced hot-path allocations.

## Phase 3 — ECS

Build:

- entities
- generations
- components
- archetypes
- queries
- systems
- lifecycle management

Milestone: efficiently process large entity counts.

## Phase 4 — Job System

Build:

- worker threads
- queues
- work stealing
- dependencies
- parallel-for
- synchronization

Milestone: engine workloads scale across CPU cores.

## Phase 5 — Renderer

Build:

- graphics device
- buffers
- textures
- shaders
- pipelines
- materials
- camera
- meshes
- basic lighting

Milestone: real-time 3D scene rendering.

## Phase 6 — Render Graph

Build:

- render passes
- resource graph
- synchronization
- transient resources

Milestone: scalable modern rendering architecture.

## Phase 7 — Scene + World

Build:

- transforms
- cameras
- lights
- scene serialization
- spatial structures
- streaming foundation

Milestone: reusable game worlds.

## Phase 8 — Asset Pipeline

Build:

- asset handles
- importers
- cache
- async loading
- hot reload foundation

Milestone: efficient content loading.

## Phase 9 — Physics

Build:

- collision
- broad phase
- rigid bodies
- constraints
- raycasts
- character controller

Milestone: stable fixed-timestep simulation.

## Phase 10 — Audio + Animation

Build:

- audio runtime
- spatial audio
- animation clips
- skeletons
- blending

Milestone: complete game presentation foundation.

## Phase 11 — Editor

Build:

- scene editor
- hierarchy
- inspector
- asset browser
- gizmos
- play mode

Milestone: usable visual development workflow.

## Phase 12 — Large Worlds

Build:

- chunks
- streaming
- LOD
- origin rebasing
- procedural generation hooks

Milestone: large sandbox/simulation worlds.

## Phase 13 — Networking

Implement only after the core runtime is stable.

## Phase 14 — Production Optimization

Profile real workloads and optimize measured bottlenecks.

Measure:

- CPU frame time
- GPU frame time
- memory
- loading time
- draw calls
- synchronization
- cache behavior

---

# 28. Benchmark Strategy

Every major subsystem should have benchmarks.

### ECS

Test:

- 10,000 entities
- 100,000 entities
- 1,000,000 entities

Measure:

- creation
- destruction
- iteration
- queries

### Job System

Measure:

- submission overhead
- scheduling overhead
- worker utilization
- parallel speedup

### Renderer

Measure:

- draw calls
- instanced objects
- visible entities
- CPU submission time
- GPU frame time

### Asset System

Measure:

- cold load
- cached load
- asynchronous load
- memory usage

---

# 29. Performance Rules

1. Never optimize without profiling.
2. Avoid allocations inside hot loops.
3. Prefer contiguous memory.
4. Prefer batch processing.
5. Minimize synchronization.
6. Avoid unnecessary virtual dispatch.
7. Reuse GPU resources.
8. Cache expensive work.
9. Stream large worlds.
10. Keep debug systems removable.
11. Measure CPU and GPU independently.
12. Keep frame-time variance low.
13. Prefer predictable performance over clever abstractions.
14. Do not add subsystems merely because they sound advanced.
15. Every abstraction must justify its runtime cost.

---

# 30. Quality Requirements

Every major subsystem should eventually have:

- unit tests
- integration tests
- benchmarks
- debug instrumentation
- documentation

Critical systems should also have stress tests.

---

# 31. Initial Stress Targets

These are aspirational targets and must be validated against real hardware.

- 100,000+ lightweight ECS entities
- 10,000+ active physics objects where practical
- large numbers of instanced meshes
- asynchronous asset streaming
- multithreaded simulation
- stable fixed timestep
- minimal frame allocations
- large streamed worlds

Real performance depends on workload and hardware.

---

# 32. Documentation

Target:

    docs/
    ├── architecture/
    ├── core/
    ├── ecs/
    ├── renderer/
    ├── physics/
    ├── assets/
    ├── world/
    ├── scripting/
    └── editor/

Major public APIs should be documented.

---

# 33. What NOT To Do

Do not:

- immediately build a huge editor
- put Minecraft-specific logic into the engine core
- hard-code one game
- add dependencies without reason
- optimize based only on theory
- make every subsystem global
- mix rendering and gameplay logic
- make every entity a heap object
- block the main thread on asset loading
- prematurely implement multiplayer
- create one giant monolithic engine file

---

# 34. First Real Milestone

The first milestone is not a complete game.

It is:

    Engine starts
       ↓
    Window opens
       ↓
    Main loop runs
       ↓
    ECS creates entities
       ↓
    Job system executes work
       ↓
    Renderer draws a 3D object
       ↓
    Profiler reports frame time
       ↓
    Clean shutdown

Once this works reliably, higher-level systems can be built on top.

---

# 35. Long-Term Goal

AGE should provide infrastructure while individual games provide:

- gameplay
- content
- rules
- assets
- worlds
- UI
- progression

Conceptually:

    Atomic Game Engine
            ↓
    ┌───────┼────────┐
    ↓       ↓        ↓
  Game A  Game B   Game C
    ↓       ↓        ↓
  RPG     Sandbox  Space Sim

---

# 36. Current Status

| System | Status |
|---|---|
| Repository | Ready |
| Master Plan | Complete |
| Language | To be selected |
| Platform | To be selected |
| Core | Not started |
| ECS | Not started |
| Job System | Not started |
| Renderer | Not started |
| Physics | Not started |
| Assets | Not started |
| World Streaming | Not started |
| Audio | Not started |
| Animation | Not started |
| Networking | Not started |
| Editor | Not started |
| Profiler | Not started |

---

# 37. Immediate Next Steps

1. Select implementation language.
2. Select first target platform.
3. Select first graphics backend.
4. Create repository architecture.
5. Implement Core runtime.
6. Add memory/handle infrastructure.
7. Implement ECS.
8. Add Job System.
9. Build first renderer.
10. Add profiling from the beginning.

---

## Final Principle

**Atomic Game Engine should be built as infrastructure first and as a game second.**

Every major decision should answer:

> Does this make the engine more reusable, measurable, scalable, and efficient?

If not, it should not enter the engine core.
