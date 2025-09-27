#pragma once

#include <vector>

#include "glad/glad.h"
#include "vert_buf.h"

namespace engine::graphics {
struct VertBufElement {
  unsigned int type;
  unsigned int count;
  unsigned int size;
  bool normalized;
};

class VertBufLayout {
 public:
  VertBufLayout() = default;

  explicit VertBufLayout(unsigned int capacity);

  VertBufLayout(const VertBufLayout& other) = delete;

  VertBufLayout& operator=(const VertBufLayout& other) = delete;

  VertBufLayout(VertBufLayout&& other) = delete;

  VertBufLayout& operator=(VertBufLayout&& other) = delete;

  ~VertBufLayout() = default;

  [[nodiscard]] unsigned int GetSize() const;

  [[nodiscard]] unsigned int GetStride() const;

  template <typename T>
  void PushBack(unsigned int count);

  template <>
  void PushBack<float>(const unsigned int count) {
    elements_.push_back({GL_FLOAT, count, sizeof(float), false});
    stride_ += count * sizeof(float);
  }

  template <>
  void PushBack<int>(const unsigned int count) {
    elements_.push_back({GL_INT, count, sizeof(int), false});
    stride_ += count * sizeof(int);
  }

  template <>
  void PushBack<unsigned int>(const unsigned int count) {
    elements_.push_back({GL_UNSIGNED_INT, sizeof(int), count, false});
    stride_ += count * sizeof(int);
  }

  template <>
  void PushBack<char>(const unsigned int count) {
    elements_.push_back({GL_BYTE, count, sizeof(char), true});
    stride_ += count * sizeof(char);
  }

  template <>
  void PushBack<unsigned char>(const unsigned int count) {
    elements_.push_back({GL_UNSIGNED_BYTE, sizeof(char), count, true});
    stride_ += count * sizeof(char);
  }

  const VertBufElement& operator[](unsigned int i) const;

 private:
  std::vector<VertBufElement> elements_;
  unsigned int stride_ = 0;
};

class VertArr {
 public:
  VertArr() = default;

  explicit VertArr(const VertBuf& buffer, const VertBufLayout& layout);

  VertArr(const VertArr& other) = delete;

  VertArr& operator=(const VertArr& other) = delete;

  VertArr(VertArr&& other) noexcept;

  VertArr& operator=(VertArr&& other) noexcept;

  ~VertArr();

  void Bind() const;

  static void Unbind();

 private:
  unsigned int id_ = 0;
};
}  // namespace engine::graphics
