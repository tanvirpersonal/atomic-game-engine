#pragma once

#include <cstdint>
#include <string>

namespace atomic {

struct WindowDesc {
    std::uint32_t width = 1280;
    std::uint32_t height = 720;
    std::string title = "Atomic Game Engine";
};

class Window {
public:
    virtual ~Window() = default;

    virtual bool create(const WindowDesc& desc) = 0;
    virtual void poll_events() = 0;
    virtual bool should_close() const noexcept = 0;
    virtual void close() noexcept = 0;
    virtual void* native_handle() const noexcept = 0;
};

} // namespace atomic
