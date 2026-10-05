#pragma once

#include "atomic/renderer/viewport.hpp"

namespace atomic {

class Renderer {
public:
    virtual ~Renderer() = default;

    virtual bool initialize(void* native_window, const Viewport& viewport) = 0;
    virtual void begin_frame() = 0;
    virtual void clear() = 0;
    virtual void end_frame() = 0;
    virtual void shutdown() = 0;
};

} // namespace atomic
