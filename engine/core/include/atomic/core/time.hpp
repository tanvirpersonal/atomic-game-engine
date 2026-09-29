#pragma once

#include <cstdint>

namespace atomic {

using Seconds = double;

class FrameClock {
public:
    explicit FrameClock(Seconds fixed_step = 1.0 / 60.0,
                        Seconds max_delta = 0.25) noexcept;

    void reset() noexcept;
    Seconds tick() noexcept;

    Seconds delta() const noexcept { return delta_; }
    Seconds elapsed() const noexcept { return elapsed_; }
    Seconds fixed_step() const noexcept { return fixed_step_; }
    Seconds accumulator() const noexcept { return accumulator_; }
    std::uint64_t frame_index() const noexcept { return frame_index_; }

    bool consume_fixed_step() noexcept;

private:
    Seconds fixed_step_;
    Seconds max_delta_;
    Seconds delta_ = 0.0;
    Seconds elapsed_ = 0.0;
    Seconds accumulator_ = 0.0;
    std::uint64_t frame_index_ = 0;
};

} // namespace atomic
