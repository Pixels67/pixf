#include "vert_buf.h"

#include <glad/glad.h>

namespace Engine::Graphics {
	VertBuf::VertBuf(const std::vector<float> &vertices, const std::vector<unsigned int> &indices,
	                 const unsigned int usage) {
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

		m_IndexCount = indices.size();
	}

	VertBuf::VertBuf(VertBuf &&other) noexcept {
		m_Vbo = other.m_Vbo;
		m_Ebo = other.m_Ebo;
		m_IndexCount = other.m_IndexCount;
		other.m_Vbo = 0;
		other.m_Ebo = 0;
		other.m_IndexCount = 0;
	}

	VertBuf &VertBuf::operator=(VertBuf &&other) noexcept {
		if (this == &other) return *this;

		m_Vbo = other.m_Vbo;
		m_Ebo = other.m_Ebo;
		m_IndexCount = other.m_IndexCount;
		other.m_Vbo = 0;
		other.m_Ebo = 0;
		other.m_IndexCount = 0;

		return *this;
	}

	VertBuf::~VertBuf() {
		glDeleteBuffers(1, &m_Vbo);
		glDeleteBuffers(1, &m_Ebo);
	}

	void VertBuf::Bind() const {
		glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ebo);
	}

	void VertBuf::Unbind() {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	unsigned int VertBuf::GetIndexCount() const {
		return m_IndexCount;
	}
} // namespace Engine::Graphics
