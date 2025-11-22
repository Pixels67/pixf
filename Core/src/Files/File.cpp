#include "File.hpp"

namespace Pixf::Core::Files {
    std::string ReadFile(const std::string &path) {
        std::ifstream file(path);

        if (!file.is_open()) {
            throw FileError("Failed to open file.");
        }

        std::string data;
        std::string buffer;
        while (std::getline(file, buffer)) {
            data += buffer;
        }

        file.close();

        return data;
    }

    void WriteFile(const std::string &path, const std::string &data) {
        std::ofstream file(path);

        if (!file.is_open()) {
            throw FileError("Failed to open file.");
        }

        file.clear();
        file << data;
        file.close();
    }

    void DeleteFile(const std::string &path) { std::filesystem::remove(path); }
    bool FileExists(const std::string &path) { return std::filesystem::exists(path); }

    void MakeDirectory(const std::string &path) {
        const std::filesystem::path dir = path;

        try {
            if (std::filesystem::exists(dir)) {
                return;
            }

            if (!std::filesystem::create_directory(dir)) {
                throw FileError("Failed to create directory.");
            }
        } catch (const std::filesystem::filesystem_error &e) {
            throw FileError("Failed to create directory: " + std::string(e.what()));
        }
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

    std::vector<std::string>
    GetFilesInDirectory(const std::string &directory, const std::string &extension, const bool recursive) {
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
} // namespace Pixf::Core::Files
