#ifndef VERTEXARRAY_HPP
#define VERTEXARRAY_HPP

namespace Pixf::Core::Graphics::Gl {
    class VertexArray {
    public:
        VertexArray() = default;
        void Init();

        VertexArray(const VertexArray &other) = delete;
        VertexArray(VertexArray &&other) noexcept;

        VertexArray &operator=(const VertexArray &other) = delete;
        VertexArray &operator=(VertexArray &&other) noexcept;

        ~VertexArray();

        void Bind() const;
        static void Unbind();

        void Cleanup() const;

    private:
        unsigned int m_Id = 0;
    };
} // namespace Pixf::Core::Graphics::Gl

#endif // VERTEXARRAY_HPP
