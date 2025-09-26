#pragma once

#include <vector>

#include "camera.h"
#include "material.h"
#include "shader.h"
#include "texture.h"
#include "vert_arr.h"
#include "core/transform.h"

struct Vertex {
    glm::vec3 position;
    glm::vec2 texCoords;
};

namespace Engine::Graphics {
    // @formatter:off
    const std::vector CUBE_VERTS{
        Vertex{{-0.5f, -0.5f, -0.5f},  {0.0f, 0.0f}},
        Vertex{{ 0.5f, -0.5f, -0.5f},  {1.0f, 0.0f}},
        Vertex{{ 0.5f,  0.5f, -0.5f},  {1.0f, 1.0f}},
        Vertex{{ 0.5f,  0.5f, -0.5f},  {1.0f, 1.0f}},
        Vertex{{-0.5f,  0.5f, -0.5f},  {0.0f, 1.0f}},
        Vertex{{-0.5f, -0.5f, -0.5f},  {0.0f, 0.0f}},

        Vertex{{-0.5f, -0.5f,  0.5f},  {0.0f, 0.0f}},
        Vertex{{ 0.5f, -0.5f,  0.5f},  {1.0f, 0.0f}},
        Vertex{{ 0.5f,  0.5f,  0.5f},  {1.0f, 1.0f}},
        Vertex{{ 0.5f,  0.5f,  0.5f},  {1.0f, 1.0f}},
        Vertex{{-0.5f,  0.5f,  0.5f},  {0.0f, 1.0f}},
        Vertex{{-0.5f, -0.5f,  0.5f},  {0.0f, 0.0f}},

        Vertex{{-0.5f,  0.5f,  0.5f},  {1.0f, 0.0f}},
        Vertex{{-0.5f,  0.5f, -0.5f},  {1.0f, 1.0f}},
        Vertex{{-0.5f, -0.5f, -0.5f},  {0.0f, 1.0f}},
        Vertex{{-0.5f, -0.5f, -0.5f},  {0.0f, 1.0f}},
        Vertex{{-0.5f, -0.5f,  0.5f},  {0.0f, 0.0f}},
        Vertex{{-0.5f,  0.5f,  0.5f},  {1.0f, 0.0f}},

        Vertex{{0.5f,  0.5f,  0.5f},   {1.0f, 0.0f}},
        Vertex{{0.5f,  0.5f, -0.5f},   {1.0f, 1.0f}},
        Vertex{{0.5f, -0.5f, -0.5f},   {0.0f, 1.0f}},
        Vertex{{0.5f, -0.5f, -0.5f},   {0.0f, 1.0f}},
        Vertex{{0.5f, -0.5f,  0.5f},   {0.0f, 0.0f}},
        Vertex{{0.5f,  0.5f,  0.5f},   {1.0f, 0.0f}},

        Vertex{{-0.5f, -0.5f, -0.5f},  {0.0f, 1.0f}},
        Vertex{{ 0.5f, -0.5f, -0.5f},  {1.0f, 1.0f}},
        Vertex{{ 0.5f, -0.5f,  0.5f},  {1.0f, 0.0f}},
        Vertex{{ 0.5f, -0.5f,  0.5f},  {1.0f, 0.0f}},
        Vertex{{-0.5f, -0.5f,  0.5f},  {0.0f, 0.0f}},
        Vertex{{-0.5f, -0.5f, -0.5f},  {0.0f, 1.0f}},

        Vertex{{-0.5f,  0.5f, -0.5f},  {0.0f, 1.0f}},
        Vertex{{ 0.5f,  0.5f, -0.5f},  {1.0f, 1.0f}},
        Vertex{{ 0.5f,  0.5f,  0.5f},  {1.0f, 0.0f}},
        Vertex{{ 0.5f,  0.5f,  0.5f},  {1.0f, 0.0f}},
        Vertex{{-0.5f,  0.5f,  0.5f},  {0.0f, 0.0f}},
        Vertex{{-0.5f,  0.5f, -0.5f},  {0.0f, 1.0f}},
    };

    const std::vector QUAD_VERTS{
        Vertex{{-0.5f, 0.0f, -0.5f}, {0.0f, 0.0f}},
        Vertex{{ 0.5f, 0.0f, -0.5f}, {1.0f, 0.0f}},
        Vertex{{ 0.5f, 0.0f,  0.5f}, {1.0f, 1.0f}},
        Vertex{{ 0.5f, 0.0f,  0.5f}, {1.0f, 1.0f}},
        Vertex{{-0.5f, 0.0f,  0.5f}, {0.0f, 1.0f}},
        Vertex{{-0.5f, 0.0f, -0.5f}, {0.0f, 0.0f}},
    };

    // @formatter:on

    class Mesh {
    public:
        Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices);

        explicit Mesh(const std::vector<Vertex> &vertices);


        Mesh(Mesh &other) = delete;

        Mesh &operator=(Mesh &other) = delete;

        Mesh(Mesh &&other) = delete;

        Mesh &operator=(Mesh &&other) = delete;

        void Render(const Material &material, const Camera &camera, const Core::Transform &transform = {}) const;

    private:
        VertArr m_VertArr;
        VertBuf m_VertBuf;
    };
} // namespace Engine::Graphics
