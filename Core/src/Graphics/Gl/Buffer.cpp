#include "Buffer.hpp"

#include "Gl.hpp"
#include "Memory/Buffer.hpp"

namespace Pixf::Core::Graphics::Gl {
    unsigned int ToGlBufferType(const BufferType type) {
        unsigned int glBufferType = 0;

        switch (type) {
            case BufferType::VertexBuffer:
                glBufferType = GL_ARRAY_BUFFER;
                break;
            case BufferType::IndexBuffer:
                glBufferType = GL_ELEMENT_ARRAY_BUFFER;
                break;
            case BufferType::None:
                PIXF_ASSERT(false, "Invalid buffer type");
                break;
        }

        PIXF_ASSERT(glBufferType != 0, "Unknown buffer type");

        return glBufferType;
    }

    unsigned int ToGlBufferUsage(const BufferUsage usage) {
        unsigned int glBufferUsage = 0;

        switch (usage) {
            case BufferUsage::StaticDraw:
                glBufferUsage = GL_STATIC_DRAW;
                break;
            case BufferUsage::DynamicDraw:
                glBufferUsage = GL_DYNAMIC_DRAW;
                break;
        }

        PIXF_ASSERT(glBufferUsage != 0, "Unknown buffer usage");

        return glBufferUsage;
    }

    Buffer::Buffer(const Memory::Buffer &buffer, const BufferType type, const BufferUsage usage) : m_Type(type) {
        PIXF_GL_CALL(glGenBuffers(1, &m_Id));
        PIXF_GL_CALL(glBindBuffer(ToGlBufferType(type), m_Id));

        PIXF_GL_CALL(glBufferData(ToGlBufferType(type), buffer.GetSize(), buffer.Get(), ToGlBufferUsage(usage)));

        PIXF_GL_CALL(glBindBuffer(ToGlBufferType(type), 0));
    }

    Buffer::Buffer(Buffer &&other) noexcept : m_Id(other.m_Id), m_Type(other.m_Type) { other.m_Id = 0; }

    Buffer &Buffer::operator=(Buffer &&other) noexcept {
        if (this == &other) {
            return *this;
        }

        Clear();

        m_Id = other.m_Id;
        m_Type = other.m_Type;
        other.m_Id = 0;

        return *this;
    }

    Buffer::~Buffer() { Clear(); }

    BufferType Buffer::GetType() const { return m_Type; }

    void Buffer::Bind() const { PIXF_GL_CALL(glBindBuffer(ToGlBufferType(m_Type), m_Id)); }

    void Buffer::Unbind(const BufferType type) { PIXF_GL_CALL(glBindBuffer(ToGlBufferType(type), 0)); }

    void Buffer::Clear() const {
        if (m_Id == 0) {
            return;
        }

        PIXF_GL_CALL(glDeleteBuffers(1, &m_Id));
    }
} // namespace Pixf::Core::Graphics::Gl
