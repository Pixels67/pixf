#include "Shader.hpp"

#include "Gl.hpp"
#include "Math/Math.hpp"
#include "Memory/Buffer.hpp"

namespace Flock::Graphics {
    u32 ToGlType(const ShaderType shaderType) {
        switch (shaderType) {
            case ShaderType::VertexShader: return GL_VERTEX_SHADER;
            case ShaderType::FragmentShader: return GL_FRAGMENT_SHADER;
            default: return 0;
        }
    }

    Shader::Shader(Shader &&other) noexcept
        : m_Id(other.m_Id) {
        other.m_Id = 0;
    }

    Shader &Shader::operator=(Shader &&other) noexcept {
        if (this == &other) {
            return *this;
        }

        Clear();

        m_Id       = other.m_Id;
        other.m_Id = 0;

        return *this;
    }

    Shader::~Shader() {
        Clear();
    }

    std::optional<Shader> Shader::Create(const ShaderType type, const std::string &src) {
        Shader shader{};

        FLK_GL_CALL(shader.m_Id = glCreateShader(ToGlType(type)));
        const char *source = src.c_str();
        FLK_GL_CALL(glShaderSource(shader.m_Id, 1, &source, nullptr));
        FLK_GL_CALL(glCompileShader(shader.m_Id));

        int success = 0;
        FLK_GL_CALL(glGetShaderiv(shader.m_Id, GL_COMPILE_STATUS, &success));
        if (success != GL_TRUE) {
            constexpr u32 msgLength = 1024;

            GLsizei log_length = 0;
            char    message[msgLength];
            FLK_GL_CALL(glGetShaderInfoLog(shader.m_Id, msgLength, &log_length, message));

            if (type == VertexShader) {
                Debug::LogErr("Failed to compile vertex shader: {}", message);
            } else {
                Debug::LogErr("Failed to compile fragment shader: {}", message);
            }

            return std::nullopt;
        }

        return shader;
    }

    void Shader::Clear() const {
        if (m_Id == 0) {
            return;
        }

        FLK_GL_CALL(glDeleteProgram(m_Id));
    }

    u32 Shader::GetGlId() const {
        return m_Id;
    }
}
