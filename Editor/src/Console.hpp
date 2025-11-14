#ifndef CONSOLE_HPP
#define CONSOLE_HPP

#include <string>
#include <vector>

#include "Time/LocalTime.hpp"

namespace Pixf::Editor {

    class Console {
    public:
        enum class Severity {
            Info,
            Warning,
            Error,
        };

        struct ConsoleEntry {
            std::string message;
            Severity severity;
            Core::Time::LocalTime time;
        };

        static std::string SeverityToString(Severity severity);

        void Log(const std::string &message);
        void LogWarning(const std::string &message);
        void LogError(const std::string &message);

        void RenderAsGui() const;

    private:
        std::vector<ConsoleEntry> m_Entries;
    };
} // namespace Pixf::Editor

#endif // CONSOLE_HPP
