#pragma once

#include <cstdint>

namespace atomic {

struct Viewport {
    std::uint32_t width = 1280;
    std::uint32_t height = 720;

    void resize(std::uint32_t new_width, std::uint32_t new_height) noexcept {
        width = new_width;
        height = new_height;
    }

    [[nodiscard]] float aspect_ratio() const noexcept {
        return height == 0 ? 0.0f : static_cast<float>(width) / static_cast<float>(height);
    }
};

} // namespace atomic
