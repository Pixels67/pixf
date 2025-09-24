#pragma once

#include <string>

#include <matrix.hpp>

namespace Engine::Graphics {
	class Shader {
		struct ShaderSources {
			std::string vertSrc;
			std::string fragSrc;
		};

	public:
		Shader(const Shader &other);

		Shader &operator=(const Shader &other);

		Shader(Shader &&other);

		Shader &operator=(Shader &&other);


		~Shader();


		static Shader CreateShader();

		static Shader CreateShader(const std::string &src);

		static Shader LoadFromFile(const std::string &path);


		void Bind() const;

		static void Unbind();


		void SetUniform(const std::string &name, std::initializer_list<int> values) const;

		void SetUniform(const std::string &name, std::initializer_list<unsigned int> values) const;

		void SetUniform(const std::string &name, std::initializer_list<float> values) const;

		void SetUniform(const std::string &name, glm::mat4 matrix) const;

	private:
		unsigned int m_Id = 0;
		std::string m_VertShaderSrc = "";
		std::string m_FragShaderSrc = "";


		explicit Shader(const std::string &vertShaderSrc, const std::string &fragShaderSrc);


		static ShaderSources ParseShader(const std::string &source);

		static unsigned int CreateVertShader(const std::string &src);

		static unsigned int CreateFragShader(const std::string &src);
	};
} // namespace Engine::Graphics
