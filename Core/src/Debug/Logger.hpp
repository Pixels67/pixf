#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <cstdint>
#include <iostream>

#include "Time/LocalTime.hpp"

namespace Pixf::Core::Debug {
    enum class Severity : uint8_t { Info = 0, Warning = 1, Error = 2, Fatal = 3 };

    struct LoggerConfig {
        Severity logLevel = Severity::Info;
    };

    class Logger {
    public:
        static void Init(LoggerConfig config = {});
        static Logger &GetInstance();

        template<typename... Args>
        static void Print(Args... args) {
            ((std::cout << args), ...);
            std::cout << '\n';
        }

        template<typename... Args>
        static void PrintError(Args... args) {
            ((std::cerr << args), ...);
            std::cerr << '\n';
        }

        template<typename... Args>
        void Info(Args... args) {
            if (static_cast<int>(config.logLevel) <= static_cast<int>(Severity::Info)) {
                Print("[", Time::GetLocalTime().ToString(), "] [INFO] ", args...);
            }
        }

        template<typename... Args>
        void Warn(Args... args) {
            if (static_cast<int>(config.logLevel) <= static_cast<int>(Severity::Warning)) {
                Print("[", Time::GetLocalTime().ToString(), "] [WARN] ", args...);
            }
        }

        template<typename... Args>
        void Error(Args... args) {
            if (static_cast<int>(config.logLevel) <= static_cast<int>(Severity::Error)) {
                PrintError("[", Time::GetLocalTime().ToString(), "] [ERROR] ", args...);
            }
        }

        template<typename... Args>
        void Fatal(Args... args) {
            if (static_cast<int>(config.logLevel) <= static_cast<int>(Severity::Fatal)) {
                PrintError("[", Time::GetLocalTime().ToString(), "] [FATAL] ", args...);
            }
        }

    private:
        static Logger s_Instance;

        LoggerConfig config = {};
    };

#ifndef NDEBUG
#define PIXF_LOG_INFO(...) Pixf::Core::Debug::Logger::GetInstance().Info(__VA_ARGS__)
#define PIXF_LOG_WARN(...) Pixf::Core::Debug::Logger::GetInstance().Warn(__VA_ARGS__)
#else
#define PIXF_LOG_INFO(...)
#define PIXF_LOG_WARN(...)
#endif

#define PIXF_LOG_ERROR(...) Pixf::Core::Debug::Logger::GetInstance().Error(__VA_ARGS__)
#define PIXF_LOG_FATAL(...) Pixf::Core::Debug::Logger::GetInstance().Fatal(__VA_ARGS__)

} // namespace Pixf::Core::Debug

#endif // LOGGER_HPP
