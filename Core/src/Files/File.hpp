#ifndef PIXF_FILE_HPP
#define PIXF_FILE_HPP

#include "Common.hpp"
#include "Error/Error.hpp"

namespace Pixf::Core::Files {
    class FileError final : public Error::Error {
    public:
        using Error::Error;
    };

    PIXF_API std::string ReadFile(const std::string &path);
    PIXF_API void WriteFile(const std::string &path, const std::string &data);
    PIXF_API void DeleteFile(const std::string &path);
    PIXF_API bool FileExists(const std::string &path);

    PIXF_API void MakeDirectory(const std::string &path);
    PIXF_API std::vector<std::string> GetFilesInDirectory(const std::string &directory, bool recursive = false);
    PIXF_API std::vector<std::string>
    GetFilesInDirectory(const std::string &directory, const std::string &extension, bool recursive = false);
} // namespace Pixf::Core::Files

#endif // PIXF_FILE_HPP
