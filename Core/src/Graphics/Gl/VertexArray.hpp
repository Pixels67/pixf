#ifndef PIXF_VERTEXARRAY_HPP
#define PIXF_VERTEXARRAY_HPP

#include "Buffer.hpp"
#include "VertexLayout.hpp"
#include "Common.hpp"

namespace Pixf::Core::Graphics::Gl {
    class PIXF_API VertexArray {
    public:
        VertexArray();

        VertexArray(const VertexArray &) = delete;
        VertexArray(VertexArray &&other) noexcept;
        VertexArray &operator=(const VertexArray &) = delete;
        VertexArray &operator=(VertexArray &&other) noexcept;

        ~VertexArray();

        void AddVertexBuffer(const Buffer &buffer, const VertexLayout &layout) const;
        void AddIndexBuffer(const Buffer &buffer) const;

        void Bind() const;
        static void Unbind();

        void Clear() const;

    private:
        unsigned int m_Id = 0;
    };
} // namespace Pixf::Core::Graphics::Gl

#endif // PIXF_VERTEXARRAY_HPP
