#include "Application.hpp"

#include <chrono>

namespace atomic {

int Application::run() {
    initialize();

    using clock = std::chrono::steady_clock;
    auto previous = clock::now();

    constexpr double maxDelta = 0.25;
    constexpr int maxFrames = 1;

    // Foundation loop: a bounded first frame keeps the core executable deterministic
    // until the platform/window layer is introduced.
    for (int frame = 0; frame < maxFrames; ++frame) {
        const auto now = clock::now();
        double delta = std::chrono::duration<double>(now - previous).count();
        previous = now;
        if (delta > maxDelta) delta = maxDelta;
        update(delta);
    }

    shutdown();
    return 0;
}

} // namespace atomic
