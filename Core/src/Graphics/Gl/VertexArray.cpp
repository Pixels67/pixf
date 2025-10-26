#include "VertexArray.hpp"

#include "Debug/Logger.hpp"
#include "Gl.hpp"

namespace Pixf::Core::Graphics::Gl {
    void VertexArray::Init() { PIXF_GL_CALL(glGenVertexArrays(1, &m_Id)); }

    VertexArray::VertexArray(VertexArray &&other) noexcept : m_Id{other.m_Id} { other.m_Id = 0; }

    VertexArray &VertexArray::operator=(VertexArray &&other) noexcept {
        if (this == &other) {
            return *this;
        }

        Cleanup();

        m_Id = other.m_Id;
        other.m_Id = 0;

        return *this;
    }

    VertexArray::~VertexArray() {
        Cleanup();
    }

    void VertexArray::Bind() const { PIXF_GL_CALL(glBindVertexArray(m_Id)); }

    void VertexArray::Unbind() { PIXF_GL_CALL(glBindVertexArray(0)); }

    void VertexArray::Cleanup() const {
        if (m_Id == 0) {
            return;
        }

        PIXF_GL_CALL(glDeleteVertexArrays(1, &m_Id));
    }
} // namespace Pixf::Core::Graphics::Gl
