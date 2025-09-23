#include "render_object.h"

#include <glad/glad.h>

namespace Engine::Graphics {
    RenderObject::RenderObject(const std::vector<float> &vertices, const std::vector<unsigned int> &indices)
        : m_VertBuf(VertBuf(vertices, indices, GL_STATIC_DRAW)) {
        VertBufLayout layout(2);
        layout.PushBack<float>(3);
        layout.PushBack<float>(2);

        m_VertArr = VertArr(m_VertBuf, layout);
    }

    void RenderObject::Render(const Shader &shader) const {
        shader.Bind();
        m_VertArr.Bind();

        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_VertBuf.GetIndexCount()), GL_UNSIGNED_INT, nullptr);
    }
} // namespace Engine::Graphics
