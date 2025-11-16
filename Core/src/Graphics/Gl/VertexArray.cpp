#include "VertexArray.hpp"

#include "Gl.hpp"

namespace Pixf::Core::Graphics::Gl {
    VertexArray::VertexArray() { PIXF_GL_CALL(glGenVertexArrays(1, &m_Id)); }

    VertexArray::VertexArray(VertexArray &&other) noexcept : m_Id{other.m_Id} { other.m_Id = 0; }

    VertexArray &VertexArray::operator=(VertexArray &&other) noexcept {
        if (this == &other) {
            return *this;
        }

        Clear();

        m_Id = other.m_Id;
        other.m_Id = 0;

        return *this;
    }

    VertexArray::~VertexArray() { Clear(); }

    void VertexArray::AddVertexBuffer(const Buffer &buffer, const VertexLayout &layout) const {
        PIXF_ASSERT(buffer.GetType() == BufferType::VertexBuffer, "Buffer is not a vertex buffer");

        Bind();

        buffer.Bind();
        layout.Bind();

        Unbind();
        Buffer::Unbind(BufferType::VertexBuffer);
    }

    void VertexArray::AddIndexBuffer(const Buffer &buffer) const {
        PIXF_ASSERT(buffer.GetType() == BufferType::IndexBuffer, "Buffer is not an index buffer");

        Bind();

        buffer.Bind();

        Unbind();
        Buffer::Unbind(BufferType::IndexBuffer);
    }

    void VertexArray::Bind() const { PIXF_GL_CALL(glBindVertexArray(m_Id)); }

    void VertexArray::Unbind() { PIXF_GL_CALL(glBindVertexArray(0)); }

    void VertexArray::Clear() const {
        if (m_Id == 0) {
            return;
        }

        PIXF_GL_CALL(glDeleteVertexArrays(1, &m_Id));
    }
} // namespace Pixf::Core::Graphics::Gl
