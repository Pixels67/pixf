#pragma once

#include <vector>

#include "camera.h"
#include "core/transform.h"
#include "material.h"
#include "vert_arr.h"

struct Vertex {
  glm::vec3 position;
  glm::vec2 tex_coords;
};

namespace pixf::graphics {
const std::vector CUBE_VERTS{
    Vertex{{-0.5F, -0.5F, -0.5F}, {0.0F, 0.0F}}, Vertex{{0.5F, -0.5F, -0.5F}, {1.0F, 0.0F}},
    Vertex{{0.5F, 0.5F, -0.5F}, {1.0F, 1.0F}},   Vertex{{0.5F, 0.5F, -0.5F}, {1.0F, 1.0F}},
    Vertex{{-0.5F, 0.5F, -0.5F}, {0.0F, 1.0F}},  Vertex{{-0.5F, -0.5F, -0.5F}, {0.0F, 0.0F}},

    Vertex{{-0.5F, -0.5F, 0.5F}, {0.0F, 0.0F}},  Vertex{{0.5F, -0.5F, 0.5F}, {1.0F, 0.0F}},
    Vertex{{0.5F, 0.5F, 0.5F}, {1.0F, 1.0F}},    Vertex{{0.5F, 0.5F, 0.5F}, {1.0F, 1.0F}},
    Vertex{{-0.5F, 0.5F, 0.5F}, {0.0F, 1.0F}},   Vertex{{-0.5F, -0.5F, 0.5F}, {0.0F, 0.0F}},

    Vertex{{-0.5F, 0.5F, 0.5F}, {1.0F, 0.0F}},   Vertex{{-0.5F, 0.5F, -0.5F}, {1.0F, 1.0F}},
    Vertex{{-0.5F, -0.5F, -0.5F}, {0.0F, 1.0F}}, Vertex{{-0.5F, -0.5F, -0.5F}, {0.0F, 1.0F}},
    Vertex{{-0.5F, -0.5F, 0.5F}, {0.0F, 0.0F}},  Vertex{{-0.5F, 0.5F, 0.5F}, {1.0F, 0.0F}},

    Vertex{{0.5F, 0.5F, 0.5F}, {1.0F, 0.0F}},    Vertex{{0.5F, 0.5F, -0.5F}, {1.0F, 1.0F}},
    Vertex{{0.5F, -0.5F, -0.5F}, {0.0F, 1.0F}},  Vertex{{0.5F, -0.5F, -0.5F}, {0.0F, 1.0F}},
    Vertex{{0.5F, -0.5F, 0.5F}, {0.0F, 0.0F}},   Vertex{{0.5F, 0.5F, 0.5F}, {1.0F, 0.0F}},

    Vertex{{-0.5F, -0.5F, -0.5F}, {0.0F, 1.0F}}, Vertex{{0.5F, -0.5F, -0.5F}, {1.0F, 1.0F}},
    Vertex{{0.5F, -0.5F, 0.5F}, {1.0F, 0.0F}},   Vertex{{0.5F, -0.5F, 0.5F}, {1.0F, 0.0F}},
    Vertex{{-0.5F, -0.5F, 0.5F}, {0.0F, 0.0F}},  Vertex{{-0.5F, -0.5F, -0.5F}, {0.0F, 1.0F}},

    Vertex{{-0.5F, 0.5F, -0.5F}, {0.0F, 1.0F}},  Vertex{{0.5F, 0.5F, -0.5F}, {1.0F, 1.0F}},
    Vertex{{0.5F, 0.5F, 0.5F}, {1.0F, 0.0F}},    Vertex{{0.5F, 0.5F, 0.5F}, {1.0F, 0.0F}},
    Vertex{{-0.5F, 0.5F, 0.5F}, {0.0F, 0.0F}},   Vertex{{-0.5F, 0.5F, -0.5F}, {0.0F, 1.0F}},
};

const std::vector QUAD_VERTS{
    Vertex{{-0.5F, 0.0F, -0.5F}, {0.0F, 0.0F}}, Vertex{{0.5F, 0.0F, -0.5F}, {1.0F, 0.0F}},
    Vertex{{0.5F, 0.0F, 0.5F}, {1.0F, 1.0F}},   Vertex{{0.5F, 0.0F, 0.5F}, {1.0F, 1.0F}},
    Vertex{{-0.5F, 0.0F, 0.5F}, {0.0F, 1.0F}},  Vertex{{-0.5F, 0.0F, -0.5F}, {0.0F, 0.0F}},
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
  VertArr vert_arr_;
  VertBuf vert_buf_;
  std::vector<Vertex> vertices_;
  std::vector<unsigned int> indices_;

  unsigned int index_count_ = 0;
};
}  // namespace pixf::graphics
