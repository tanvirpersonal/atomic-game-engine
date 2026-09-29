# Core Runtime Architecture

The Core runtime is intentionally small and independent of gameplay, editor, physics, and specific rendering APIs.

## Responsibilities

- application lifetime
- frame timing
- main loop
- logging
- events
- configuration
- platform-independent engine state

## Frame model

    start frame
       ↓
    process events
       ↓
    fixed simulation accumulator
       ↓
    variable update
       ↓
    render stage
       ↓
    end frame

## Timing

The runtime tracks delta time, fixed timestep, elapsed time, frame index, and accumulator. Frame delta is clamped so a long stall cannot create unbounded simulation catch-up.

## Next layers

1. platform abstraction
2. memory and handles
3. ECS
4. job system
5. renderer
