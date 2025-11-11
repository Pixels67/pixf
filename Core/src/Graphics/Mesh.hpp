#ifndef MESH_HPP
#define MESH_HPP

#include <vector>

#include "Gl/IndexBuffer.hpp"
#include "Gl/VertexArray.hpp"
#include "Gl/VertexBuffer.hpp"
#include "Vertex.hpp"

namespace Pixf::Core::Graphics {
    class PIXF_API Mesh {
    public:
        Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices);

        Mesh(const Mesh &other) = delete;
        Mesh(Mesh &&other) noexcept;

        Mesh &operator=(const Mesh &other) = delete;
        Mesh &operator=(Mesh &&other) noexcept;

        ~Mesh();

        void Bind() const;
        static void Unbind();

        void Cleanup();

        unsigned int GetIndexCount() const;

    private:
        Gl::VertexArray m_Vao;
        Gl::VertexBuffer m_Vbo;
        Gl::IndexBuffer m_Ibo;
        unsigned int m_IndexCount;
    };
} // namespace Pixf::Core::Graphics

#endif // MESH_HPP
