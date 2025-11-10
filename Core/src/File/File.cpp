#include "File.hpp"

#include <filesystem>
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
        std::string buffer;
        while (std::getline(file, buffer)) {
            data += buffer;
        }

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

    std::vector<std::string> GetFilesInDirectory(const std::string &directory, const bool recursive) {
        std::vector<std::string> files;

        if (recursive) {
            for (const auto &entry: std::filesystem::recursive_directory_iterator(directory)) {
                if (entry.is_regular_file()) {
                    files.push_back(entry.path().string());
                }
            }
        } else {
            for (const auto &entry: std::filesystem::directory_iterator(directory)) {
                if (entry.is_regular_file()) {
                    files.push_back(entry.path().string());
                }
            }
        }

        return files;
    }

    std::vector<std::string> GetFilesInDirectory(const std::string &directory, const std::string &extension,
                                                 const bool recursive) {
        std::vector<std::string> files;

        if (recursive) {
            for (const auto &entry: std::filesystem::recursive_directory_iterator(directory)) {
                if (entry.is_regular_file() && entry.path().extension() == extension) {
                    files.push_back(entry.path().string());
                }
            }
        } else {
            for (const auto &entry: std::filesystem::directory_iterator(directory)) {
                if (entry.is_regular_file() && entry.path().extension() == extension) {
                    files.push_back(entry.path().string());
                }
            }
        }

        return files;
    }
} // namespace Pixf::Core::File
