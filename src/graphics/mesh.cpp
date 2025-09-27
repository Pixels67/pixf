#include "mesh.h"

#include <glad/glad.h>

#include "gtc/type_ptr.inl"

namespace engine::graphics {
Mesh::Mesh(const std::vector<Vertex>& vertices,
           const std::vector<unsigned int>& indices) {
  std::vector<float> vertex_data{};
  for (auto [position, texCoords] : vertices) {
    vertex_data.push_back(position.x);
    vertex_data.push_back(position.y);
    vertex_data.push_back(position.z);
    vertex_data.push_back(texCoords.s);
    vertex_data.push_back(texCoords.t);
  }

  vert_buf_ = VertBuf(vertex_data, indices, GL_STATIC_DRAW);

  VertBufLayout layout(2);
  layout.PushBack<float>(3);
  layout.PushBack<float>(2);

  vert_arr_ = VertArr(vert_buf_, layout);
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

  vert_buf_ = VertBuf(vertex_data, indices, GL_STATIC_DRAW);

  VertBufLayout layout(2);
  layout.PushBack<float>(3);
  layout.PushBack<float>(2);

  vert_arr_ = VertArr(vert_buf_, layout);
}

void Mesh::Render(const Material& material, const Camera& camera,
                  const core::Transform& transform) const {
  vert_arr_.Bind();

  constexpr int width = 800;
  constexpr int height = 600;
  const glm::mat4 proj = glm::perspective<float>(
      glm::radians(60.0F),
      static_cast<float>(width) / static_cast<float>(height), 0.1F, 100.0F);

  material.shader.SetUniform("proj", proj);
  material.shader.SetUniform("view", camera.GetViewMatrix());
  material.shader.SetUniform("model", transform.GetMatrix());
  material.Bind();

  glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(vert_buf_.GetIndexCount()),
                 GL_UNSIGNED_INT, nullptr);
  Material::Unbind();
}
}  // namespace engine::graphics
