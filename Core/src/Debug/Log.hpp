#ifndef FLK_LOG_HPP
#define FLK_LOG_HPP

#include <SLog.hpp>

namespace Flock::Debug {
    // I know this feels wrong but it's intended
    using namespace SLog;

    /**
     * Logs to the Flock logger with the Trace severity.
     * @tparam Args The types to log.
     * @param format The format string (fmt-style formatting).
     * @param args The variables to log.
     */
    template <typename... Args>
    void LogTrc(const std::string &format, Args... args) {
        Logger &logger = Logger::Create("Flock", {.color = true});
        logger.Log(Trace, format, args...);
    }

    /**
     * Logs to the Flock logger with the Debug severity.
     * @tparam Args The types to log.
     * @param format The format string (fmt-style formatting).
     * @param args The variables to log.
     */
    template <typename... Args>
    void LogDbg(const std::string &format, Args... args) {
        Logger &logger = Logger::Create("Flock", {.color = true});
        logger.Log(Severity::Debug, format, args...);
    }

    /**
     * Logs to the Flock logger with the Info severity.
     * @tparam Args The types to log.
     * @param format The format string (fmt-style formatting).
     * @param args The variables to log.
     */
    template <typename... Args>
    void LogInf(const std::string &format, Args... args) {
        Logger &logger = Logger::Create("Flock", {.color = true});
        logger.Log(Info, format, args...);
    }

    /**
     * Logs to the Flock logger with the Warning severity.
     * @tparam Args The types to log.
     * @param format The format string (fmt-style formatting).
     * @param args The variables to log.
     */
    template <typename... Args>
    void LogWrn(const std::string &format, Args... args) {
        Logger &logger = Logger::Create("Flock", {.color = true});
        logger.Log(Warning, format, args...);
    }

    /**
     * Logs to the Flock logger with the Error severity.
     * @tparam Args The types to log.
     * @param format The format string (fmt-style formatting).
     * @param args The variables to log.
     */
    template <typename... Args>
    void LogErr(const std::string &format, Args... args) {
        Logger &logger = Logger::Create("Flock", {.color = true});
        logger.Log(Error, format, args...);
    }

    /**
     * Logs to the Flock logger with the Fatal severity.
     * @tparam Args The types to log.
     * @param format The format string (fmt-style formatting).
     * @param args The variables to log.
     */
    template <typename... Args>
    void LogFtl(const std::string &format, Args... args) {
        Logger &logger = Logger::Create("Flock", {.color = true});
        logger.Log(Fatal, format, args...);
    }
}

#endif //FLK_LOG_HPP
