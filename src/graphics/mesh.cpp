#include "mesh.h"

#include <glad/glad.h>

#include "camera.h"
#include "gl/shader.h"
#include "gtc/type_ptr.inl"
#include "lighting/point_light.h"
#include "material.h"
#include "resource_manager.h"

namespace pixf::graphics {
gl::VertBufLayout GetLayout() {
  gl::VertBufLayout layout(2);
  layout.PushBack<float>(3);
  layout.PushBack<float>(2);
  layout.PushBack<float>(3);

  return layout;
}

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
    : vertices_(vertices), indices_(indices), index_count_(indices.size()) {
  std::vector<float> vertex_data{};
  for (auto [position, texCoords, normal] : vertices) {
    vertex_data.push_back(position.x);
    vertex_data.push_back(position.y);
    vertex_data.push_back(position.z);
    vertex_data.push_back(texCoords.s);
    vertex_data.push_back(texCoords.t);
    vertex_data.push_back(normal.x);
    vertex_data.push_back(normal.y);
    vertex_data.push_back(normal.z);
  }

  vert_buf_ = gl::VertBuf(vertex_data, indices, GL_STATIC_DRAW);
  vert_arr_ = gl::VertArr(vert_buf_, GetLayout());

  sub_meshes_.push_back({});
}

Mesh::Mesh(const std::vector<Vertex>& vertices) {
  std::vector<float> vertex_data{};
  for (auto [position, texCoords, normal] : vertices) {
    vertex_data.push_back(position.x);
    vertex_data.push_back(position.y);
    vertex_data.push_back(position.z);
    vertex_data.push_back(texCoords.s);
    vertex_data.push_back(texCoords.t);
    vertex_data.push_back(normal.x);
    vertex_data.push_back(normal.y);
    vertex_data.push_back(normal.z);
  }

  std::vector<unsigned int> indices{};
  indices.reserve(vertices.size());
  for (int i = 0; i < vertices.size(); i++) {
    indices.push_back(i);
  }

  index_count_ = indices.size();
  vert_buf_ = gl::VertBuf(vertex_data, indices, GL_STATIC_DRAW);
  vert_arr_ = gl::VertArr(vert_buf_, GetLayout());
  vertices_ = vertices;
  indices_ = indices;

  sub_meshes_.push_back({});
}

Mesh::Mesh(const Mesh& other) {
  if (this == &other) {
    return;
  }

  Mesh temp(other.vertices_, other.indices_);
  *this = std::move(temp);
}

Mesh& Mesh::operator=(const Mesh& other) {
  if (this == &other) {
    return *this;
  }

  Mesh temp(other.vertices_, other.indices_);
  *this = std::move(temp);
  return *this;
}

Mesh::Mesh(Mesh&& other) noexcept {
  if (this == &other) {
    return;
  }

  this->vert_arr_ = std::move(other.vert_arr_);
  this->vert_buf_ = std::move(other.vert_buf_);
  this->index_count_ = other.index_count_;
  this->vertices_ = std::move(other.vertices_);
  this->indices_ = std::move(other.indices_);
  this->sub_meshes_ = std::move(other.sub_meshes_);
}

Mesh& Mesh::operator=(Mesh&& other) noexcept {
  if (this == &other) {
    return *this;
  }

  this->vert_arr_ = std::move(other.vert_arr_);
  this->vert_buf_ = std::move(other.vert_buf_);
  this->index_count_ = other.index_count_;
  this->vertices_ = std::move(other.vertices_);
  this->indices_ = std::move(other.indices_);
  this->sub_meshes_ = std::move(other.sub_meshes_);

  return *this;
}

void Mesh::Render(const MeshRenderConfig& mesh_render_config) const {
  if (mesh_render_config.materials.empty() ||
      mesh_render_config.materials.size() != sub_meshes_.size()) {
    std::cerr << "Invalid number of materials provided!\n";
    return;
  }

  if (index_count_ <= 0) {
    return;
  }

  if (!vert_buf_.IsValid()) {
    return;
  }

  vert_arr_.Bind();
  for (int i = 0; i < sub_meshes_.size(); i++) {
    BindMaterial(mesh_render_config.materials[i], mesh_render_config.resource_manager,
                 mesh_render_config.proj, mesh_render_config.ambient_light,
                 mesh_render_config.point_lights, mesh_render_config.camera,
                 mesh_render_config.transform);
    if (sub_meshes_[i].count == 0) {
      glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(index_count_), GL_UNSIGNED_INT,
                     reinterpret_cast<void*>(sub_meshes_[i].start_index));
      break;
    }

    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(sub_meshes_[i].count), GL_UNSIGNED_INT,
                   reinterpret_cast<void*>(sub_meshes_[i].start_index));
  }

  Material::Unbind();
}

void Mesh::BindMaterial(const Material& material, const ResourceManager& resource_manager,
                        const glm::mat4& proj, const glm::vec3 ambient_light,
                        const std::vector<lighting::PointLight>& point_lights,
                        const CameraTransform& camera, const core::Transform& transform) {
  resource_manager.GetShader(material.shader)->SetUniform("transforms.proj", proj);
  resource_manager.GetShader(material.shader)
      ->SetUniform("transforms.view", camera.GetViewMatrix());
  resource_manager.GetShader(material.shader)
      ->SetUniform("transforms.model", transform.GetMatrix());

  resource_manager.GetShader(material.shader)
      ->SetUniform("point_light_count", {static_cast<int>(point_lights.size())});

  for (int i = 0; i < point_lights.size(); i++) {
    std::string element = "point_light[" + std::to_string(i) + "]";
    resource_manager.GetShader(material.shader)
        ->SetUniform(element + ".light_color", point_lights[i].color * point_lights[i].intensity);
    resource_manager.GetShader(material.shader)
        ->SetUniform(element + ".light_pos", point_lights[i].position);
    resource_manager.GetShader(material.shader)
        ->SetUniform(element + ".k_linear", {point_lights[i].linear_falloff});
    resource_manager.GetShader(material.shader)
        ->SetUniform(element + ".k_quadratic", {point_lights[i].quadratic_falloff});
  }

  resource_manager.GetShader(material.shader)->SetUniform("ambient_light", ambient_light);

  material.Bind(resource_manager);
}
}  // namespace pixf::graphics
