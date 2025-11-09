#include "File.hpp"

#include <fstream>
#include <string>

#include "Error/Result.hpp"

namespace Pixf::Core::File {
    FileResult ReadFile(const std::string &path) {
        std::ifstream file(path);

        if (!file.is_open()) {
            return FileError::FailedToOpen;
        }

        std::string data;
        file >> data;
        file.close();

        return data;
    }

    FileError WriteFile(const std::string &path, const std::string &data) {
        std::ofstream file(path);

        if (!file.is_open()) {
            return FileError::FailedToOpen;
        }

        file.clear();
        file << data;
        file.close();

        return FileError::None;
    }
} // namespace Pixf::Core::File
