#include "Mesh.hpp"

namespace Pixf::Core::Graphics {
    Mesh::Mesh(const MeshData &data) : m_IndexCount(data.indices.size()) {
        m_VertexBuffer = Gl::Buffer(data.vertices, Gl::BufferType::VertexBuffer);
        m_IndexBuffer = Gl::Buffer(data.indices, Gl::BufferType::IndexBuffer);
        m_VertexArray = Gl::VertexArray();

        m_VertexArray.AddVertexBuffer(m_VertexBuffer, Vertex::GetLayout());
        m_VertexArray.AddIndexBuffer(m_IndexBuffer);
    }

    size_t Mesh::GetIndexCount() const { return m_IndexCount; }

    void Mesh::Bind() const { m_VertexArray.Bind(); }

    void Mesh::Unbind() { Gl::VertexArray::Unbind(); }

    void Mesh::Clear() const {
        m_VertexArray.Clear();
        m_VertexBuffer.Clear();
        m_IndexBuffer.Clear();
    }
} // namespace Pixf::Core::Graphics
