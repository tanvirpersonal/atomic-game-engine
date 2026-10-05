#pragma once

namespace atomic {

class Application {
public:
    Application() = default;
    virtual ~Application() = default;

    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    int run();

protected:
    virtual void initialize() {}
    virtual void update(double /*deltaSeconds*/) {}
    virtual void shutdown() {}
};

} // namespace atomic
