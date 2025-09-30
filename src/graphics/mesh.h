#pragma once

#include <vector>

#include "camera.h"
#include "core/transform.h"
#include "gl/vert_arr.h"
#include "material.h"

namespace pixf::graphics {
struct Vertex {
  glm::vec3 position;
  glm::vec2 tex_coords;
  glm::vec3 normal;
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

class Mesh {
 public:
  Mesh() = default;

  Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);

  explicit Mesh(const std::vector<Vertex>& vertices);

  Mesh(const Mesh& other);

  Mesh& operator=(const Mesh& other);

  Mesh(Mesh&& other) noexcept;

  Mesh& operator=(Mesh&& other) noexcept;

  void Render(const Material& material, const ShaderManager& shader_manager,
              const CameraTransform& camera, const glm::mat4& proj,
              const core::Transform& transform = {}) const;

 private:
  gl::VertArr vert_arr_;
  gl::VertBuf vert_buf_;
  std::vector<Vertex> vertices_;
  std::vector<unsigned int> indices_;

  unsigned int index_count_ = 0;
};
}  // namespace pixf::graphics
