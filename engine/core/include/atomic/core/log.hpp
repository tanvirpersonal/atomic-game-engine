#pragma once
#include <string_view>
namespace atomic {
enum class LogLevel { Debug, Info, Warning, Error };
void log(LogLevel level, std::string_view message) noexcept;
inline void log_debug(std::string_view m) noexcept { log(LogLevel::Debug, m); }
inline void log_info(std::string_view m) noexcept { log(LogLevel::Info, m); }
inline void log_warning(std::string_view m) noexcept { log(LogLevel::Warning, m); }
inline void log_error(std::string_view m) noexcept { log(LogLevel::Error, m); }
} // namespace atomic
