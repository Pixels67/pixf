#pragma once

#include <vector>

namespace engine::graphics {
class VertBuf {
 public:
  VertBuf() = default;

  VertBuf(const std::vector<float>& vertices,
          const std::vector<unsigned int>& indices, unsigned int usage);

  VertBuf(const VertBuf& other) = delete;

  VertBuf& operator=(const VertBuf& other) = delete;

  VertBuf(VertBuf&& other) noexcept;

  VertBuf& operator=(VertBuf&& other) noexcept;

  ~VertBuf();

  void Bind() const;

  static void Unbind();

  [[nodiscard]] unsigned int GetIndexCount() const;

 private:
  unsigned int vbo_ = 0;
  unsigned int ebo_ = 0;

  unsigned int index_count_ = 0;
};
}  // namespace engine::graphics
