#ifndef PIXF_GL_BUFFER_HPP
#define PIXF_GL_BUFFER_HPP

#include "Common.hpp"
#include "Memory/Buffer.hpp"

namespace Pixf::Core::Graphics::Gl {
    enum class BufferType : uint8_t {
        None,
        VertexBuffer,
        IndexBuffer,
    };

    unsigned int ToGlBufferType(BufferType type);

    enum class BufferUsage : uint8_t {
        StaticDraw,
        DynamicDraw,
    };

    unsigned int ToGlBufferUsage(BufferUsage usage);

    class PIXF_API Buffer {
    public:
        Buffer() = default;

        explicit Buffer(const Memory::Buffer &buffer, BufferType type, BufferUsage usage = BufferUsage::StaticDraw);

        Buffer(const Buffer &other) = delete;
        Buffer(Buffer &&other) noexcept;

        Buffer &operator=(const Buffer &other) = delete;
        Buffer &operator=(Buffer &&other) noexcept;

        ~Buffer();

        BufferType GetType() const;

        void Bind() const;
        static void Unbind(BufferType type);

        void Clear() const;

    private:
        unsigned int m_Id = 0;
        BufferType m_Type = BufferType::None;
    };
} // namespace Pixf::Core::Graphics::Gl

#endif // PIXF_GL_BUFFER_HPP
