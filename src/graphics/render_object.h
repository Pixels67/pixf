#pragma once

#include <vector>

#include "shader.h"
#include "vert_arr.h"

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
		VertArr m_VertArr;
		VertBuf m_VertBuf;
	};
} // namespace Engine::Graphics