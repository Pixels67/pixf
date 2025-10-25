#ifndef INDEXBUFFER_HPP
#define INDEXBUFFER_HPP

#include <vector>

namespace Pixf::Core::Graphics::Gl {

    class IndexBuffer {
    public:
        IndexBuffer() = default;

        explicit IndexBuffer(const std::vector<unsigned int> &data);

        IndexBuffer(const IndexBuffer &other) = delete;
        IndexBuffer(IndexBuffer &&other) noexcept;

        IndexBuffer &operator=(const IndexBuffer &other) = delete;
        IndexBuffer &operator=(IndexBuffer &&other) noexcept;

        ~IndexBuffer();

        void Bind() const;
        static void Unbind();

        void Cleanup() const;

    private:
        unsigned int m_Id = 0;
    };

} // namespace Pixf::Core::Graphics::Gl

#endif // INDEXBUFFER_HPP
