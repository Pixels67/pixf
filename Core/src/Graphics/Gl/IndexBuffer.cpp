#include "IndexBuffer.hpp"

#include <vector>

#include "Gl.hpp"

namespace Pixf::Core::Graphics::Gl {
    IndexBuffer::IndexBuffer(const std::vector<unsigned int> &data) {
        GL_CALL(glGenBuffers(1, &m_Id));
        GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Id));
        GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * data.size(), data.data(), GL_STATIC_DRAW));
        GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    }

    IndexBuffer::IndexBuffer(IndexBuffer &&other) noexcept : m_Id(other.m_Id) { other.m_Id = 0; }

    IndexBuffer &IndexBuffer::operator=(IndexBuffer &&other) noexcept {
        if (this == &other) {
            return *this;
        }

        Cleanup();

        m_Id = other.m_Id;
        other.m_Id = 0;

        return *this;
    }

    IndexBuffer::~IndexBuffer() { Cleanup(); }

    void IndexBuffer::Bind() const { GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Id)); }

    void IndexBuffer::Unbind() { GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0)); }

    void IndexBuffer::Cleanup() const {
        if (m_Id == 0) {
            return;
        }

        GL_CALL(glDeleteBuffers(1, &m_Id));
    }
} // namespace Pixf::Core::Graphics::Gl
