#include "file_io.h"

#include <fstream>

namespace pixf::file {
std::string ReadFile(const std::string& path) {
  std::ifstream file(path);
  if (!file.is_open()) {
    return "";
  }

  file.seekg(0, std::ios::end);
  const auto size = file.tellg();
  file.seekg(0, std::ios::beg);

  std::string content(size, '\0');
  file.read(content.data(), size);

  return content;
}
}  // namespace pixf::file
