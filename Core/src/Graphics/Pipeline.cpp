#include "Pipeline.hpp"

#include <utility>

#include "Gl.hpp"
#include "Debug/Log.hpp"
#include "Graphics/CubeMap.hpp"
#include "Graphics/Shader.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/TextureArray.hpp"
#include "glad/glad.h"

namespace Flock::Graphics {
    std::optional<Pipeline> Pipeline::Create(const Shader &vertex, const Shader &fragment) {
        Pipeline pipeline{};

        pipeline.m_Id = LinkShaders(vertex, fragment);
        if (pipeline.m_Id == 0) {
            return std::nullopt;
        }

        pipeline.m_DefaultTexture = Texture::Default();
        pipeline.SetSamplerUnits();

        return pipeline;
    }

    Pipeline::~Pipeline() {
        Clear();
    }

    Pipeline::Pipeline(Pipeline &&other) noexcept {
        m_Id             = other.m_Id;
        m_Samplers       = std::move(other.m_Samplers);
        m_DefaultTexture = std::move(other.m_DefaultTexture);
        m_Uniforms       = std::move(other.m_Uniforms);
        other.m_Id       = 0;
    }

    Pipeline &Pipeline::operator=(Pipeline &&other) noexcept {
        if (this == &other) {
            return *this;
        }

        Clear();

        m_Id             = other.m_Id;
        m_Samplers       = std::move(other.m_Samplers);
        m_DefaultTexture = std::move(other.m_DefaultTexture);
        m_Uniforms       = std::move(other.m_Uniforms);
        other.m_Id       = 0;

        return *this;
    }

    void Pipeline::Clear() const {
        FLK_GL_CALL(glDeleteProgram(m_Id));
    }

    bool Pipeline::Bind() const {
        if (m_Id == 0) {
            return false;
        }

        FLK_GL_CALL(glUseProgram(m_Id));
        SetDefaultTextures();

        for (const auto &[name, uniform]: m_Uniforms) {
            if (!BindUniform(name, uniform)) {
                Debug::LogErr("Failed to bind uniform: {}", name);
            }
        }

        return true;
    }

    void Pipeline::Unbind() {
        FLK_GL_CALL(glUseProgram(0));
    }

    void Pipeline::SetUniform(const std::string &name, const u8 value) {
        m_Uniforms[name] = {.type = UniformType::U8, .data = value};
    }

    void Pipeline::SetUniform(const std::string &name, u32 value) {
        m_Uniforms[name] = {.type = UniformType::U32, .data = value};
    }

    void Pipeline::SetUniform(const std::string &name, i32 value) {
        m_Uniforms[name] = {.type = UniformType::I32, .data = value};
    }

    void Pipeline::SetUniform(const std::string &name, f32 value) {
        m_Uniforms[name] = {.type = UniformType::F32, .data = value};
    }

    void Pipeline::SetUniform(const std::string &name, Vector2f value) {
        m_Uniforms[name] = {.type = UniformType::Vec2, .data = value};
    }

    void Pipeline::SetUniform(const std::string &name, Vector3f value) {
        m_Uniforms[name] = {.type = UniformType::Vec3, .data = value};
    }

    void Pipeline::SetUniform(const std::string &name, Vector4f value) {
        m_Uniforms[name] = {.type = UniformType::Vec4, .data = value};
    }

    void Pipeline::SetUniform(const std::string &name, Color3u8 value) {
        m_Uniforms[name] = {.type = UniformType::Col3, .data = value};
    }

    void Pipeline::SetUniform(const std::string &name, Color4u8 value) {
        m_Uniforms[name] = {.type = UniformType::Col4, .data = value};
    }

    void Pipeline::SetUniform(const std::string &name, Matrix4f value) {
        m_Uniforms[name] = {.type = UniformType::Mat4, .data = value};
    }

    bool Pipeline::SetUniform(const std::string &name, const Texture &value) const {
        if (m_Id == 0 || !m_Samplers.contains(name)) {
            return false;
        }

        if (m_Samplers.at(name).glType != GL_SAMPLER_2D &&
            m_Samplers.at(name).glType != GL_SAMPLER_2D_SHADOW) {
            return false;
        }

        Texture::SetActiveUnit(m_Samplers.at(name).unit);
        return value.Bind();
    }

    bool Pipeline::SetUniform(const std::string &name, const CubeMap &value) const {
        if (m_Id == 0 || !m_Samplers.contains(name)) {
            return false;
        }

        if (m_Samplers.at(name).glType != GL_SAMPLER_CUBE &&
            m_Samplers.at(name).glType != GL_SAMPLER_CUBE_SHADOW) {
            return false;
            }

        Texture::SetActiveUnit(m_Samplers.at(name).unit);
        return value.Bind();
    }

    bool Pipeline::SetUniform(const std::string &name, const TextureArray &value) const {
        if (m_Id == 0 || !m_Samplers.contains(name)) {
            return false;
        }

        if (m_Samplers.at(name).glType != GL_SAMPLER_2D_ARRAY &&
            m_Samplers.at(name).glType != GL_SAMPLER_2D_ARRAY_SHADOW) {
            return false;
        }

        Texture::SetActiveUnit(m_Samplers.at(name).unit);
        return value.Bind();
    }

    void Pipeline::ResetUniforms() {
        m_Uniforms.clear();
        SetDefaultTextures(true);
    }

    u32 Pipeline::LinkShaders(const Shader &vertex, const Shader &fragment) {
        unsigned int program = 0;

        FLK_GL_CALL(program = glCreateProgram());
        FLK_GL_CALL(glAttachShader(program, vertex.GlId()));
        FLK_GL_CALL(glAttachShader(program, fragment.GlId()));
        FLK_GL_CALL(glLinkProgram(program));

        int success = 0;
        FLK_GL_CALL(glGetProgramiv(program, GL_LINK_STATUS, &success));
        if (success != GL_TRUE) {
            constexpr unsigned int msgLength = 1024;

            GLsizei log_length = 0;
            char    message[msgLength];
            FLK_GL_CALL(glGetProgramInfoLog(program, msgLength, &log_length, message));

            Debug::LogErr("Failed to link shaders: {}", message);

            return 0;
        }

        return program;
    }

    bool Pipeline::SetSamplerUnits() {
        if (m_Id == 0) {
            return false;
        }

        i32 boundPipeline = 0;
        FLK_GL_CALL(glGetIntegerv(GL_CURRENT_PROGRAM, &boundPipeline));

        FLK_GL_CALL(glUseProgram(m_Id));

        i32 uniformCount;
        FLK_GL_CALL(glGetProgramiv(m_Id, GL_ACTIVE_UNIFORMS, &uniformCount));

        i32 unitCounter = 0;
        for (i32 i = 0; i < uniformCount; i++) {
            char    name[256];
            GLsizei length;
            i32     size;
            GLenum  type;
            FLK_GL_CALL(glGetActiveUniform(m_Id, i, sizeof(name), &length, &size, &type, name));

            if (type == GL_SAMPLER_2D || type == GL_SAMPLER_2D_SHADOW || type == GL_SAMPLER_2D_ARRAY || type ==
                GL_SAMPLER_2D_ARRAY_SHADOW || type == GL_SAMPLER_CUBE || type == GL_SAMPLER_CUBE_SHADOW) {
                std::string uniformName = name;

                if (uniformName.ends_with("[0]")) {
                    uniformName = uniformName.substr(0, uniformName.size() - 3);
                }

                for (i32 j = 0; j < size; j++) {
                    std::string elementName = size > 1
                                                  ? uniformName + "[" + std::to_string(j) + "]"
                                                  : uniformName;

                    const i32 unit          = unitCounter++;
                    m_Samplers[elementName] = {.unit = unit, .glType = type};

                    i32 location = glGetUniformLocation(m_Id, elementName.c_str());
                    FLK_GL_CALL(glUniform1i(location, unit));
                }
            }
        }

        FLK_GL_CALL(glUseProgram(boundPipeline));

        return true;
    }

    bool Pipeline::BindUniform(const std::string &name, const Uniform &uniform) const {
        if (m_Id == 0) {
            return false;
        }

        switch (const auto [type, data] = uniform; type) {
            case UniformType::U8:
                FLK_GL_CALL(glUniform1ui(glGetUniformLocation(m_Id, name.c_str()), std::any_cast<u8>(data)));
                break;
            case UniformType::U32:
                FLK_GL_CALL(glUniform1ui(glGetUniformLocation(m_Id, name.c_str()), std::any_cast<u32>(data)));
                break;
            case UniformType::I32:
                FLK_GL_CALL(glUniform1i(glGetUniformLocation(m_Id, name.c_str()), std::any_cast<i32>(data)));
                break;
            case UniformType::F32:
                FLK_GL_CALL(glUniform1f(glGetUniformLocation(m_Id, name.c_str()), std::any_cast<f32>(data)));
                break;
            case UniformType::Vec2: {
                const auto vec = std::any_cast<Vector2f>(data);
                FLK_GL_CALL(glUniform2f(glGetUniformLocation(m_Id, name.c_str()), vec.x, vec.y));
                break;
            }
            case UniformType::Vec3: {
                const auto vec = std::any_cast<Vector3f>(data);
                FLK_GL_CALL(glUniform3f(glGetUniformLocation(m_Id, name.c_str()), vec.x, vec.y, vec.z));
                break;
            }
            case UniformType::Vec4: {
                const auto vec = std::any_cast<Vector4f>(data);
                FLK_GL_CALL(glUniform4f(glGetUniformLocation(m_Id, name.c_str()), vec.x, vec.y, vec.z, vec.w));
                break;
            }
            case UniformType::Col3: {
                const auto vec = std::any_cast<Color3u8>(data).ToVector();
                FLK_GL_CALL(glUniform3f(glGetUniformLocation(m_Id, name.c_str()), vec.x, vec.y, vec.z));
                break;
            }
            case UniformType::Col4: {
                const auto vec = std::any_cast<Color4u8>(data).ToVector();
                FLK_GL_CALL(glUniform4f(glGetUniformLocation(m_Id, name.c_str()), vec.x, vec.y, vec.z, vec.w));
                break;
            }
            case UniformType::Mat4: {
                const auto mat = std::any_cast<Matrix4f>(data);
                FLK_GL_CALL(glUniformMatrix4fv(glGetUniformLocation(m_Id, name.c_str()), 1, true, mat.Data()));
                break;
            }
            default:
                return false;
        }

        return true;
    }

    void Pipeline::SetDefaultTextures(const bool overwrite) const {
        i32 activeUnit;
        FLK_GL_CALL(glGetIntegerv(GL_ACTIVE_TEXTURE, &activeUnit));

        for (const auto &[name, info]: m_Samplers) {
            if (info.glType != GL_SAMPLER_2D) {
                continue;
            }

            i32 id = 0;
            FLK_GL_CALL(glActiveTexture(GL_TEXTURE0 + info.unit));
            FLK_GL_CALL(glGetIntegerv(GL_TEXTURE_BINDING_2D, &id));

            if (id != 0 && !overwrite) {
                continue;
            }

            SetUniform(name, m_DefaultTexture);
        }

        FLK_GL_CALL(glActiveTexture(activeUnit));
    }
}
