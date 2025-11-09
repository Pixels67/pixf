#ifndef PIXF_FILE_HPP
#define PIXF_FILE_HPP

#include <string>

#include "Error/Result.hpp"

namespace Pixf::Core::File {
    enum class FileError : uint8_t {
        None = 0,
        FailedToOpen,
    };

    using FileResult = Error::Result<std::string, FileError>;

    FileResult ReadFile(const std::string &path);
    FileError WriteFile(const std::string &path, const std::string &data);
} // namespace Pixf::Core::File

#endif // PIXF_FILE_HPP
