#include "VertexArray.hpp"

#include "Gl.hpp"
#include "Graphics/Buffer.hpp"
#include "Graphics/VertexLayout.hpp"
#include "glad/glad.h"

namespace Flock::Graphics {
    VertexArray VertexArray::Create() {
        VertexArray vertArr{};
        FLK_GL_CALL(glGenVertexArrays(1, &vertArr.m_Id));

        return vertArr;
    }

    VertexArray::~VertexArray() {
        Clear();
    }

    VertexArray::VertexArray(VertexArray &&other) noexcept {
        m_Id        = other.m_Id;
        m_VertexSet = other.m_VertexSet;
        m_IndexSet  = other.m_IndexSet;
        other.m_Id  = 0;
    }

    VertexArray &VertexArray::operator=(VertexArray &&other) noexcept {
        if (this == &other) {
            return *this;
        }

        Clear();

        m_Id        = other.m_Id;
        m_VertexSet = other.m_VertexSet;
        m_IndexSet  = other.m_IndexSet;
        other.m_Id  = 0;

        return *this;
    }

    void VertexArray::Clear() const {
        if (m_Id == 0) {
            return;
        }

        FLK_GL_CALL(glDeleteVertexArrays(1, &m_Id));
    }

    bool VertexArray::Bind() const {
        if (m_Id == 0 || !m_VertexSet) {
            return false;
        }

        FLK_GL_CALL(glBindVertexArray(m_Id));
        return true;
    }

    void VertexArray::Unbind() {
        FLK_GL_CALL(glBindVertexArray(0));
    }

    bool VertexArray::SetVertexBuffer(const Buffer &buffer, const VertexLayout &layout) {
        if (m_Id == 0 || buffer.Type() != BufferType::Vertex) {
            return false;
        }

        i32 boundVertArr;
        FLK_GL_CALL(glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &boundVertArr));

        FLK_GL_CALL(glBindVertexArray(m_Id));
        if (!buffer.Bind()) {
            return false;
        }

        layout.Bind();

        FLK_GL_CALL(glBindVertexArray(boundVertArr));
        m_VertexSet = true;
        return true;
    }

    bool VertexArray::SetIndexBuffer(const Buffer &buffer) {
        if (m_Id == 0 || buffer.Type() != BufferType::Index) {
            return false;
        }

        i32 boundVertArr;
        FLK_GL_CALL(glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &boundVertArr));

        FLK_GL_CALL(glBindVertexArray(m_Id));
        if (!buffer.Bind()) {
            return false;
        }

        FLK_GL_CALL(glBindVertexArray(boundVertArr));
        m_IndexSet = true;
        return true;
    }
}
