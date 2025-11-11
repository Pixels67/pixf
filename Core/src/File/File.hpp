#ifndef PIXF_FILE_HPP
#define PIXF_FILE_HPP

#include <string>
#include <vector>

#include "Error/Result.hpp"

namespace Pixf::Core::File {
    enum class FileError : uint8_t {
        None = 0,
        FailedToOpen,
    };

    using FileResult = Error::Result<std::string, FileError>;

    PIXF_API FileResult ReadFile(const std::string &path);
    PIXF_API FileError WriteFile(const std::string &path, const std::string &data);
    PIXF_API void DeleteFile(const std::string &path);

    PIXF_API std::vector<std::string> GetFilesInDirectory(const std::string &directory, bool recursive = false);
    PIXF_API std::vector<std::string> GetFilesInDirectory(const std::string &directory, const std::string &extension,
                                                          bool recursive = false);
} // namespace Pixf::Core::File

#endif // PIXF_FILE_HPP
