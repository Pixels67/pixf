#include "shader.h"

#include <iostream>
#include <sstream>
#include <string>
#include <glad/glad.h>

#include "core/string_utils.h"
#include "file/file_io.h"

constexpr int POS_LAYOUT_INDEX = 0;
constexpr int UV_LAYOUT_INDEX  = 1;

constexpr auto DEFAULT_SHADER = R"(#version 330 core
@VERTEX
@pos in vec3 aPos;
@uv in vec2 aUv;
out vec2 uv;
void main()
{
   gl_Position = vec4(aPos, 1.0);
   uv = aUv;
}
@FRAGMENT
uniform sampler2D tex;
in vec2 uv;
out vec4 FragColor;
void main()
{
	FragColor = texture(tex, uv);
})";

namespace Engine::Graphics {
    Shader::Shader(const std::string &vertShaderSrc, const std::string &fragShaderSrc) {
        const unsigned int vertShader = CreateVertShader(vertShaderSrc);
        const unsigned int fragShader = CreateFragShader(fragShaderSrc);

        m_Id = glCreateProgram();

        glAttachShader(m_Id, vertShader);
        glAttachShader(m_Id, fragShader);

        glLinkProgram(m_Id);

        glDeleteShader(vertShader);
        glDeleteShader(fragShader);

        int success;
        glGetProgramiv(m_Id, GL_LINK_STATUS, &success);

        if (!success) {
            char infoLog[512];
            glGetProgramInfoLog(m_Id, 512, nullptr, infoLog);
            std::cerr << "Shader linking failed!\n" << infoLog << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    void Shader::Unbind() {
        glUseProgram(0);
    }

    Shader::~Shader() {
        glDeleteProgram(m_Id);
    }

    Shader Shader::CreateShader() {
        return CreateShader(DEFAULT_SHADER);
    }

    Shader Shader::CreateShader(const std::string &src) {
        auto [vertSrc, fragSrc] = ParseShader(src);
        return Shader(vertSrc, fragSrc);
    }

    void Shader::Bind() const {
        glUseProgram(m_Id);
    }

    void Shader::SetUniform(const std::string &name, const std::initializer_list<int> values) const {
        Bind();

        const auto it = values.begin();
        switch (values.size()) {
        case 1:
            glUniform1i(glGetUniformLocation(m_Id, name.c_str()), *it);
            break;
        case 2:
            glUniform2i(glGetUniformLocation(m_Id, name.c_str()), *it, *(it + 1));
            break;
        case 3:
            glUniform3i(glGetUniformLocation(m_Id, name.c_str()), *it, *(it + 1), *(it + 2));
            break;
        case 4:
            glUniform4i(glGetUniformLocation(m_Id, name.c_str()), *it, *(it + 1), *(it + 2), *(it + 3));
            break;
        default:
            break;
        }

        Unbind();
    }

    void Shader::SetUniform(const std::string &name, const std::initializer_list<unsigned int> values) const {
        Bind();

        const auto it = values.begin();
        switch (values.size()) {
        case 1:
            glUniform1ui(glGetUniformLocation(m_Id, name.c_str()), *it);
            break;
        case 2:
            glUniform2ui(glGetUniformLocation(m_Id, name.c_str()), *it, *(it + 1));
            break;
        case 3:
            glUniform3ui(glGetUniformLocation(m_Id, name.c_str()), *it, *(it + 1), *(it + 2));
            break;
        case 4:
            glUniform4ui(glGetUniformLocation(m_Id, name.c_str()), *it, *(it + 1), *(it + 2), *(it + 3));
            break;
        default:
            break;
        }

        Unbind();
    }

    void Shader::SetUniform(const std::string &name, const std::initializer_list<float> values) const {
        Bind();

        const auto it = values.begin();
        switch (values.size()) {
        case 1:
            glUniform1f(glGetUniformLocation(m_Id, name.c_str()), *it);
            break;
        case 2:
            glUniform2f(glGetUniformLocation(m_Id, name.c_str()), *it, *(it + 1));
            break;
        case 3:
            glUniform3f(glGetUniformLocation(m_Id, name.c_str()), *it, *(it + 1), *(it + 2));
            break;
        case 4:
            glUniform4f(glGetUniformLocation(m_Id, name.c_str()), *it, *(it + 1), *(it + 2), *(it + 3));
            break;
        default:
            break;
        }

        Unbind();
    }

    Shader Shader::LoadFromFile(const std::string &path) {
        const std::string str   = File::ReadFile(path);
        auto [vertSrc, fragSrc] = ParseShader(str);
        return Shader(vertSrc, fragSrc);
    }

    Shader::ShaderSources Shader::ParseShader(const std::string &source) {
        std::string str = source;
        str             = Core::ReplaceAll(str, "@pos", "layout (location = " + std::to_string(POS_LAYOUT_INDEX) + ")");
        str             = Core::ReplaceAll(str, "@uv", "layout (location = " + std::to_string(UV_LAYOUT_INDEX) + ")");

        ShaderSources sources;
        sources.vertSrc = "";
        sources.fragSrc = "";

        std::istringstream ss(str);
        std::string line;
        bool ignore = false;
        while (std::getline(ss, line)) {
            if (line.find("@FRAGMENT") != std::string::npos) {
                ignore = true;
                continue;
            }

            if (line.find("@VERTEX") != std::string::npos) {
                ignore = false;
                continue;
            }

            if (!ignore) sources.vertSrc += line + '\n';
        }

        ignore = false;
        ss     = std::istringstream(str);
        while (std::getline(ss, line)) {
            if (line.find("@FRAGMENT") != std::string::npos) {
                ignore = false;
                continue;
            }

            if (line.find("@VERTEX") != std::string::npos) {
                ignore = true;
                continue;
            }

            if (!ignore) sources.fragSrc += line + '\n';
        }

        return sources;
    }

    unsigned int Shader::CreateVertShader(const std::string &src) {
        const unsigned int shader = glCreateShader(GL_VERTEX_SHADER);
        const char *cStr          = src.c_str();
        glShaderSource(shader, 1, &cStr, nullptr);
        glCompileShader(shader);

        int success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

        if (!success) {
            char infoLog[512];
            glGetShaderInfoLog(shader, 512, nullptr, infoLog);
            std::cerr << "Vertex shader compilation failed!\n" << infoLog << std::endl;
            exit(EXIT_FAILURE);
        }

        return shader;
    }

    unsigned int Shader::CreateFragShader(const std::string &src) {
        const unsigned int shader = glCreateShader(GL_FRAGMENT_SHADER);
        const char *cStr          = src.c_str();
        glShaderSource(shader, 1, &cStr, nullptr);
        glCompileShader(shader);

        int success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

        if (!success) {
            char infoLog[512];
            glGetShaderInfoLog(shader, 512, nullptr, infoLog);
            std::cerr << "Fragment shader compilation failed!\n" << infoLog << std::endl;
            exit(EXIT_FAILURE);
        }

        return shader;
    }
} // namespace Engine::Graphics
