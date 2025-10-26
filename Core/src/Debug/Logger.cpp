#include "Logger.hpp"

namespace Pixf::Core::Debug {
    Logger Logger::s_Instance = {};

    void Logger::Init(const LoggerConfig config) {
        s_Instance.config = config;
    }

    Logger &Logger::GetInstance() {
        return s_Instance;
    }
} // namespace Pixf::Core::Debug
