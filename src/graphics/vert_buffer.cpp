#include "vert_buffer.h"

#include <glad/glad.h>

namespace Engine::Graphics {
	VertBuffer::VertBuffer(const std::vector<float> &vertices, const std::vector<unsigned int> &indices, unsigned int usage) {
		unsigned int vbo;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER,
		             static_cast<unsigned int>(vertices.size() * sizeof(float)),
		             vertices.data(),
		             usage);

		m_Vbo = vbo;
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		unsigned int ebo;
		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		             static_cast<unsigned int>(indices.size() * sizeof(int)),
		             indices.data(),
		             usage);

		m_Ebo = ebo;
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void VertBuffer::Bind() const {
		glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ebo);
	}

	void VertBuffer::Unbind() {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
} // namespace Engine::Graphics
