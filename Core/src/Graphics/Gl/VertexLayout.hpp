#ifndef PIXF_VERTEXLAYOUT_HPP
#define PIXF_VERTEXLAYOUT_HPP

#include "Gl.hpp"
#include "Common.hpp"

namespace Pixf::Core::Graphics::Gl {
    enum class AttribType : uint8_t {
        Float32,
        Int32,
        UInt32,
        Int8,
        UInt8,
    };

    size_t GetSize(AttribType type);
    unsigned int ToGlAttribType(AttribType type);

    struct PIXF_API VertexLayoutElement {
        size_t offset;
        unsigned int index;
        unsigned int count;
        AttribType type;
        bool normalized;
    };

    class PIXF_API VertexLayout {
    public:
        VertexLayout &Add(unsigned int count, AttribType type, bool normalized = false);

        VertexLayout &
        Add(unsigned int count, AttribType type, unsigned int index, size_t offset, bool normalized = false);

        void Bind() const;
        void Unbind() const;

    private:
        std::vector<VertexLayoutElement> m_Elements;
        size_t m_Stride = 0;
    };
} // namespace Pixf::Core::Graphics::Gl

#endif // PIXF_VERTEXLAYOUT_HPP
