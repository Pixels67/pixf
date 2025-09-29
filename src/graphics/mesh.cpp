#include "mesh.h"

#include <glad/glad.h>

#include "gtc/type_ptr.inl"

namespace pixf::graphics {
VertBufLayout GetLayout() {
  VertBufLayout layout(2);
  layout.PushBack<float>(3);
  layout.PushBack<float>(2);

  return layout;
}

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) : index_count_(indices.size()), vertices_(vertices), indices_(indices) {
  std::vector<float> vertex_data{};
  for (auto [position, texCoords] : vertices) {
    vertex_data.push_back(position.x);
    vertex_data.push_back(position.y);
    vertex_data.push_back(position.z);
    vertex_data.push_back(texCoords.s);
    vertex_data.push_back(texCoords.t);
  }

  
  vert_buf_ = VertBuf(vertex_data, indices, GL_STATIC_DRAW);
  vert_arr_ = VertArr(vert_buf_, GetLayout());
  
  
}

Mesh::Mesh(const std::vector<Vertex>& vertices) {
  std::vector<float> vertex_data{};
  for (auto [position, texCoords] : vertices) {
    vertex_data.push_back(position.x);
    vertex_data.push_back(position.y);
    vertex_data.push_back(position.z);
    vertex_data.push_back(texCoords.s);
    vertex_data.push_back(texCoords.t);
  }

  std::vector<unsigned int> indices{};
  indices.reserve(vertices.size());
  for (int i = 0; i < vertices.size(); i++) {
    indices.push_back(i);
  }

  index_count_ = indices.size();
  vert_buf_ = VertBuf(vertex_data, indices, GL_STATIC_DRAW);
  vert_arr_ = VertArr(vert_buf_, GetLayout());
  vertices_ = vertices;
  indices_ = indices;
}

Mesh::Mesh(const Mesh& other) {
  if (this == &other) { return;
}

  Mesh temp(other.vertices_, other.indices_);
  *this = std::move(temp);
}

Mesh& Mesh::operator=(const Mesh& other) {
  if (this == &other) { return *this;
}

  Mesh temp(other.vertices_, other.indices_);
  *this = std::move(temp);
  return *this;
}

Mesh::Mesh(Mesh&& other)  noexcept {
  if (this == &other) { return;
}

  this->vert_arr_ = std::move(other.vert_arr_);
  this->vert_buf_ = std::move(other.vert_buf_);
  this->index_count_ = other.index_count_;
  this->vertices_ = std::move(other.vertices_);
  this->indices_ = std::move(other.indices_);
}

Mesh& Mesh::operator=(Mesh&& other)  noexcept {
  if (this == &other) { return *this;
}

  this->vert_arr_ = std::move(other.vert_arr_);
  this->vert_buf_ = std::move(other.vert_buf_);
  this->index_count_ = other.index_count_;
  this->vertices_ = std::move(other.vertices_);
  this->indices_ = std::move(other.indices_);

  return *this;
}

void Mesh::Render(const Material& material, const CameraTransform& camera, const glm::mat4& proj,
                  const core::Transform& transform) const {
  if (!vert_buf_.IsValid()) { return;
}
  vert_arr_.Bind();

  material.GetShader().SetUniform("proj", proj);
  material.GetShader().SetUniform("view", camera.GetViewMatrix());
  material.GetShader().SetUniform("model", transform.GetMatrix());
  material.Bind();

  glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(index_count_), GL_UNSIGNED_INT, nullptr);
  Material::Unbind();
}
}  // namespace pixf::graphics
