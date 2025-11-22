#include "Logger.hpp"

namespace Pixf::Core::Debug {
    uint16_t operator|(const LogLevel rhs, const LogLevel lhs) {
        return static_cast<uint16_t>(rhs) | static_cast<uint16_t>(lhs);
    }

    uint16_t operator&(const LogLevel rhs, const LogLevel lhs) {
        return static_cast<uint16_t>(rhs) & static_cast<uint16_t>(lhs);
    }

    uint16_t operator~(const LogLevel level) { return ~static_cast<uint16_t>(level); }

    std::string ToString(const LogLevel level) {
        switch (level) {
            case LogLevelTrace:
                return "Trace";
            case LogLevelDebug:
                return "Debug";
            case LogLevelInfo:
                return "Info";
            case LogLevelWarning:
                return "Warning";
            case LogLevelError:
                return "Error";
            case LogLevelFatal:
                return "Fatal";
            default:
                return "Unknown";
        }
    }

    std::string ToStringUpper(const LogLevel level) {
        std::string str = ToString(level);
        for (char &ch: str) {
            ch = std::toupper(static_cast<unsigned char>(ch));
        }

        return str;
    }

    Logger &Logger::Get(const std::string &name) {
        if (!s_Loggers.contains(name)) {
            s_Loggers.insert({name, Logger(name)});
        }

        return s_Loggers.at(name);
    }

    void Logger::Configure(const std::string &name, const LoggerConfig &config) {
        s_Loggers.insert({name, Logger(name, config)});
    }

    void Logger::Configure(const LoggerConfig &config) {
        m_Config = config;
    }

    LoggerConfig &Logger::GetConfig() {
        return m_Config;
    }

    Logger::Logger(const std::string &name, const LoggerConfig &config) : m_Name(name), m_Config(config) {}
} // namespace Pixf::Core::Debug
