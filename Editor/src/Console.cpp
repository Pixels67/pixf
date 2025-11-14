#include "Console.hpp"

#include <string>
#include <vector>

#include "Gui/Gui.hpp"
#include "Time/LocalTime.hpp"

namespace Pixf::Editor {
    std::string Console::SeverityToString(const Severity severity) {
        switch (severity) {
            case Severity::Error:
                return "ERROR";
            case Severity::Warning:
                return "WARN";
            case Severity::Info:
                return "INFO";
            default:
                return "UNK";
        }
    }

    void Console::Log(const std::string &message) {
        m_Entries.push_back({.message = message, .severity = Severity::Info, .time = Core::Time::GetLocalTime()});
    }

    void Console::LogWarning(const std::string &message) {
        m_Entries.push_back({.message = message, .severity = Severity::Warning, .time = Core::Time::GetLocalTime()});
    }

    void Console::LogError(const std::string &message) {
        m_Entries.push_back({.message = message, .severity = Severity::Error, .time = Core::Time::GetLocalTime()});
    }

    void Console::RenderAsGui() const {
        for (const auto &[message, severity, time]: m_Entries) {
            Core::Gui::Text("[%s] [%s] %s", time.ToString().c_str(), SeverityToString(severity).c_str(),
                            message.c_str());
        }
    }
} // namespace Pixf::Editor
