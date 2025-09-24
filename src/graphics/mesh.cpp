#include "mesh.h"

#include <glad/glad.h>

#include "gtc/type_ptr.inl"

namespace Engine::Graphics {
    Mesh::Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices) {
        std::vector<float> vertexData{};
        for (auto [position, texCoords] : vertices) {
            vertexData.push_back(position.x);
            vertexData.push_back(position.y);
            vertexData.push_back(position.z);
            vertexData.push_back(texCoords.s);
            vertexData.push_back(texCoords.t);
        }

        m_VertBuf = VertBuf(vertexData, indices, GL_STATIC_DRAW);

        VertBufLayout layout(2);
        layout.PushBack<float>(3);
        layout.PushBack<float>(2);

        m_VertArr = VertArr(m_VertBuf, layout);
    }

    void Mesh::Render(const Material &material, const Core::Transform &transform, const Camera &camera) const {
        m_VertArr.Bind();

        constexpr int width = 800;
        constexpr int height = 600;
        const glm::mat4 proj = glm::perspective<float>(
            glm::radians(60.0f),
            static_cast<float>(width) / static_cast<float>(height),
            0.1f,
            100.0f
        );

        material.shader.SetUniform("proj", proj);
        material.shader.SetUniform("view", camera.GetViewMatrix());
        material.shader.SetUniform("model", transform.GetMatrix());
        material.Bind();

        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_VertBuf.GetIndexCount()), GL_UNSIGNED_INT, nullptr);
        Material::Unbind();
    }
} // namespace Engine::Graphics
