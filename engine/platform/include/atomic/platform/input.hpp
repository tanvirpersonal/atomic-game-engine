#pragma once

#include <cstddef>
#include <cstdint>

namespace atomic {

enum class Key : std::uint16_t {
    Unknown = 0,
    A, D, S, W,
    Escape,
    Space,
    Left, Right, Up, Down,
    Count
};

class Input {
public:
    void begin_frame() noexcept;
    void set_key(Key key, bool down) noexcept;

    [[nodiscard]] bool down(Key key) const noexcept;
    [[nodiscard]] bool pressed(Key key) const noexcept;
    [[nodiscard]] bool released(Key key) const noexcept;

private:
    static constexpr std::size_t key_count = static_cast<std::size_t>(Key::Count);
    bool current_[key_count]{};
    bool previous_[key_count]{};
};

} // namespace atomic
