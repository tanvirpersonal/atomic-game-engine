# Lesson 02 — Input: From OS Events to Game State

The goal of this lesson is to understand one important engine boundary:

```text
Operating System
      ↓
 Platform Backend
      ↓
  Input State
      ↓
    Game
```

## 1. Why not let gameplay read OS events directly?

A game should not need to know whether Windows, Linux, or another platform produced a key press. The platform layer translates native input into engine concepts such as `Key::W`.

That separation is called an **abstraction boundary**.

## 2. Three useful questions

For every key, gameplay usually needs three different answers:

- `down()` — is it currently held?
- `pressed()` — did it become held this frame?
- `released()` — did it stop being held this frame?

Example:

```cpp
if (input.down(Key::W)) {
    player.move_forward();
}

if (input.pressed(Key::Space)) {
    player.jump();
}
```

## 3. Why keep two states?

The engine stores:

```text
previous frame
current frame
```

Then:

```text
pressed  = current && !previous
released = !current && previous
down     = current
```

This is a small example of a general engine principle: **store data in a form that makes common queries cheap**.

## 4. Important limitation

This lesson intentionally does **not** connect to a real OS window yet. The input API is backend-independent first. In the next platform lesson we will connect a real window/event source to this interface.

## Exercise

Before moving on, try to answer:

1. Why would `pressed()` be wrong if we only stored one boolean?
2. Why should gameplay not depend on Linux/Windows key codes?
3. What should happen to `previous` and `current` at the start of a frame?

## Next

Real window creation + OS event polling.
