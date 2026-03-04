#include "File.hpp"

#include <fstream>
#include <sstream>

namespace Flock::FileIo {
    std::optional<std::string> ReadTextFile(const std::filesystem::path &filePath) {
        std::ifstream file(filePath);

        if (!file.is_open()) {
            Debug::LogErr("FileIo::ReadTextFile: Error opening file!");
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
}
