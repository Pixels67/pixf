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

        MeshData m_Data;

    public:
        /**
         * @brief Static factory method.
         * @param data The mesh data.
         * @return The mesh if successful; std::nullopt otherwise.
         */
        static std::optional<Mesh> Create(const MeshData &data);

        /**
         * @brief Static factory method.
         * @param halfExtents The half extents of the square.
         * @return A newly created square mesh.
         */
        static Mesh Square(Vector2f halfExtents = Vector2f::One());

        /**
         * @brief Static factory method.
         * @param halfExtents The half extents of the box.
         * @return A newly created box mesh.
         */
        static Mesh Box(Vector3f halfExtents = Vector3f::One());

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

        [[nodiscard]] const MeshData &GetData() const;
    };
}

#endif //FLK_MESH_HPP
