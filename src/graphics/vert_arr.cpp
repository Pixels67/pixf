#include "vert_arr.h"

#include <glad/glad.h>

#include "vert_buf.h"

namespace engine::graphics {
VertBufLayout::VertBufLayout(const unsigned int capacity) {
  elements_.reserve(capacity);
}

unsigned int VertBufLayout::GetSize() const {
  return static_cast<unsigned int>(elements_.size());
}

unsigned int VertBufLayout::GetStride() const { return stride_; }

const VertBufElement& VertBufLayout::operator[](const unsigned int i) const {
  return elements_[i];
}

VertArr::VertArr(const VertBuf& buffer, const VertBufLayout& layout) {
  glGenVertexArrays(1, &id_);
  glBindVertexArray(id_);

  buffer.Bind();
  unsigned int offset = 0;
  for (int i = 0; i < layout.GetSize(); i++) {
    glVertexAttribPointer(i, static_cast<GLint>(layout[i].size), layout[i].type,
                          static_cast<GLboolean>(layout[i].normalized),
                          static_cast<GLsizei>(layout.GetStride()),
                          reinterpret_cast<void*>(offset));

    glEnableVertexAttribArray(i);

    offset += layout[i].count * layout[i].size;
  }

  glBindVertexArray(0);
  VertBuf::Unbind();
}

VertArr::VertArr(VertArr&& other) noexcept : id_(other.id_) { other.id_ = 0; }

VertArr& VertArr::operator=(VertArr&& other) noexcept {
  if (this == &other) {
    return *this;
  }

  id_ = other.id_;
  other.id_ = 0;

  return *this;
}

VertArr::~VertArr() { glDeleteVertexArrays(1, &id_); }

void VertArr::Bind() const { glBindVertexArray(id_); }

void VertArr::Unbind() { glBindVertexArray(0); }
}  // namespace engine::graphics
