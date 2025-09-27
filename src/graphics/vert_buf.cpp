#include "vert_buf.h"

#include <glad/glad.h>

namespace engine::graphics {
VertBuf::VertBuf(const std::vector<float>& vertices,
                 const std::vector<unsigned int>& indices,
                 const unsigned int usage)
    : index_count_(static_cast<unsigned int>(indices.size())) {
  unsigned int vbo = 0;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER,
               static_cast<unsigned int>(vertices.size() * sizeof(float)),
               vertices.data(), usage);

  vbo_ = vbo;
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  unsigned int ebo = 0;
  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               static_cast<unsigned int>(indices.size() * sizeof(int)),
               indices.data(), usage);

  ebo_ = ebo;
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

VertBuf::VertBuf(VertBuf&& other) noexcept
    : vbo_(other.vbo_), ebo_(other.ebo_), index_count_(other.index_count_) {
  other.vbo_ = 0;
  other.ebo_ = 0;
  other.index_count_ = 0;
}

VertBuf& VertBuf::operator=(VertBuf&& other) noexcept {
  if (this == &other) {
    return *this;
  }

  vbo_ = other.vbo_;
  ebo_ = other.ebo_;
  index_count_ = other.index_count_;
  other.vbo_ = 0;
  other.ebo_ = 0;
  other.index_count_ = 0;

  return *this;
}

VertBuf::~VertBuf() {
  glDeleteBuffers(1, &vbo_);
  glDeleteBuffers(1, &ebo_);
}

void VertBuf::Bind() const {
  glBindBuffer(GL_ARRAY_BUFFER, vbo_);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
}

void VertBuf::Unbind() {
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

unsigned int VertBuf::GetIndexCount() const { return index_count_; }
}  // namespace engine::graphics
