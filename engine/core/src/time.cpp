#include "atomic/core/time.hpp"

#include <algorithm>
#include <chrono>

namespace atomic {

namespace { using Clock = std::chrono::steady_clock; }

FrameClock::FrameClock(Seconds fixed_step, Seconds max_delta) noexcept
    : fixed_step_(fixed_step), max_delta_(max_delta) {}

void FrameClock::reset() noexcept {
    delta_ = 0.0;
    elapsed_ = 0.0;
    accumulator_ = 0.0;
    frame_index_ = 0;
}

Seconds FrameClock::tick() noexcept {
    static auto previous = Clock::now();
    const auto now = Clock::now();
    const auto raw = std::chrono::duration<Seconds>(now - previous).count();
    previous = now;
    delta_ = std::clamp(raw, Seconds{0.0}, max_delta_);
    elapsed_ += delta_;
    accumulator_ += delta_;
    ++frame_index_;
    return delta_;
}

bool FrameClock::consume_fixed_step() noexcept {
    if (accumulator_ < fixed_step_) return false;
    accumulator_ -= fixed_step_;
    return true;
}

} // namespace atomic
