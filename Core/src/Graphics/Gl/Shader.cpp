#include "Shader.hpp"

#include "Gl.hpp"

namespace Pixf::Core::Graphics::Gl {
    static constexpr unsigned int g_MaxTextureCount = 16;
    static constexpr unsigned int s_MaxTextureNameLength = 256;

    Shader::Shader(const unsigned int id) : m_Id(id) { InitTextureMap(); }

    Shader Shader::Create(const std::string &vertSrc, const std::string &fragSrc) {
        const unsigned int vertShader = CreateGlShader(GL_VERTEX_SHADER, vertSrc);
        const unsigned int fragShader = CreateGlShader(GL_FRAGMENT_SHADER, fragSrc);
        const unsigned int program = LinkGlShaders(vertShader, fragShader);

        PIXF_GL_CALL(glDeleteShader(vertShader));
        PIXF_GL_CALL(glDeleteShader(fragShader));

        PIXF_CORE_LOG_DEBUG("Created shader with ID: {}", program);

        return Shader(program);
    }

    Shader::Shader(Shader &&other) noexcept : m_Id(other.m_Id), m_TextureUniformMap(other.m_TextureUniformMap) { other.m_Id = 0; }

    Shader &Shader::operator=(Shader &&other) noexcept {
        if (this == &other) {
            return *this;
        }

        Cleanup();

        m_Id = other.m_Id;
        m_TextureUniformMap = other.m_TextureUniformMap;
        other.m_Id = 0;

        return *this;
    }

    Shader::~Shader() {
        Cleanup();
        PIXF_CORE_LOG_DEBUG("Cleaned up shader with ID: {}", m_Id);
    }

    void Shader::Bind() const { PIXF_GL_CALL(glUseProgram(m_Id)); }

    void Shader::Unbind() { PIXF_GL_CALL(glUseProgram(0)); }

    void Shader::SetUniform(const std::string &name, const char value) const {
        PIXF_GL_CALL(glUniform1i(glGetUniformLocation(m_Id, name.c_str()), value));
    }

    void Shader::SetUniform(const std::string &name, const int value) const {
        PIXF_GL_CALL(glUniform1i(glGetUniformLocation(m_Id, name.c_str()), value));
    }

    void Shader::SetUniform(const std::string &name, const float value) const {
        PIXF_GL_CALL(glUniform1f(glGetUniformLocation(m_Id, name.c_str()), value));
    }

    void Shader::SetUniform(const std::string &name, const Math::Vector2f &value) const {
        PIXF_GL_CALL(glUniform2f(glGetUniformLocation(m_Id, name.c_str()), value.x, value.y));
    }

    void Shader::SetUniform(const std::string &name, const Math::Vector3f &value) const {
        PIXF_GL_CALL(glUniform3f(glGetUniformLocation(m_Id, name.c_str()), value.x, value.y, value.z));
    }

    void Shader::SetUniform(const std::string &name, const Math::Vector4f &value) const {
        PIXF_GL_CALL(glUniform4f(glGetUniformLocation(m_Id, name.c_str()), value.x, value.y, value.z, value.w));
    }

    void Shader::SetUniform(const std::string &name, const Math::Color3u8 value) const {
        const Math::Vector3f vec(static_cast<float>(value.r) / 255.0F, static_cast<float>(value.g) / 255.0F,
                                 static_cast<float>(value.b) / 255.0F);
        SetUniform(name, vec);
    }

    void Shader::SetUniform(const std::string &name, const Math::Color4u8 value) const {
        const Math::Vector4f vec(static_cast<float>(value.r) / 255.0F, static_cast<float>(value.g) / 255.0F,
                                 static_cast<float>(value.b) / 255.0F, static_cast<float>(value.a) / 255.0F);
        SetUniform(name, vec);
    }

    void Shader::SetUniform(const std::string &name, const Math::Matrix4f &value) const {
        PIXF_GL_CALL(glUniformMatrix4fv(glGetUniformLocation(m_Id, name.c_str()), 1, GL_FALSE, value.Data()));
    }

    std::unordered_map<std::string, uint8_t> Shader::GetTextureUniformMap() const { return m_TextureUniformMap; }

    unsigned int Shader::CreateGlShader(const unsigned int type, const std::string &src) {
        const unsigned int shader = glCreateShader(type);
        const char *source = src.c_str();
        PIXF_GL_CALL(glShaderSource(shader, 1, &source, nullptr));
        PIXF_GL_CALL(glCompileShader(shader));

        int success = 0;
        PIXF_GL_CALL(glGetShaderiv(shader, GL_COMPILE_STATUS, &success));
        if (success != GL_TRUE) {
            constexpr unsigned int msgLength = 1024;

            GLsizei log_length = 0;
            char message[msgLength];
            PIXF_GL_CALL(glGetShaderInfoLog(shader, msgLength, &log_length, message));

            if (type == GL_VERTEX_SHADER) {
                throw ShaderCompilationError("Failed to compile vertex shader: " + std::string(message));
            }

            throw ShaderCompilationError("Failed to compile fragment shader: " + std::string(message));
        }

        return shader;
    }

    unsigned int Shader::LinkGlShaders(const unsigned int vertShader, const unsigned int fragShader) {
        unsigned int program = 0;
        PIXF_GL_CALL(program = glCreateProgram());
        PIXF_GL_CALL(glAttachShader(program, vertShader));
        PIXF_GL_CALL(glAttachShader(program, fragShader));
        PIXF_GL_CALL(glLinkProgram(program));

        int success = 0;
        PIXF_GL_CALL(glGetProgramiv(program, GL_LINK_STATUS, &success));
        if (success != GL_TRUE) {
            constexpr unsigned int msgLength = 1024;

            GLsizei log_length = 0;
            char message[msgLength];
            PIXF_GL_CALL(glGetProgramInfoLog(program, msgLength, &log_length, message));

            throw ShaderCompilationError("Failed to link shader: " + std::string(message));
        }

        return program;
    }

    void Shader::Cleanup() const {
        if (m_Id == 0) {
            return;
        }

        int currentShader = 0;
        PIXF_GL_CALL(glGetIntegerv(GL_CURRENT_PROGRAM, &currentShader));

        if (currentShader == static_cast<int>(m_Id)) {
            Unbind();
        }

        PIXF_GL_CALL(glDeleteShader(m_Id));
    }

    void Shader::InitTextureMap() {
        GLint numUniforms = 0;
        PIXF_GL_CALL(glGetProgramiv(m_Id, GL_ACTIVE_UNIFORMS, &numUniforms));

        uint8_t textureUnit = 0;

        for (GLint i = 0; i < numUniforms; i++) {
            GLsizei length = 0;
            int size = 0;
            GLenum type = 0;
            char name[s_MaxTextureNameLength];

            PIXF_GL_CALL(glGetActiveUniform(m_Id, i, sizeof(name), &length, &size, &type, name));

            int currentShader = 0;
            PIXF_GL_CALL(glGetIntegerv(GL_CURRENT_PROGRAM, &currentShader));

            Bind();

            if (type == GL_SAMPLER_2D || type == GL_SAMPLER_CUBE || type == GL_SAMPLER_2D_ARRAY) {
                m_TextureUniformMap[name] = textureUnit;
                SetUniform(name, textureUnit);
                textureUnit++;
                if (textureUnit == g_MaxTextureCount) {
                    return;
                }
            }

            PIXF_GL_CALL(glUseProgram(currentShader));
        }
    }
} // namespace Pixf::Core::Graphics::Gl
