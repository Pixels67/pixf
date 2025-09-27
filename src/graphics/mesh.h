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

namespace engine::graphics {
// @formatter:off
const std::vector CUBE_VERTS{
    Vertex{{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}},
    Vertex{{0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}},
    Vertex{{0.5f, 0.5f, -0.5f}, {1.0f, 1.0f}},
    Vertex{{0.5f, 0.5f, -0.5f}, {1.0f, 1.0f}},
    Vertex{{-0.5f, 0.5f, -0.5f}, {0.0f, 1.0f}},
    Vertex{{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}},

    Vertex{{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f}},
    Vertex{{0.5f, -0.5f, 0.5f}, {1.0f, 0.0f}},
    Vertex{{0.5f, 0.5f, 0.5f}, {1.0f, 1.0f}},
    Vertex{{0.5f, 0.5f, 0.5f}, {1.0f, 1.0f}},
    Vertex{{-0.5f, 0.5f, 0.5f}, {0.0f, 1.0f}},
    Vertex{{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f}},

    Vertex{{-0.5f, 0.5f, 0.5f}, {1.0f, 0.0f}},
    Vertex{{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f}},
    Vertex{{-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}},
    Vertex{{-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}},
    Vertex{{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f}},
    Vertex{{-0.5f, 0.5f, 0.5f}, {1.0f, 0.0f}},

    Vertex{{0.5f, 0.5f, 0.5f}, {1.0f, 0.0f}},
    Vertex{{0.5f, 0.5f, -0.5f}, {1.0f, 1.0f}},
    Vertex{{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}},
    Vertex{{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}},
    Vertex{{0.5f, -0.5f, 0.5f}, {0.0f, 0.0f}},
    Vertex{{0.5f, 0.5f, 0.5f}, {1.0f, 0.0f}},

    Vertex{{-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}},
    Vertex{{0.5f, -0.5f, -0.5f}, {1.0f, 1.0f}},
    Vertex{{0.5f, -0.5f, 0.5f}, {1.0f, 0.0f}},
    Vertex{{0.5f, -0.5f, 0.5f}, {1.0f, 0.0f}},
    Vertex{{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f}},
    Vertex{{-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}},

    Vertex{{-0.5f, 0.5f, -0.5f}, {0.0f, 1.0f}},
    Vertex{{0.5f, 0.5f, -0.5f}, {1.0f, 1.0f}},
    Vertex{{0.5f, 0.5f, 0.5f}, {1.0f, 0.0f}},
    Vertex{{0.5f, 0.5f, 0.5f}, {1.0f, 0.0f}},
    Vertex{{-0.5f, 0.5f, 0.5f}, {0.0f, 0.0f}},
    Vertex{{-0.5f, 0.5f, -0.5f}, {0.0f, 1.0f}},
};

const std::vector kQuadVerts{
    Vertex{{-0.5f, 0.0f, -0.5f}, {0.0f, 0.0f}},
    Vertex{{0.5f, 0.0f, -0.5f}, {1.0f, 0.0f}},
    Vertex{{0.5f, 0.0f, 0.5f}, {1.0f, 1.0f}},
    Vertex{{0.5f, 0.0f, 0.5f}, {1.0f, 1.0f}},
    Vertex{{-0.5f, 0.0f, 0.5f}, {0.0f, 1.0f}},
    Vertex{{-0.5f, 0.0f, -0.5f}, {0.0f, 0.0f}},
};

// @formatter:on

class Mesh {
 public:
  Mesh(const std::vector<Vertex>& vertices,
       const std::vector<unsigned int>& indices);

  explicit Mesh(const std::vector<Vertex>& vertices);

  Mesh(Mesh& other) = delete;

  Mesh& operator=(Mesh& other) = delete;

  Mesh(Mesh&& other) = delete;

  Mesh& operator=(Mesh&& other) = delete;

  void Render(const Material& material, const Camera& camera,
              const core::Transform& transform = {}) const;

 private:
  VertArr vert_arr_;
  VertBuf vert_buf_;
};
}  // namespace engine::graphics
