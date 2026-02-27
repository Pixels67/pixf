#ifndef FLK_MESH_HPP
#define FLK_MESH_HPP

#include "Common.hpp"
#include "Vertex.hpp"
#include "VertexArray.hpp"

namespace Flock::Graphics {
    /**
     * @struct MeshData
     * @brief The data required for constructing a Mesh.
     */
    struct FLK_API MeshData {
        std::vector<Vertex> vertices;
        std::vector<u32>    indices;
    };

    /**
     * @class Mesh
     * @brief If you don't know what a mesh is then you shouldn't be here.
     */
    class FLK_API Mesh {
        VertexArray m_VertexArray;
        Buffer      m_VertexBuffer;
        Buffer      m_IndexBuffer;
        usize       m_IndexCount  = 0;
        bool        m_Initialized = false;

    public:
        /**
         * @brief Static factory method.
         * @param data The mesh data.
         * @return The mesh if successful; std::nullopt otherwise.
         */
        static std::optional<Mesh> Create(const MeshData &data);

        Mesh() = default;
        ~Mesh();

        Mesh(const Mesh &other)     = delete;
        Mesh(Mesh &&other) noexcept = default;

        Mesh &operator=(const Mesh &other)     = delete;
        Mesh &operator=(Mesh &&other) noexcept = default;

        /**
         * @brief Clears the mesh.
         */
        void Clear();

        /**
         * @brief Binds the mesh.
         * @return true if successful; false otherwise.
         */
        bool Bind() const;

        /**
         * @brief Unbinds the currently bound mesh.
         */
        static void Unbind();

        /**
         * @return The index count of the mesh.
         */
        [[nodiscard]] usize GetIndexCount() const;
    };
}

#endif //FLK_MESH_HPP
