#pragma once

#include "atomic/core/time.hpp"

namespace atomic {

class Application {
public:
    Application() = default;
    virtual ~Application() = default;

    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    int run();
    void request_exit() noexcept { running_ = false; }

protected:
    virtual void on_start() {}
    virtual void on_fixed_update(Seconds) {}
    virtual void on_update(Seconds) {}
    virtual void on_render() {}
    virtual void on_shutdown() {}

private:
    bool running_ = false;
};

} // namespace atomic
