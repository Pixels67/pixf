#include "File.hpp"

#include <fstream>
#include <sstream>

#include "Debug/Log.hpp"

namespace Flock::FileIo {
    std::optional<std::string> ReadText(const std::filesystem::path &filePath) {
        std::ifstream file(filePath);

        if (!file.is_open()) {
            Debug::LogErr("FileIo::ReadText: Error opening file '{}'", filePath.string());
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
            Debug::LogErr("FileIo::WriteText: Error creating file '{}'", filePath.string());
            return false;
        }

        file << text;
        return true;
    }

    std::optional<std::vector<u8>> ReadBytes(const std::filesystem::path& filePath) {
        std::ifstream file(filePath, std::ios::binary | std::ios::ate);

        if (!file.is_open()) {
            Debug::LogErr("FileIo::ReadBytes: Error opening file '{}'", filePath.string());
            return std::nullopt;
        }

        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);

        std::vector<u8> buffer(size);
        if (!file.read(reinterpret_cast<char*>(buffer.data()), size)) {
            Debug::LogErr("FileIo::ReadBytes: Error reading file '{}'", filePath.string());
            return std::nullopt;
        }

        return buffer;
    }
}
