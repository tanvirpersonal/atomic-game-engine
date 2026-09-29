#include "atomic/core/log.hpp"
#include <iostream>
namespace atomic {
void log(LogLevel level, std::string_view message) noexcept {
    const char* prefix = "[INFO]";
    switch (level) {
        case LogLevel::Debug: prefix = "[DEBUG]"; break;
        case LogLevel::Info: prefix = "[INFO]"; break;
        case LogLevel::Warning: prefix = "[WARN]"; break;
        case LogLevel::Error: prefix = "[ERROR]"; break;
    }
    std::clog << prefix << ' ' << message << '\n';
}
} // namespace atomic
