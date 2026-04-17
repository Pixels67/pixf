#include "Mesh.hpp"

#include <stdint.h>

#include "Graphics/Vertex.hpp"
#include "Graphics/VertexArray.hpp"
#include "Memory/Buffer.hpp"

namespace Flock::Graphics {
    std::optional<Mesh> Mesh::Create(const MeshData &data) {
        Mesh mesh{};

        mesh.m_Data = data;

        mesh.m_VertexArray  = VertexArray::Create();
        mesh.m_VertexBuffer = Buffer::Create(data.vertices, BufferType::Vertex);
        mesh.m_IndexBuffer  = Buffer::Create(data.indices, BufferType::Index);

        if (!mesh.m_VertexArray.SetVertexBuffer(mesh.m_VertexBuffer, Vertex::Layout())) {
            return std::nullopt;
        }

        if (!mesh.m_VertexArray.SetIndexBuffer(mesh.m_IndexBuffer)) {
            return std::nullopt;
        }

        mesh.m_IndexCount  = data.indices.size();
        mesh.m_Initialized = true;

        return mesh;
    }

    Mesh Mesh::Square(const Vector2f halfExtents) {
        const Vector2f h = halfExtents;

        const std::vector<Vertex> vertices = {
            {Vector3f{-h.x, -h.y, 0}, {0, 0, 1}, {0, 0}, {1, 0, 0}, {0, 1, 0}},
            {Vector3f{h.x, -h.y, 0}, {0, 0, 1}, {1, 0}, {1, 0, 0}, {0, 1, 0}},
            {Vector3f{h.x, h.y, 0}, {0, 0, 1}, {1, 1}, {1, 0, 0}, {0, 1, 0}},
            {Vector3f{-h.x, h.y, 0}, {0, 0, 1}, {0, 1}, {1, 0, 0}, {0, 1, 0}},
        };


        const std::vector<uint32_t> indices = {
            0, 1, 2, 2, 3, 0
        };

        return Create({.vertices = vertices, .indices = indices}).value();
    }

    Mesh Mesh::Box(const Vector3f halfExtents) {
        const Vector3f h = halfExtents;

        const std::vector<Vertex> vertices = {
            // Front face (normal: 0, 0, 1)
            {Vector3f{-h.x, -h.y, h.z}, {0, 0, 1}, {0, 0}, {1, 0, 0}, {0, 1, 0}},
            {Vector3f{h.x, -h.y, h.z}, {0, 0, 1}, {1, 0}, {1, 0, 0}, {0, 1, 0}},
            {Vector3f{h.x, h.y, h.z}, {0, 0, 1}, {1, 1}, {1, 0, 0}, {0, 1, 0}},
            {Vector3f{-h.x, h.y, h.z}, {0, 0, 1}, {0, 1}, {1, 0, 0}, {0, 1, 0}},

            // Back face (normal: 0, 0, -1)
            {Vector3f{h.x, -h.y, -h.z}, {0, 0, -1}, {0, 0}, {-1, 0, 0}, {0, 1, 0}},
            {Vector3f{-h.x, -h.y, -h.z}, {0, 0, -1}, {1, 0}, {-1, 0, 0}, {0, 1, 0}},
            {Vector3f{-h.x, h.y, -h.z}, {0, 0, -1}, {1, 1}, {-1, 0, 0}, {0, 1, 0}},
            {Vector3f{h.x, h.y, -h.z}, {0, 0, -1}, {0, 1}, {-1, 0, 0}, {0, 1, 0}},

            // Left face (normal: -1, 0, 0)
            {Vector3f{-h.x, -h.y, -h.z}, {-1, 0, 0}, {0, 0}, {0, 0, 1}, {0, 1, 0}},
            {Vector3f{-h.x, -h.y, h.z}, {-1, 0, 0}, {1, 0}, {0, 0, 1}, {0, 1, 0}},
            {Vector3f{-h.x, h.y, h.z}, {-1, 0, 0}, {1, 1}, {0, 0, 1}, {0, 1, 0}},
            {Vector3f{-h.x, h.y, -h.z}, {-1, 0, 0}, {0, 1}, {0, 0, 1}, {0, 1, 0}},

            // Right face (normal: 1, 0, 0)
            {Vector3f{h.x, -h.y, h.z}, {1, 0, 0}, {0, 0}, {0, 0, -1}, {0, 1, 0}},
            {Vector3f{h.x, -h.y, -h.z}, {1, 0, 0}, {1, 0}, {0, 0, -1}, {0, 1, 0}},
            {Vector3f{h.x, h.y, -h.z}, {1, 0, 0}, {1, 1}, {0, 0, -1}, {0, 1, 0}},
            {Vector3f{h.x, h.y, h.z}, {1, 0, 0}, {0, 1}, {0, 0, -1}, {0, 1, 0}},

            // Top face (normal: 0, 1, 0)
            {Vector3f{-h.x, h.y, h.z}, {0, 1, 0}, {0, 0}, {1, 0, 0}, {0, 0, -1}},
            {Vector3f{h.x, h.y, h.z}, {0, 1, 0}, {1, 0}, {1, 0, 0}, {0, 0, -1}},
            {Vector3f{h.x, h.y, -h.z}, {0, 1, 0}, {1, 1}, {1, 0, 0}, {0, 0, -1}},
            {Vector3f{-h.x, h.y, -h.z}, {0, 1, 0}, {0, 1}, {1, 0, 0}, {0, 0, -1}},

            // Bottom face (normal: 0, -1, 0)
            {Vector3f{-h.x, -h.y, -h.z}, {0, -1, 0}, {0, 0}, {1, 0, 0}, {0, 0, 1}},
            {Vector3f{h.x, -h.y, -h.z}, {0, -1, 0}, {1, 0}, {1, 0, 0}, {0, 0, 1}},
            {Vector3f{h.x, -h.y, h.z}, {0, -1, 0}, {1, 1}, {1, 0, 0}, {0, 0, 1}},
            {Vector3f{-h.x, -h.y, h.z}, {0, -1, 0}, {0, 1}, {1, 0, 0}, {0, 0, 1}},
        };

        const std::vector<uint32_t> indices = {
            0, 2, 1, 2, 0, 3,       // Front
            4, 6, 5, 6, 4, 7,       // Back
            8, 10, 9, 10, 8, 11,    // Left
            12, 14, 13, 14, 12, 15, // Right
            16, 18, 17, 18, 16, 19, // Top
            20, 22, 21, 22, 20, 23, // Bottom
        };

        return Create({.vertices = vertices, .indices = indices}).value();
    }

    Mesh::~Mesh() {
        Clear();
    }

    void Mesh::Clear() {
        m_VertexArray.Clear();
        m_VertexBuffer.Clear();
        m_IndexBuffer.Clear();
        m_IndexCount = 0;
    }

    bool Mesh::Bind() const {
        return m_VertexArray.Bind();
    }

    void Mesh::Unbind() {
        VertexArray::Unbind();
    }

    usize Mesh::IndexCount() const {
        return m_IndexCount;
    }

    const MeshData &Mesh::Data() const {
        return m_Data;
    }
}
