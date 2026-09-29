#include "atomic/core/time.hpp"

#include <algorithm>

namespace atomic {

FrameClock::FrameClock(Seconds fixed_step, Seconds max_delta) noexcept
    : last_tick_(Clock::now()),
      fixed_step_(fixed_step > 0.0 ? fixed_step : 1.0 / 60.0),
      max_delta_(max_delta > 0.0 ? max_delta : 0.25) {}

void FrameClock::reset() noexcept {
    last_tick_ = Clock::now();
    delta_ = 0.0;
    elapsed_ = 0.0;
    accumulator_ = 0.0;
    frame_index_ = 0;
}

Seconds FrameClock::tick() noexcept {
    const auto now = Clock::now();
    const auto raw = std::chrono::duration<Seconds>(now - last_tick_).count();
    last_tick_ = now;

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
