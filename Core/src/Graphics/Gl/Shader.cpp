#include "Shader.hpp"

#include <string>

#include "../../Common.hpp"
#include "Gl.hpp"

namespace Pixf::Core::Graphics::Gl {
    Shader::Shader(const std::string &vertSrc, const std::string &fragSrc) {
        const unsigned int vertShader = CreateGlShader(GL_VERTEX_SHADER, vertSrc);
        const unsigned int fragShader = CreateGlShader(GL_FRAGMENT_SHADER, fragSrc);
        const unsigned int program = LinkGlShaders(vertShader, fragShader);

        GL_CALL(glDeleteShader(vertShader));
        GL_CALL(glDeleteShader(fragShader));

        m_Id = program;

        InitTextureMap();
    }

    Shader::Shader(Shader &&other) noexcept : m_Id(other.m_Id), m_TextureMap(other.m_TextureMap) { other.m_Id = 0; }

    Shader &Shader::operator=(Shader &&other) noexcept {
        if (this == &other) {
            return *this;
        }

        Cleanup();

        m_Id = other.m_Id;
        m_TextureMap = other.m_TextureMap;
        other.m_Id = 0;

        return *this;
    }

    Shader::~Shader() { Cleanup(); }

    void Shader::Bind() const { GL_CALL(glUseProgram(m_Id)); }

    void Shader::Unbind() { GL_CALL(glUseProgram(0)); }

    void Shader::SetUniform(const std::string &name, const char value) const {
        GL_CALL(glUniform1i(glGetUniformLocation(m_Id, name.c_str()), value));
    }

    void Shader::SetUniform(const std::string &name, const int value) const {
        GL_CALL(glUniform1i(glGetUniformLocation(m_Id, name.c_str()), value));
    }

    void Shader::SetUniform(const std::string &name, const float value) const {
        GL_CALL(glUniform1f(glGetUniformLocation(m_Id, name.c_str()), value));
    }

    void Shader::SetUniform(const std::string &name, const vec2 value) const {
        GL_CALL(glUniform2f(glGetUniformLocation(m_Id, name.c_str()), value.x, value.y));
    }

    void Shader::SetUniform(const std::string &name, const vec3 value) const {
        GL_CALL(glUniform3f(glGetUniformLocation(m_Id, name.c_str()), value.x, value.y, value.z));
    }

    void Shader::SetUniform(const std::string &name, const vec4 value) const {
        GL_CALL(glUniform4f(glGetUniformLocation(m_Id, name.c_str()), value.x, value.y, value.z, value.w));
    }

    void Shader::SetUniform(const std::string &name, mat4 value) const {
        GL_CALL(glUniformMatrix4fv(glGetUniformLocation(m_Id, name.c_str()), 1, GL_FALSE, value_ptr(value)));
    }

    std::unordered_map<std::string, uint8_t> Shader::GetTextureMap() const { return m_TextureMap; }

    unsigned int Shader::CreateGlShader(const unsigned int type, const std::string &src) {
        const unsigned int shader = glCreateShader(type);
        const char *source = src.c_str();
        GL_CALL(glShaderSource(shader, 1, &source, nullptr));
        GL_CALL(glCompileShader(shader));

        int success = 0;
        GL_CALL(glGetShaderiv(shader, GL_COMPILE_STATUS, &success));
        if (success != GL_TRUE) {
            constexpr unsigned int msgLength = 1024;

            GLsizei log_length = 0;
            char message[msgLength];
            GL_CALL(glGetShaderInfoLog(shader, msgLength, &log_length, message));

            // TODO: Better error handling
            ASSERT(false, message);
        }

        return shader;
    }

    unsigned int Shader::LinkGlShaders(const unsigned int vertShader, const unsigned int fragShader) {
        unsigned int program = 0;
        GL_CALL(program = glCreateProgram());
        GL_CALL(glAttachShader(program, vertShader));
        GL_CALL(glAttachShader(program, fragShader));
        GL_CALL(glLinkProgram(program));

        int success = 0;
        GL_CALL(glGetProgramiv(program, GL_LINK_STATUS, &success));
        if (success != GL_TRUE) {
            constexpr unsigned int msgLength = 1024;

            GLsizei log_length = 0;
            char message[msgLength];
            GL_CALL(glGetProgramInfoLog(program, msgLength, &log_length, message));

            // TODO: Better error handling
            ASSERT(false, message);
        }

        return program;
    }

    void Shader::Cleanup() const {
        if (m_Id == 0) {
            return;
        }

        int currentShader = 0;
        GL_CALL(glGetIntegerv(GL_CURRENT_PROGRAM, &currentShader));

        if (currentShader == m_Id) {
            Unbind();
        }

        GL_CALL(glDeleteShader(m_Id));
    }

    void Shader::InitTextureMap() {
        GLint numUniforms = 0;
        GL_CALL(glGetProgramiv(m_Id, GL_ACTIVE_UNIFORMS, &numUniforms));

        uint8_t textureUnit = 0;

        for (GLint i = 0; i < numUniforms; i++) {
            GLsizei length = 0;
            int size = 0;
            GLenum type = 0;
            char name[maxTextureNameLength];

            GL_CALL(glGetActiveUniform(m_Id, i, sizeof(name), &length, &size, &type, name));

            int currentShader = 0;
            GL_CALL(glGetIntegerv(GL_CURRENT_PROGRAM, &currentShader));

            Bind();

            if (type == GL_SAMPLER_2D || type == GL_SAMPLER_CUBE || type == GL_SAMPLER_2D_ARRAY) {
                m_TextureMap[name] = textureUnit;
                SetUniform(name, textureUnit);
                textureUnit++;
                if (textureUnit == maxTextureCount) {
                    return;
                }
            }

            GL_CALL(glUseProgram(currentShader));
        }
    }
} // namespace Pixf::Core::Graphics::Gl
