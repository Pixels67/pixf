#include "IndexBuffer.hpp"

#include <vector>

#include "Debug/Logger.hpp"
#include "Gl.hpp"

namespace Pixf::Core::Graphics::Gl {
    IndexBuffer::IndexBuffer(const std::vector<unsigned int> &data) {
        PIXF_GL_CALL(glGenBuffers(1, &m_Id));
        PIXF_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Id));
        PIXF_GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * data.size(), data.data(), GL_STATIC_DRAW));
        PIXF_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
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

    IndexBuffer::~IndexBuffer() {
        Cleanup();
    }

    void IndexBuffer::Bind() const { PIXF_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Id)); }

    void IndexBuffer::Unbind() { PIXF_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0)); }

    void IndexBuffer::Cleanup() const {
        if (m_Id == 0) {
            return;
        }

        PIXF_GL_CALL(glDeleteBuffers(1, &m_Id));
    }
} // namespace Pixf::Core::Graphics::Gl
