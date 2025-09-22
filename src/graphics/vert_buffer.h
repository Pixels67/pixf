#pragma once

#include <vector>

namespace Engine::Graphics {
	class VertBuffer {
	public:
		VertBuffer(const std::vector<float> &vertices, const std::vector<unsigned int> &indices, unsigned int usage);

		VertBuffer(const VertBuffer &other) = delete;
		VertBuffer &operator=(const VertBuffer &other) = delete;

		VertBuffer(VertBuffer &&other) = delete;
		VertBuffer &operator=(VertBuffer &&other) = delete;

		~VertBuffer() = default;

		void Bind() const;
		static void Unbind() ;

	private:
		unsigned int m_Vbo = 0;
		unsigned int m_Ebo = 0;
	};
} // namespace Engine::Graphics