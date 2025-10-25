#include "Mesh.hpp"

#include <vector>

#include "Gl/IndexBuffer.hpp"
#include "Gl/VertexArray.hpp"
#include "Gl/VertexBuffer.hpp"
#include "Gl/VertexLayout.hpp"
#include "Vertex.hpp"

namespace Pixf::Core::Graphics {
    Mesh::Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices) :
        m_IndexCount(indices.size()) {
        Gl::VertexLayout layout{};
        layout.Push<float>(3);
        layout.Push<float>(3);
        layout.Push<float>(2);

        m_Vao.Init();
        m_Vbo = Gl::VertexBuffer(vertices);
        m_Ibo = Gl::IndexBuffer(indices);

        m_Vao.Bind();
        m_Vbo.Bind();
        m_Ibo.Bind();
        layout.Bind();

        Gl::VertexArray::Unbind();
    }

    Mesh::Mesh(Mesh &&other) noexcept :
        m_Vao(std::move(other.m_Vao)), m_Vbo(std::move(other.m_Vbo)), m_Ibo(std::move(other.m_Ibo)),
        m_IndexCount(other.m_IndexCount) {}

    Mesh &Mesh::operator=(Mesh &&other) noexcept {
        if (this == &other) {
            return *this;
        }

        Cleanup();

        m_Vao = std::move(other.m_Vao);
        m_Vbo = std::move(other.m_Vbo);
        m_Ibo = std::move(other.m_Ibo);
        m_IndexCount = other.m_IndexCount;

        return *this;
    }

    Mesh::~Mesh() { Cleanup(); }

    void Mesh::Bind() const { m_Vao.Bind(); }

    void Mesh::Unbind() { Gl::VertexArray::Unbind(); }

    void Mesh::Cleanup() {
        m_Vao.Cleanup();
        m_Vbo.Cleanup();
        m_Ibo.Cleanup();
        m_IndexCount = 0;
    }

    unsigned int Mesh::GetIndexCount() const { return m_IndexCount; }
} // namespace Pixf::Core::Graphics
