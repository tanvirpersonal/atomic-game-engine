#include "atomic/platform/input.hpp"

#include <utility>

namespace atomic {

void Input::begin_frame() noexcept {
    std::swap(current_, previous_);
}

void Input::set_key(Key key, bool down) noexcept {
    const auto index = static_cast<std::size_t>(key);
    if (index >= key_count) {
        return;
    }
    current_[index] = down;
}

bool Input::down(Key key) const noexcept {
    const auto index = static_cast<std::size_t>(key);
    return index < key_count && current_[index];
}

bool Input::pressed(Key key) const noexcept {
    const auto index = static_cast<std::size_t>(key);
    return index < key_count && current_[index] && !previous_[index];
}

bool Input::released(Key key) const noexcept {
    const auto index = static_cast<std::size_t>(key);
    return index < key_count && !current_[index] && previous_[index];
}

} // namespace atomic
