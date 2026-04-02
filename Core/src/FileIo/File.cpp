#include "File.hpp"

#include <fstream>
#include <sstream>

namespace Flock::FileIo {
    std::optional<std::string> ReadText(const std::filesystem::path &filePath) {
        std::ifstream file(filePath);

        if (!file.is_open()) {
            Debug::LogErr("FileIo::ReadText: Error opening file!");
            return std::nullopt;
        }

        std::stringstream content;
        std::string       line;
        while (std::getline(file, line)) {
            content << line << '\n';
        }

        file.close();

        return content.str();
    }

    bool WriteText(const std::filesystem::path &filePath, const std::string &text) {
        std::ofstream file(filePath);

        if (!file.is_open()) {
            Debug::LogErr("FileIo::WriteText: Error creating file!");
            return false;
        }

        file << text;
        return true;
    }
}
