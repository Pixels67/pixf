#ifndef PIXF_LOGGER_HPP
#define PIXF_LOGGER_HPP

#include <fmt/chrono.h>
#include <fmt/format.h>

#include "Common.hpp"

namespace Pixf::Core::Debug {
    enum LogLevel : uint16_t {
        LogLevelNone = 0,
        LogLevelTrace = 1 << 0,
        LogLevelDebug = 1 << 1,
        LogLevelInfo = 1 << 2,
        LogLevelWarning = 1 << 3,
        LogLevelError = 1 << 4,
        LogLevelFatal = 1 << 5,
        LogLevelAll = 0b111111,
    };

    PIXF_API uint16_t operator|(LogLevel rhs, LogLevel lhs);
    PIXF_API uint16_t operator&(LogLevel rhs, LogLevel lhs);
    PIXF_API uint16_t operator~(LogLevel level);

    PIXF_API std::string ToString(LogLevel level);
    PIXF_API std::string ToStringUpper(LogLevel level);

    struct PIXF_API LoggerConfig {
        uint16_t visibility = LogLevelAll;
        std::string timeFormat = "%Y-%m-%d %H:%M:%S";
        std::string logFormat = "[{t}] [{L}] [{n}] {m}";
    };

    class PIXF_API Logger {
    public:
        static Logger &Get(const std::string &name);
        static void Configure(const std::string &name, const LoggerConfig &config = {});

        template<typename... Args>
        void Print(fmt::format_string<Args...> format, Args &&...args) {
            std::cout << fmt::format(format, std::forward<Args>(args)...);
        }

        template<typename... Args>
        void PrintLn(fmt::format_string<Args...> format, Args &&...args) {
            std::cout << fmt::format(format, std::forward<Args>(args)...) << '\n';
        }

        template<typename... Args>
        void PrintError(fmt::format_string<Args...> format, Args &&...args) {
            std::cerr << fmt::format(format, std::forward<Args>(args)...);
        }

        template<typename... Args>
        void PrintErrorLn(fmt::format_string<Args...> format, Args &&...args) {
            std::cerr << fmt::format(format, std::forward<Args>(args)...) << '\n';
        }

        template<typename... Args>
        void Log(const LogLevel level, const char *format, Args... args) {
            if (!(m_Config.visibility & level)) {
                return;
            }

            const std::string message = fmt::format(fmt::runtime(format), args...);

            auto now = std::chrono::system_clock::now();
            const std::string timeFormat = "{:" + m_Config.timeFormat + "}";

            const std::string time = fmt::format(fmt::runtime(timeFormat), now);

            if (level == LogLevelFatal || level == LogLevelError) {
                PrintErrorLn(fmt::runtime(m_Config.logFormat),
                             fmt::arg("t", time),
                             fmt::arg("l", ToString(level)),
                             fmt::arg("L", ToStringUpper(level)),
                             fmt::arg("n", m_Name),
                             fmt::arg("m", message));
                return;
            }

            PrintLn(fmt::runtime(m_Config.logFormat),
                    fmt::arg("t", time),
                    fmt::arg("l", ToString(level)),
                    fmt::arg("L", ToStringUpper(level)),
                    fmt::arg("n", m_Name),
                    fmt::arg("m", message));
        }

    private:
        inline static std::unordered_map<std::string, Logger> s_Loggers;

        std::string m_Name;
        LoggerConfig m_Config;

        explicit Logger(const std::string &name, const LoggerConfig &config = {});
    };

#ifndef NDEBUG

#define PIXF_CORE_LOG_TRACE(...)                                                                                       \
    Pixf::Core::Debug::Logger::Get("Core").Log(Pixf::Core::Debug::LogLevelTrace, __VA_ARGS__);

#define PIXF_CORE_LOG_DEBUG(...)                                                                                       \
    Pixf::Core::Debug::Logger::Get("Core").Log(Pixf::Core::Debug::LogLevelDebug, __VA_ARGS__);

#else

#define PIXF_CORE_LOG_TRACE(...)
#define PIXF_CORE_LOG_DEBUG(...)

#endif

#define PIXF_CORE_LOG_INFO(...)                                                                                        \
    Pixf::Core::Debug::Logger::Get("Core").Log(Pixf::Core::Debug::LogLevelInfo, __VA_ARGS__);

#define PIXF_CORE_LOG_WARN(...)                                                                                        \
    Pixf::Core::Debug::Logger::Get("Core").Log(Pixf::Core::Debug::LogLevelWarning, __VA_ARGS__);

#define PIXF_CORE_LOG_ERROR(...)                                                                                       \
    Pixf::Core::Debug::Logger::Get("Core").Log(Pixf::Core::Debug::LogLevelError, __VA_ARGS__);

#define PIXF_CORE_LOG_FATAL(...)                                                                                       \
    Pixf::Core::Debug::Logger::Get("Core").Log(Pixf::Core::Debug::LogLevelFatal, __VA_ARGS__);
} // namespace Pixf::Core::Debug

#endif // PIXF_LOGGER_HPP
