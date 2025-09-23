#include "vert_arr.h"

#include <glad/glad.h>

#include "vert_buf.h"

namespace Engine::Graphics {
	VertBufLayout::VertBufLayout(const unsigned int capacity) {
		m_Elements.reserve(capacity);
	}

	unsigned int VertBufLayout::GetSize() const {
		return static_cast<unsigned int>(m_Elements.size());
	}

	unsigned int VertBufLayout::GetStride() const {
		return m_Stride;
	}

	const VertBufElement &VertBufLayout::operator[](const unsigned int i) const {
		return m_Elements[i];
	}

	VertArr::VertArr(const VertBuf &buffer, const VertBufLayout &layout) {
		glGenVertexArrays(1, &m_Id);
		glBindVertexArray(m_Id);

		buffer.Bind();
		unsigned int offset = 0;
		for (int i = 0; i < layout.GetSize(); i++) {
			glVertexAttribPointer(i,
			                      static_cast<GLint>(layout[i].size),
			                      layout[i].type,
			                      layout[i].normalized,
			                      static_cast<GLsizei>(layout.GetStride()),
			                      reinterpret_cast<void *>(offset));

			glEnableVertexAttribArray(i);

			offset += layout[i].count * layout[i].size;
		}

		glBindVertexArray(0);
		VertBuf::Unbind();
	}

	VertArr::VertArr(VertArr &&other) noexcept {
		m_Id = other.m_Id;
		other.m_Id = 0;
	}

	VertArr &VertArr::operator=(VertArr &&other) noexcept {
		if (this == &other) return *this;

		m_Id = other.m_Id;
		other.m_Id = 0;

		return *this;
	}

	VertArr::~VertArr() {
		glDeleteVertexArrays(1, &m_Id);
	}

	void VertArr::Bind() const {
		glBindVertexArray(m_Id);
	}

	void VertArr::Unbind() {
		glBindVertexArray(0);
	}

	template<>
	void VertBufLayout::PushBack<float>(const unsigned int count) {
		m_Elements.push_back({GL_FLOAT, count, sizeof(float), false});
		m_Stride += count * sizeof(float);
	}

	template<>
	void VertBufLayout::PushBack<int>(const unsigned int count) {
		m_Elements.push_back({GL_INT, count, sizeof(int), false});
		m_Stride += count * sizeof(int);
	}

	template<>
	void VertBufLayout::PushBack<unsigned int>(const unsigned int count) {
		m_Elements.push_back({GL_UNSIGNED_INT, sizeof(int), count, false});
		m_Stride += count * sizeof(int);
	}

	template<>
	void VertBufLayout::PushBack<char>(const unsigned int count) {
		m_Elements.push_back({GL_BYTE, count, sizeof(char), true});
		m_Stride += count * sizeof(char);
	}

	template<>
	void VertBufLayout::PushBack<unsigned char>(const unsigned int count) {
		m_Elements.push_back({GL_UNSIGNED_BYTE, sizeof(char), count, true});
		m_Stride += count * sizeof(char);
	}
} // namespace Engine::Graphics
