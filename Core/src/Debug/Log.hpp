#ifndef FLK_LOG_HPP
#define FLK_LOG_HPP

#include <SLog.hpp>

namespace Flock::Debug {
    using namespace SLog;
    
    template <typename... Args>
    void LogTrc(const std::string &format, Args... args) {
        Logger &logger = Logger::Create("Core", {.color = true});
        logger.Log(Trace, format, args...);
    }

    template <typename... Args>
    void LogDbg(const std::string &format, Args... args) {
        Logger &logger = Logger::Create("Core", {.color = true});
        logger.Log(Severity::Debug, format, args...);
    }

    template <typename... Args>
    void LogInf(const std::string &format, Args... args) {
        Logger &logger = Logger::Create("Core", {.color = true});
        logger.Log(Info, format, args...);
    }

    template <typename... Args>
    void LogWrn(const std::string &format, Args... args) {
        Logger &logger = Logger::Create("Core", {.color = true});
        logger.Log(Warning, format, args...);
    }

    template <typename... Args>
    void LogErr(const std::string &format, Args... args) {
        Logger &logger = Logger::Create("Core", {.color = true});
        logger.Log(Error, format, args...);
    }

    template <typename... Args>
    void LogFtl(const std::string &format, Args... args) {
        Logger &logger = Logger::Create("Core", {.color = true});
        logger.Log(Fatal, format, args...);
    }
}

#endif //FLK_LOG_HPP
