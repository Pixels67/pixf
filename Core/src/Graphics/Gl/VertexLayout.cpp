#include "VertexLayout.hpp"

#include "Gl.hpp"

namespace Pixf::Core::Graphics::Gl {
    size_t GetSize(const AttribType type) {
        size_t size = 0;

        switch (type) {
            case AttribType::Float32:
                size = sizeof(float);
                break;
            case AttribType::Int32:
                size = sizeof(int);
                break;
            case AttribType::UInt32:
                size = sizeof(unsigned int);
                break;
            case AttribType::Int8:
                size = sizeof(char);
                break;
            case AttribType::UInt8:
                size = sizeof(unsigned char);
                break;
        }

        PIXF_ASSERT(size != 0, "Unknown attrib type");

        return size;
    }

    unsigned int ToGlAttribType(const AttribType type) {
        unsigned int glType = 0;

        switch (type) {
            case AttribType::Float32:
                glType = GL_FLOAT;
                break;
            case AttribType::Int32:
                glType = GL_INT;
                break;
            case AttribType::UInt32:
                glType = GL_UNSIGNED_INT;
                break;
            case AttribType::Int8:
                glType = GL_BYTE;
                break;
            case AttribType::UInt8:
                glType = GL_UNSIGNED_BYTE;
                break;
        }

        PIXF_ASSERT(glType != 0, "Unknown attrib type");

        return glType;
    }

    VertexLayout &VertexLayout::Add(const unsigned int count, const AttribType type, const bool normalized) {
        const unsigned int elementCount = m_Elements.size();

        const VertexLayoutElement element{m_Stride, elementCount, count, type, normalized};
        m_Elements.push_back(element);

        m_Stride += count * GetSize(type);

        return *this;
    }

    VertexLayout &VertexLayout::Add(const unsigned int count,
                                    const AttribType type,
                                    const unsigned int index,
                                    const size_t offset,
                                    const bool normalized) {
        const VertexLayoutElement element{offset, index, count, type, normalized};
        m_Elements.push_back(element);

        const size_t required = offset + (count * GetSize(type));
        m_Stride = std::max(m_Stride, required);

        return *this;
    }

    void VertexLayout::Bind() const {
        for (const auto &[offset, index, count, type, normalized]: m_Elements) {
            const void *offsetPtr = reinterpret_cast<void *>(offset);

            PIXF_GL_CALL(glVertexAttribPointer(index, count, ToGlAttribType(type), normalized, m_Stride, offsetPtr));
            PIXF_GL_CALL(glEnableVertexAttribArray(index));
        }
    }

    void VertexLayout::Unbind() const {
        for (const auto &element: m_Elements) {
            PIXF_GL_CALL(glDisableVertexAttribArray(element.index));
        }
    }
} // namespace Pixf::Core::Graphics::Gl
