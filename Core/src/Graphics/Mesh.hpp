#ifndef PIXF_MESH_HPP
#define PIXF_MESH_HPP

#include <vector>

#include "Gl/Buffer.hpp"
#include "Gl/VertexArray.hpp"
#include "Vertex.hpp"

namespace Pixf::Core::Graphics {
    struct PIXF_API MeshData {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
    };

    class PIXF_API Mesh {
    public:
        Mesh() = default;
        explicit Mesh(const MeshData &data);

        size_t GetIndexCount() const;

        void Bind() const;
        static void Unbind();

        void Clear() const;

    private:
        Gl::VertexArray m_VertexArray;
        Gl::Buffer m_VertexBuffer;
        Gl::Buffer m_IndexBuffer;
        size_t m_IndexCount = 0;
    };
} // namespace Pixf::Core::Graphics

#endif // PIXF_MESH_HPP
