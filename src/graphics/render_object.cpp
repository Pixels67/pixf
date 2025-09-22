#include "render_object.h"

#include <glad/glad.h>

namespace Engine::Graphics {
	RenderObject::RenderObject(const std::vector<float> &vertices, const std::vector<unsigned int> &indices) {
		m_vertBuf = VertBuf(vertices, indices, GL_STATIC_DRAW);

		VertBufLayout layout(1);
		layout.PushBack<float>(3);

		m_vertArr = VertArr(m_vertBuf, layout);
	}

	void RenderObject::Render(const Shader &shader) const {
		shader.Bind();
		m_vertArr.Bind();

		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_vertBuf.GetIndexCount()), GL_UNSIGNED_INT, nullptr);
	}
} // namespace Engine::Graphics
