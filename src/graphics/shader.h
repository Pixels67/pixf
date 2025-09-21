#pragma once

namespace Engine {
	inline auto defaultVertShaderSrc =
			"#version 330 core\n"
			"layout (location = 0) in vec3 aPos;\n"
			"void main()\n"
			"{\n"
			"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
			"}\0";

	inline auto defaultFragShaderSrc =
			"#version 330 core\n"
			"out vec4 FragColor;\n"
			"void main()\n"
			"{\n"
			"	FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
			"}\0";

	class Shader {
	public:
		unsigned int Id;

		explicit Shader(const char *vertShaderSrc = defaultVertShaderSrc,
		                const char *fragShaderSrc = defaultFragShaderSrc);

		~Shader() = default;

		void Bind() const;

	private:
		static unsigned int CreateVertShader(const char *src);

		static unsigned int CreateFragShader(const char *src);
	};
} // namespace Engine
