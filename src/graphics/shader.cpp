#include "shader.h"

#include <glad/glad.h>
#include <iostream>

namespace Engine::Graphics {
	Shader::Shader(const char *vertShaderSrc, const char *fragShaderSrc) {
		const unsigned int vertShader = CreateVertShader(vertShaderSrc);
		const unsigned int fragShader = CreateFragShader(fragShaderSrc);

		Id = glCreateProgram();

		glAttachShader(Id, vertShader);
		glAttachShader(Id, fragShader);

		glLinkProgram(Id);

		glDeleteShader(vertShader);
		glDeleteShader(fragShader);

		int success;
		char infoLog[512];
		glGetProgramiv(Id, GL_LINK_STATUS, &success);

		if(!success) {
			glGetProgramInfoLog(Id, 512, nullptr, infoLog);
			std::cerr << "Shader linking failed!\n" << infoLog << std::endl;
			exit(EXIT_FAILURE);
		}
	}

	void Shader::Bind() const {
		glUseProgram(Id);
	}

	unsigned int Shader::CreateVertShader(const char *src) {
		const unsigned int shader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(shader, 1, &src, nullptr);
		glCompileShader(shader);

		int success;
		char infoLog[512];
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

		if (!success) {
			glGetShaderInfoLog(shader, 512, nullptr, infoLog);
			std::cerr << "Vertex shader compilation failed!\n" << infoLog << std::endl;
			exit(EXIT_FAILURE);
		}

		return shader;
	}

	unsigned int Shader::CreateFragShader(const char *src) {
		const unsigned int shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(shader, 1, &src, nullptr);
		glCompileShader(shader);

		int success;
		char infoLog[512];
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

		if (!success) {
			glGetShaderInfoLog(shader, 512, nullptr, infoLog);
			std::cerr << "Fragment shader compilation failed!\n" << infoLog << std::endl;
			exit(EXIT_FAILURE);
		}

		return shader;
	}
} // namespace Engine::Graphics

