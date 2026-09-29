#include "atomic/core/log.hpp"
#include "atomic/core/time.hpp"

#include <iostream>

int main() {
    atomic::FrameClock clock;
    clock.tick();

    atomic::log(atomic::LogLevel::Info, "Atomic Game Engine core initialized");

    std::cout
        << "frame=" << clock.frame_index()
        << " delta=" << clock.delta()
        << " fixed_step=" << clock.fixed_step()
        << '\n';

    return 0;
}
