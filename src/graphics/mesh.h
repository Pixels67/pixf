#pragma once

#include <vector>

#include "camera.h"
#include "material.h"
#include "shader.h"
#include "texture.h"
#include "vert_arr.h"
#include "core/transform.h"

struct Vertex {
	glm::vec3 position;
	glm::vec2 texCoords;
};

namespace Engine::Graphics {
	class Mesh {
	public:
		Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices);


		Mesh(Mesh &other) = delete;

		Mesh &operator=(Mesh &other) = delete;

		Mesh(Mesh &&other) = delete;

		Mesh &operator=(Mesh &&other) = delete;


		void Render(const Shader &shader, const Core::Transform &transform, const Camera &camera) const;

		void Render(const Material &material, const Core::Transform &transform, const Camera &camera) const;

	private:
		VertArr m_VertArr;
		VertBuf m_VertBuf;
	};
} // namespace Engine::Graphics