#include "atomic/core/application.hpp"

#include "atomic/core/log.hpp"

namespace atomic {

int Application::run() {
    running_ = true;
    FrameClock clock;

    on_start();

    while (running_) {
        clock.tick();

        while (clock.consume_fixed_step()) {
            on_fixed_update(clock.fixed_step());
        }

        on_update(clock.delta());
        on_render();

        // Core-only applications have no platform event source yet.
        // Derived applications should call request_exit() when their
        // platform layer signals shutdown.
        if (clock.frame_index() == 1) {
            log(LogLevel::Debug, "Application loop started");
        }
    }

    on_shutdown();
    return 0;
}

} // namespace atomic
