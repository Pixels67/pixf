#pragma once

#include <vector>

#include "core/transform.h"
#include "gl/vert_arr.h"

namespace pixf::graphics {
namespace lighting {
struct PointLight;
}

struct CameraTransform;
struct Material;
struct ResourceManager;

struct Vertex {
  glm::vec3 position;
  glm::vec2 tex_coords;
  glm::vec3 normal;
};

struct MeshRenderConfig {
  const std::vector<Material>& materials;
  const ResourceManager& resource_manager;
  const CameraTransform& camera;
  const std::vector<lighting::PointLight>& point_lights;
  glm::mat4 proj;
  core::Transform transform = {};
  glm::vec3 ambient_light;
};

const std::vector CUBE_VERTS{
    Vertex{{-0.5F, -0.5F, -0.5F}, {0.0F, 0.0F}, {0.0F, 0.0F, -1.0F}},
    Vertex{{0.5F, -0.5F, -0.5F}, {1.0F, 0.0F}, {0.0F, 0.0F, -1.0F}},
    Vertex{{0.5F, 0.5F, -0.5F}, {1.0F, 1.0F}, {0.0F, 0.0F, -1.0F}},
    Vertex{{0.5F, 0.5F, -0.5F}, {1.0F, 1.0F}, {0.0F, 0.0F, -1.0F}},
    Vertex{{-0.5F, 0.5F, -0.5F}, {0.0F, 1.0F}, {0.0F, 0.0F, -1.0F}},
    Vertex{{-0.5F, -0.5F, -0.5F}, {0.0F, 0.0F}, {0.0F, 0.0F, -1.0F}},

    Vertex{{-0.5F, -0.5F, 0.5F}, {0.0F, 0.0F}, {0.0F, 0.0F, 1.0F}},
    Vertex{{0.5F, -0.5F, 0.5F}, {1.0F, 0.0F}, {0.0F, 0.0F, 1.0F}},
    Vertex{{0.5F, 0.5F, 0.5F}, {1.0F, 1.0F}, {0.0F, 0.0F, 1.0F}},
    Vertex{{0.5F, 0.5F, 0.5F}, {1.0F, 1.0F}, {0.0F, 0.0F, 1.0F}},
    Vertex{{-0.5F, 0.5F, 0.5F}, {0.0F, 1.0F}, {0.0F, 0.0F, 1.0F}},
    Vertex{{-0.5F, -0.5F, 0.5F}, {0.0F, 0.0F}, {0.0F, 0.0F, 1.0F}},

    Vertex{{-0.5F, 0.5F, 0.5F}, {1.0F, 0.0F}, {-1.0F, 0.0F, 0.0F}},
    Vertex{{-0.5F, 0.5F, -0.5F}, {1.0F, 1.0F}, {-1.0F, 0.0F, 0.0F}},
    Vertex{{-0.5F, -0.5F, -0.5F}, {0.0F, 1.0F}, {-1.0F, 0.0F, 0.0F}},
    Vertex{{-0.5F, -0.5F, -0.5F}, {0.0F, 1.0F}, {-1.0F, 0.0F, 0.0F}},
    Vertex{{-0.5F, -0.5F, 0.5F}, {0.0F, 0.0F}, {-1.0F, 0.0F, 0.0F}},
    Vertex{{-0.5F, 0.5F, 0.5F}, {1.0F, 0.0F}, {-1.0F, 0.0F, 0.0F}},

    Vertex{{0.5F, 0.5F, 0.5F}, {1.0F, 0.0F}, {1.0F, 0.0F, 0.0F}},
    Vertex{{0.5F, 0.5F, -0.5F}, {1.0F, 1.0F}, {1.0F, 0.0F, 0.0F}},
    Vertex{{0.5F, -0.5F, -0.5F}, {0.0F, 1.0F}, {1.0F, 0.0F, 0.0F}},
    Vertex{{0.5F, -0.5F, -0.5F}, {0.0F, 1.0F}, {1.0F, 0.0F, 0.0F}},
    Vertex{{0.5F, -0.5F, 0.5F}, {0.0F, 0.0F}, {1.0F, 0.0F, 0.0F}},
    Vertex{{0.5F, 0.5F, 0.5F}, {1.0F, 0.0F}, {1.0F, 0.0F, 0.0F}},

    Vertex{{-0.5F, -0.5F, -0.5F}, {0.0F, 1.0F}, {0.0F, -1.0F, 0.0F}},
    Vertex{{0.5F, -0.5F, -0.5F}, {1.0F, 1.0F}, {0.0F, -1.0F, 0.0F}},
    Vertex{{0.5F, -0.5F, 0.5F}, {1.0F, 0.0F}, {0.0F, -1.0F, 0.0F}},
    Vertex{{0.5F, -0.5F, 0.5F}, {1.0F, 0.0F}, {0.0F, -1.0F, 0.0F}},
    Vertex{{-0.5F, -0.5F, 0.5F}, {0.0F, 0.0F}, {0.0F, -1.0F, 0.0F}},
    Vertex{{-0.5F, -0.5F, -0.5F}, {0.0F, 1.0F}, {0.0F, -1.0F, 0.0F}},

    Vertex{{-0.5F, 0.5F, -0.5F}, {0.0F, 1.0F}, {0.0F, 1.0F, 0.0F}},
    Vertex{{0.5F, 0.5F, -0.5F}, {1.0F, 1.0F}, {0.0F, 1.0F, 0.0F}},
    Vertex{{0.5F, 0.5F, 0.5F}, {1.0F, 0.0F}, {0.0F, 1.0F, 0.0F}},
    Vertex{{0.5F, 0.5F, 0.5F}, {1.0F, 0.0F}, {0.0F, 1.0F, 0.0F}},
    Vertex{{-0.5F, 0.5F, 0.5F}, {0.0F, 0.0F}, {0.0F, 1.0F, 0.0F}},
    Vertex{{-0.5F, 0.5F, -0.5F}, {0.0F, 1.0F}, {0.0F, 1.0F, 0.0F}},
};

const std::vector QUAD_VERTS{
    Vertex{{-0.5F, 0.0F, -0.5F}, {0.0F, 0.0F}, {0.0F, -1.0F, 0.0F}},
    Vertex{{0.5F, 0.0F, -0.5F}, {1.0F, 0.0F}, {0.0F, -1.0F, 0.0F}},
    Vertex{{0.5F, 0.0F, 0.5F}, {1.0F, 1.0F}, {0.0F, -1.0F, 0.0F}},
    Vertex{{0.5F, 0.0F, 0.5F}, {1.0F, 1.0F}, {0.0F, -1.0F, 0.0F}},
    Vertex{{-0.5F, 0.0F, 0.5F}, {0.0F, 1.0F}, {0.0F, -1.0F, 0.0F}},
    Vertex{{-0.5F, 0.0F, -0.5F}, {0.0F, 0.0F}, {0.0F, -1.0F, 0.0F}},
};

struct SubMesh {
  unsigned int start_index = 0;
  unsigned int count = 0;
};

class Mesh {
 public:
  Mesh() = default;
  Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
  explicit Mesh(const std::vector<Vertex>& vertices);

  Mesh(const Mesh& other);
  Mesh& operator=(const Mesh& other);

  Mesh(Mesh&& other) noexcept;
  Mesh& operator=(Mesh&& other) noexcept;

  ~Mesh() = default;

  void Render(const MeshRenderConfig& mesh_render_config) const;

 private:
  gl::VertArr vert_arr_;
  gl::VertBuf vert_buf_;
  std::vector<Vertex> vertices_{};
  std::vector<unsigned int> indices_{};
  std::vector<SubMesh> sub_meshes_{};
  unsigned int index_count_ = 0;

  static void BindMaterial(const Material& material, const ResourceManager& resource_manager,
                           const glm::mat4& proj, glm::vec3 ambient_light,
                           const std::vector<lighting::PointLight>& point_lights,
                           const CameraTransform& camera, const core::Transform& transform = {});
};
}  // namespace pixf::graphics
