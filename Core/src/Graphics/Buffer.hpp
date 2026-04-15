#ifndef FLK_GL_BUFFER_HPP
#define FLK_GL_BUFFER_HPP

#include "Common.hpp"
#include "Memory/Buffer.hpp"

namespace Flock::Graphics {
    /**
     * @enum BufferType
     * @brief OpenGL Buffer Object type.
     */
    enum class BufferType : u8 {
        None,
        Vertex,
        Index,
    };

    u32 ToGlType(BufferType type);

    /**
     * @enum BufferUsage
     * @brief OpenGL Buffer Object usage.
     */
    enum class BufferUsage : u8 {
        StaticDraw,
        DynamicDraw,
    };

    u32 ToGlType(BufferUsage usage);

    /**
     * @class Buffer
     * @brief OpenGL Buffer Object.
     */
    class FLK_API Buffer {
        u32        m_Id   = 0;
        BufferType m_Type = BufferType::None;

    public:
        /**
         * @brief Static factory method.
         * @param buffer The memory buffer to copy from.
         * @param type The type of the buffer.
         * @param usage How the buffer data are intended to be used.
         * @return A newly created buffer.
         */
        static Buffer Create(const Memory::Buffer &buffer, BufferType type,
                             BufferUsage           usage = BufferUsage::StaticDraw);

        Buffer() = default;
        ~Buffer();

        Buffer(const Buffer &other) = delete;
        Buffer(Buffer &&other) noexcept;

        Buffer &operator=(const Buffer &other) = delete;
        Buffer &operator=(Buffer &&other) noexcept;

        /**
         * @brief Binds the buffer to the current context.
         * @return true if successful; false otherwise.
         */
        bool Bind() const;

        /**
         * @brief Unbinds a buffer type from the current context.
         * @param type The type of buffer to unbind.
         */
        static void Unbind(BufferType type);

        /**
         * @brief Clears the buffer.
         */
        void Clear() const;

        /**
         * @brief Retrieves the type of the buffer.
         * @return The type of the buffer.
         */
        [[nodiscard]] BufferType Type() const;
    };
}

#endif //FLK_GL_BUFFER_HPP
