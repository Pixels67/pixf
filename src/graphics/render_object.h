#pragma once

#include <vector>

#include "graphics.h"

namespace Engine::Graphics {
	class RenderObject {
	public:
		RenderObject(const std::vector<float> &vertices, const std::vector<unsigned int> &indices);


		RenderObject(RenderObject &other) = delete;

		RenderObject &operator=(RenderObject &other) = delete;

		RenderObject(RenderObject &&other) = delete;

		RenderObject &operator=(RenderObject &&other) = delete;


		void Render(const Shader &shader) const;

	private:
		VertArr m_vertArr;
		VertBuf m_vertBuf;
	};
} // namespace Engine::Graphics