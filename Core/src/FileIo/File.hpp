#ifndef FLK_FILE_HPP
#define FLK_FILE_HPP

#include "Common.hpp"

namespace Flock::FileIo {
    std::optional<std::string> FLK_API ReadText(const std::filesystem::path &filePath);
    bool FLK_API                       WriteText(const std::filesystem::path &filePath, const std::string &text);
}

#endif //FLK_FILE_HPP
