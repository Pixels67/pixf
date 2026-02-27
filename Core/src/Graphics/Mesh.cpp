#include "Mesh.hpp"

namespace Flock::Graphics {
    std::optional<Mesh> Mesh::Create(const MeshData &data) {
        Mesh mesh{};

        mesh.m_VertexArray  = VertexArray::Create();
        mesh.m_VertexBuffer = Buffer::Create(data.vertices, BufferType::Vertex);
        mesh.m_IndexBuffer  = Buffer::Create(data.indices, BufferType::Index);

        if (!mesh.m_VertexArray.SetVertexBuffer(mesh.m_VertexBuffer, Vertex::GetLayout())) {
            return std::nullopt;
        }

        if (!mesh.m_VertexArray.SetIndexBuffer(mesh.m_IndexBuffer)) {
            return std::nullopt;
        }

        mesh.m_IndexCount = data.indices.size();
        mesh.m_Initialized = true;

        return mesh;
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
}
