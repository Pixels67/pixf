#include "Mesh.hpp"

namespace Flock::Graphics {
    std::optional<Mesh> Mesh::Create(const MeshData &data) {
        Mesh mesh{};

        mesh.m_Data = data;

        mesh.m_VertexArray  = VertexArray::Create();
        mesh.m_VertexBuffer = Buffer::Create(data.vertices, BufferType::Vertex);
        mesh.m_IndexBuffer  = Buffer::Create(data.indices, BufferType::Index);

        if (!mesh.m_VertexArray.SetVertexBuffer(mesh.m_VertexBuffer, Vertex::GetLayout())) {
            return std::nullopt;
        }

        if (!mesh.m_VertexArray.SetIndexBuffer(mesh.m_IndexBuffer)) {
            return std::nullopt;
        }

        mesh.m_IndexCount  = data.indices.size();
        mesh.m_Initialized = true;

        return mesh;
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
            0, 1, 2, 2, 3, 0,       // Front
            4, 5, 6, 6, 7, 4,       // Back
            8, 9, 10, 10, 11, 8,    // Left
            12, 13, 14, 14, 15, 12, // Right
            16, 17, 18, 18, 19, 16, // Top
            20, 21, 22, 22, 23, 20, // Bottom
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

    usize Mesh::GetIndexCount() const {
        return m_IndexCount;
    }

    const MeshData &Mesh::GetData() const {
        return m_Data;
    }
}
