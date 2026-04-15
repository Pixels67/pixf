#ifndef FLK_FILE_HPP
#define FLK_FILE_HPP

#include <filesystem>
#include <optional>
#include <string>
#include <vector>

#include "Common.hpp"

namespace Flock::FileIo {
    std::optional<std::string> FLK_API ReadText(const std::filesystem::path &filePath);
    bool FLK_API                       WriteText(const std::filesystem::path &filePath, const std::string &text);

    std::optional<std::vector<u8>> FLK_API ReadBytes(const std::filesystem::path& filePath);
}

#endif //FLK_FILE_HPP
