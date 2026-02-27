#include "Buffer.hpp"

#include "Gl.hpp"

namespace Flock::Graphics {
    u32 ToGlType(const BufferType type) {
        u32 glBufferType = 0;

        switch (type) {
            case BufferType::Vertex:
                glBufferType = GL_ARRAY_BUFFER;
                break;
            case BufferType::Index:
                glBufferType = GL_ELEMENT_ARRAY_BUFFER;
                break;
            case BufferType::None:
                FLK_EXPECT(false, "Invalid buffer type");
        }

        FLK_EXPECT(glBufferType != 0, "Unknown buffer type");

        return glBufferType;
    }

    u32 ToGlType(const BufferUsage usage) {
        u32 glBufferUsage = 0;

        switch (usage) {
            case BufferUsage::StaticDraw:
                glBufferUsage = GL_STATIC_DRAW;
                break;
            case BufferUsage::DynamicDraw:
                glBufferUsage = GL_DYNAMIC_DRAW;
                break;
        }

        FLK_EXPECT(glBufferUsage != 0, "Unknown buffer usage");

        return glBufferUsage;
    }

    Buffer Buffer::Create(const Memory::Buffer &buffer, const BufferType type, const BufferUsage usage) {
        Buffer buf{};
        buf.m_Type = type;

        FLK_GL_CALL(glGenBuffers(1, &buf.m_Id));
        FLK_GL_CALL(glBindBuffer(ToGlType(type), buf.m_Id));

        FLK_GL_CALL(glBufferData(ToGlType(type), buffer.GetSize(), buffer.Get(), ToGlType(usage)));

        FLK_GL_CALL(glBindBuffer(ToGlType(type), 0));

        return buf;
    }

    Buffer::Buffer(Buffer &&other) noexcept
        : m_Id(other.m_Id), m_Type(other.m_Type) {
        other.m_Id = 0;
    }

    Buffer &Buffer::operator=(Buffer &&other) noexcept {
        if (this == &other) {
            return *this;
        }

        Clear();

        m_Id       = other.m_Id;
        m_Type     = other.m_Type;
        other.m_Id = 0;

        return *this;
    }

    Buffer::~Buffer() {
        Clear();
    }

    BufferType Buffer::GetType() const {
        return m_Type;
    }

    bool Buffer::Bind() const {
        if (m_Id == 0) {
            return false;
        }

        FLK_GL_CALL(glBindBuffer(ToGlType(m_Type), m_Id));
        return true;
    }

    void Buffer::Unbind(const BufferType type) {
        FLK_GL_CALL(glBindBuffer(ToGlType(type), 0));
    }

    void Buffer::Clear() const {
        if (m_Id == 0) {
            return;
        }

        FLK_GL_CALL(glDeleteBuffers(1, &m_Id));
    }
}
