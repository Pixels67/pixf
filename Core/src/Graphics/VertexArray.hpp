#ifndef FLK_VERTEXARRAY_HPP
#define FLK_VERTEXARRAY_HPP

#include "Common.hpp"
#include "Buffer.hpp"
#include "VertexLayout.hpp"

namespace Flock::Graphics {
    /**
     * @class VertexArray
     * @brief An OpenGL Vertex Array Object.
     */
    class FLK_API VertexArray {
        u32 m_Id = 0;
        bool m_VertexSet = false;
        bool m_IndexSet = false;

    public:
        /**
         * @brief Static factory method.
         * @return A newly created Vertex Array.
         */
        static VertexArray Create();

        VertexArray() = default;
        ~VertexArray();

        VertexArray(const VertexArray &)          = delete;
        VertexArray(VertexArray &&other) noexcept;

        VertexArray &operator=(const VertexArray &)          = delete;
        VertexArray &operator=(VertexArray &&other) noexcept;

        /**
         * @brief Clears the Vertex Array.
         */
        void Clear() const;

        /**
         * @brief Binds the Vertex Array.
         * @return true if successful; false otherwise.
         */
        bool Bind() const;

        /**
         * @brief Unbinds the currently bound Vertex Array.
         */
        static void Unbind();

        /**
         * @brief Sets the Vertex Array vertex buffer.
         * @param buffer The vertex buffer.
         * @param layout The vertex layout.
         * @return true if succesful; false otherwise.
         */
        bool SetVertexBuffer(const Buffer &buffer, const VertexLayout &layout);

        /**
         * @brief Sets the Vertex Array index buffer.
         * @param buffer The index buffer.
         * @return true if succesful; false otherwise.
         */
        bool SetIndexBuffer(const Buffer &buffer);
    };
}

#endif //FLK_VERTEXARRAY_HPP
