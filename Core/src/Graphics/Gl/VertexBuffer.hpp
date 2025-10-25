#ifndef BUFFER_HPP
#define BUFFER_HPP

#include <vector>

#include "../Vertex.hpp"

namespace Pixf::Core::Graphics::Gl {
    class VertexBuffer {
    public:
        VertexBuffer() = default;

        explicit VertexBuffer(const std::vector<Vertex> &data);

        VertexBuffer(const VertexBuffer &other) = delete;
        VertexBuffer(VertexBuffer &&other) noexcept;

        VertexBuffer &operator=(const VertexBuffer &other) = delete;
        VertexBuffer &operator=(VertexBuffer &&other) noexcept;

        ~VertexBuffer();

        void Bind() const;
        static void Unbind();

        void Cleanup() const;

    private:
        unsigned int m_Id = 0;
    };

} // namespace Pixf::Core::Graphics::Gl

#endif // BUFFER_HPP
