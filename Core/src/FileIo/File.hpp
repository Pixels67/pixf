#ifndef FLK_FILE_HPP
#define FLK_FILE_HPP

#include "Common.hpp"

namespace Flock::FileIo {
    std::optional<std::string> FLK_API ReadTextFile(const std::filesystem::path &filePath);
}

#endif //FLK_FILE_HPP