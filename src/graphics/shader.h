#pragma once

#include <string>

namespace Engine::Graphics {
	class Shader {
		struct ShaderSources {
			std::string vertSrc;
			std::string fragSrc;
		};

	public:
		Shader(const Shader &) = delete;

		Shader &operator=(const Shader &) = delete;

		Shader(Shader &&) = delete;

		Shader &operator=(Shader &&) = delete;


		~Shader();


		static Shader CreateShader();

		static Shader CreateShader(const std::string &src);

		static Shader LoadFromFile(const std::string &path);


		void Bind() const;


		void SetUniform(const std::string &name, std::initializer_list<int> values) const;

		void SetUniform(const std::string &name, std::initializer_list<unsigned int> values) const;

		void SetUniform(const std::string &name, std::initializer_list<float> values) const;

	private:
		unsigned int m_Id = 0;


		explicit Shader(const std::string &vertShaderSrc, const std::string &fragShaderSrc);


		static void Unbind();

		static ShaderSources ParseShader(const std::string &source);

		static unsigned int CreateVertShader(const std::string &src);

		static unsigned int CreateFragShader(const std::string &src);
	};
} // namespace Engine::Graphics
