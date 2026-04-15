#ifndef FLK_VERTEXLAYOUT_HPP
#define FLK_VERTEXLAYOUT_HPP

#include <vector>

#include "Common.hpp"

namespace Flock::Graphics {
    /**
     * @enum AttribType
     * @brief a Vertex Layout attribute type.
     */
    enum class AttribType : u8 {
        U8,
        I8,
        U32,
        I32,
        F32
    };

    usize Size(AttribType type);
    u32   ToGlType(AttribType type);

    struct VertexLayoutElement {
        usize      offset;
        u32        index;
        u32        count;
        AttribType type;
        bool       normalized;
    };

    /**
     * @class VertexLayout
     * @brief An OpenGL Vertex Attribute Layout.
     */
    class FLK_API VertexLayout {
        std::vector<VertexLayoutElement> m_Elements;
        usize                            m_Stride = 0;

    public:
        /**
         * @brief Adds an attribute to the vertex layout.
         * @param count The attribute element count.
         * @param type The type of the attribute.
         * @param normalized Whether the attribute is normalized or not.
         * @return A reference to the vertex layout for chaining.
         */
        VertexLayout &Add(u32 count, AttribType type, bool normalized = false);

        /**
         * @brief Adds an attribute to the vertex layout with manually-set index and offset.
         * @param count The attribute element count.
         * @param type The type of the attribute.
         * @param index The index of the attribute.
         * @param offset The offset of the attribute
         * @param normalized Whether the attribute is normalized or not.
         * @return A reference to the vertex layout for chaining.
         */
        VertexLayout &Add(u32 count, AttribType type, u32 index, usize offset, bool normalized = false);

        /**
         * @brief Binds the vertex layout to the currently bound Vertex Array.
         */
        void Bind() const;

        /**
         * @brief Unbinds the vertex layout from the currently bound Vertex Array.
         */
        void Unbind() const;
    };
}

#endif //FLK_VERTEXLAYOUT_HPP
