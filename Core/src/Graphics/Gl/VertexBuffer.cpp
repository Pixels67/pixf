#include "VertexBuffer.hpp"

#include "Gl.hpp"

namespace Pixf::Core::Graphics::Gl {
    VertexBuffer::VertexBuffer(const std::vector<Vertex> &data) {
        GL_CALL(glGenBuffers(1, &m_Id));
        GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_Id));
        GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * data.size(), data.data(), GL_STATIC_DRAW));
        GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
    }

    VertexBuffer::VertexBuffer(VertexBuffer &&other) noexcept : m_Id(other.m_Id) { other.m_Id = 0; }

    VertexBuffer &VertexBuffer::operator=(VertexBuffer &&other) noexcept {
        if (this == &other) {
            return *this;
        }

        Cleanup();

        m_Id = other.m_Id;
        other.m_Id = 0;

        return *this;
    }

    VertexBuffer::~VertexBuffer() { Cleanup(); }

    void VertexBuffer::Bind() const { GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_Id)); }

    void VertexBuffer::Unbind() { GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0)); }

    void VertexBuffer::Cleanup() const {
        if (m_Id == 0) {
            return;
        }

        GL_CALL(glDeleteBuffers(1, &m_Id));
    }
} // namespace Pixf::Core::Graphics::Gl
