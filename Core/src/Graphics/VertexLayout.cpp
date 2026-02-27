#include "VertexLayout.hpp"

#include "Gl.hpp"

namespace Flock::Graphics {
    usize GetSize(const AttribType type) {
        usize size = 0;

        switch (type) {
            case AttribType::U8:
                size = sizeof(u8);
                break;
            case AttribType::I8:
                size = sizeof(char);
                break;
            case AttribType::U32:
                size = sizeof(u32);
                break;
            case AttribType::I32:
                size = sizeof(i32);
                break;
            case AttribType::F32:
                size = sizeof(f32);
                break;
        }

        FLK_EXPECT(size != 0, "Unknown attrib type");

        return size;
    }

    u32 ToGlType(const AttribType type) {
        u32 glType = 0;

        switch (type) {
            case AttribType::U8:
                glType = GL_UNSIGNED_BYTE;
                break;
            case AttribType::I8:
                glType = GL_BYTE;
                break;
            case AttribType::U32:
                glType = GL_UNSIGNED_INT;
                break;
            case AttribType::I32:
                glType = GL_INT;
                break;
            case AttribType::F32:
                glType = GL_FLOAT;
                break;
        }

        FLK_EXPECT(glType != 0, "Unknown attrib type");

        return glType;
    }

    VertexLayout &VertexLayout::Add(const u32 count, const AttribType type, const bool normalized) {
        const u32 elementCount = m_Elements.size();

        const VertexLayoutElement element{m_Stride, elementCount, count, type, normalized};
        m_Elements.push_back(element);

        m_Stride += count * GetSize(type);

        return *this;
    }

    VertexLayout &VertexLayout::Add(
        const u32  count, const AttribType type,
        const u32  index, const usize      offset,
        const bool normalized
    ) {
        const VertexLayoutElement element{offset, index, count, type, normalized};
        m_Elements.push_back(element);

        const usize required = offset + (count * GetSize(type));
        m_Stride             = std::max(m_Stride, required);

        return *this;
    }

    void VertexLayout::Bind() const {
        for (const auto &[offset, index, count, type, normalized]: m_Elements) {
            const void *offsetPtr = reinterpret_cast<void *>(offset);

            FLK_GL_CALL(glVertexAttribPointer(index, count, ToGlType(type), normalized, m_Stride, offsetPtr));
            FLK_GL_CALL(glEnableVertexAttribArray(index));
        }
    }

    void VertexLayout::Unbind() const {
        for (const auto &element: m_Elements) {
            FLK_GL_CALL(glDisableVertexAttribArray(element.index));
        }
    }
}
