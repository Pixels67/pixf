#include "Renderer.hpp"

#include <algorithm>
#include <string>

#include "Debug/Log.hpp"
#include "Graphics/Camera.hpp"
#include "Graphics/Framebuffer.hpp"
#include "Graphics/Gl.hpp"
#include "Graphics/Light.hpp"
#include "Graphics/Mesh.hpp"
#include "Graphics/Pipeline.hpp"
#include "Graphics/Shader.hpp"
#include "Graphics/Texture.hpp"
#include "Math/Quaternion.hpp"
#include "Math/RigidTransform.hpp"
#include "glad/glad.h"

namespace Flock {
    namespace Graphics {
        class CubeMap;
    } // namespace Graphics
}     // namespace Flock

namespace Flock::Graphics {
    static constexpr usize s_MaxLightsPerObject = 16;

    static constexpr auto s_ShadowVertShader = R"(
#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

void main() {
    gl_Position = vec4(aPosition, 1.0) * uModel * uView * uProj;
}
)";

    static constexpr auto s_ShadowFragShader = R"(
#version 330 core

out vec4 FragColor;

uniform vec3 uColor;

void main() {
    FragColor = vec4(uColor, 1.0);
}
)";

    static constexpr auto s_SkyboxVertShader = R"(
#version 330 core

layout(location = 0) in vec3 aPosition;

out vec3 vTexCoords;

uniform mat4 uView;
uniform mat4 uProj;

void main() {
    gl_Position = vec4(aPosition, 1.0) * uView * uProj;
    vTexCoords = -aPosition;
}
)";

    static constexpr auto s_SkyboxFragShader = R"(
#version 330 core

in vec3 vTexCoords;

out vec4 FragColor;

uniform samplerCube uSkybox;

void main() {
    FragColor = vec4(texture(uSkybox, vTexCoords));
}
)";

    i32 ToGlType(const DepthFunc depthFunc) {
        switch (depthFunc) {
            case DepthFunc::Always:
                return GL_ALWAYS;
            case DepthFunc::Never:
                return GL_NEVER;
            case DepthFunc::Equal:
                return GL_EQUAL;
            case DepthFunc::NEqual:
                return GL_NOTEQUAL;
            case DepthFunc::Less:
                return GL_LESS;
            case DepthFunc::Greater:
                return GL_GREATER;
            case DepthFunc::LEqual:
                return GL_LEQUAL;
            case DepthFunc::GEqual:
                return GL_GEQUAL;
            default:
                return 0;
        }
    }

    i32 ToGlType(const CullMode faceCullMode) {
        switch (faceCullMode) {
            case CullMode::None: return GL_NONE;
            case CullMode::Back: return GL_BACK;
            case CullMode::Front: return GL_FRONT;
            default: FLK_ASSERT(false);
        }
    }

    Renderer &Renderer::Render(const RenderList &commands, const SceneData &scene, RenderConfig config, const ShadowConfig &shadowConfig) {
        const auto     lights       = NearestLights(scene.lights, scene.camera.transform.position, s_MaxLightsPerObject);
        const Vector3f shadowCenter = scene.camera.transform.position;

        RenderList sortedCmds = commands;
        std::sort(sortedCmds.begin(), sortedCmds.end(), [&](RenderCommand &lhs, RenderCommand &rhs) {
            const f32 lhsZ = ((lhs.transform.position - scene.camera.transform.position) * scene.camera.transform.rotation.Inverse()).z;
            const f32 rhsZ = ((rhs.transform.position - scene.camera.transform.position) * scene.camera.transform.rotation.Inverse()).z;

            return lhsZ > rhsZ;
        });

        ShadowData shadowData;
        if (shadowConfig.enabled) {
            shadowData = GenerateShadowMaps(sortedCmds, lights, shadowConfig, shadowCenter);
        }

        auto      [origin, aspect] = config.viewport;
        const f32 aspectRatio      = static_cast<f32>(aspect.x - origin.x) / static_cast<f32>(aspect.y - origin.y);

        SetFramebuffer(config.framebuffer);
        ConfigureFramebuffer(config);

        if (scene.skybox) {
            RenderSkybox(
                *scene.skybox,
                scene.camera.transform.rotation.Inverse().ToMatrix(),
                scene.camera.ProjMatrix(aspectRatio)
            );
        }

        config.clear.clearColor = false;
        ConfigureFramebuffer(config);

        for (auto &cmd: sortedCmds) {
            auto &[mesh, pipeline, mat, trans] = cmd;

            if (!mesh || !pipeline) {
                continue;
            }

            pipeline->ResetUniforms();

            SetMatrices(*pipeline, trans, scene.camera, aspectRatio);

            pipeline->SetUniform("uCameraPosition", scene.camera.transform.position);
            pipeline->SetUniform("uAmbientColor", scene.ambientLight.color);
            pipeline->SetUniform("uAmbientIntensity", scene.ambientLight.intensity);

            SetMaterialUniforms(*pipeline, mat);
            SetLightUniforms(*pipeline, lights, shadowConfig);

            if (shadowData.shadowMaps.LayerCount() > 0) {
                if (!pipeline->SetUniform("uShadowMaps", shadowData.shadowMaps)) {
                    Debug::LogErr("Render: Failed to upload shadow maps!");
                    return *this;
                }

                for (usize i = 0; i < shadowData.spaceMatrices.size(); i++) {
                    std::string idx = "[" + std::to_string(i) + "]";
                    pipeline->SetUniform("uLightSpaceMatrices" + idx, shadowData.spaceMatrices[i]);
                }

                pipeline->SetUniform("uShadowCascadeCount", static_cast<i32>(shadowConfig.cascadeRanges.size()));
                for (usize i = 0; i < shadowConfig.cascadeRanges.size(); i++) {
                    std::string idx = "[" + std::to_string(i) + "]";
                    pipeline->SetUniform("uShadowCascadeRanges" + idx, shadowConfig.cascadeRanges[i]);
                }
            }

            if (scene.skybox) {
                pipeline->SetUniform("uSkybox", *scene.skybox);
            }

            RenderMesh(*mesh, *pipeline);
        }

        Framebuffer::Unbind();
        Mesh::Unbind();
        Pipeline::Unbind();

        return *this;
    }

    bool Renderer::SetFramebuffer(const Framebuffer *framebuffer) {
        if (framebuffer) {
            if (!framebuffer->Bind()) {
                Debug::LogErr("Renderer::SetFramebuffer: Unable to bind framebuffer!");
                return false;
            }
        } else {
            Framebuffer::Unbind();
        }

        return true;
    }

    void Renderer::ConfigureFramebuffer(RenderConfig config) {
        auto [origin, aspect] = config.viewport;

        FLK_GL_CALL(glFrontFace(GL_CCW));
        FLK_GL_CALL(glViewport(origin.x, origin.y, aspect.x, aspect.y));

        if (config.blend.enabled) {
            FLK_GL_CALL(glEnable(GL_BLEND));
            FLK_GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        } else {
            FLK_GL_CALL(glDisable(GL_BLEND));
        }

        if (config.raster.cullMode != CullMode::None) {
            FLK_GL_CALL(glEnable(GL_CULL_FACE));
            FLK_GL_CALL(glCullFace(ToGlType(config.raster.cullMode)));
        } else {
            FLK_GL_CALL(glDisable(GL_CULL_FACE));
        }

        if (config.depth.enabled) {
            FLK_GL_CALL(glEnable(GL_DEPTH_TEST));
            FLK_GL_CALL(glDepthFunc(ToGlType(config.depth.func)));
        } else {
            FLK_GL_CALL(glDisable(GL_DEPTH_TEST));
        }

        if (config.clear.clearColor) {
            const auto vec = config.clear.color.ToVector();
            FLK_GL_CALL(glClearColor(vec.x, vec.y, vec.z, 1.0F));
            FLK_GL_CALL(glClear(GL_COLOR_BUFFER_BIT));
        }

        if (config.clear.clearDepth) {
            FLK_GL_CALL(glClearDepth(config.clear.depth));
            FLK_GL_CALL(glClear(GL_DEPTH_BUFFER_BIT));
        } else {
            FLK_GL_CALL(glDisable(GL_DEPTH_TEST));
        }

        if (config.raster.fill) {
            FLK_GL_CALL(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
        } else {
            FLK_GL_CALL(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
        }
    }

    void Renderer::SetMatrices(Pipeline &pipeline, const Transform &transform, const Camera &camera, const f32 aspectRatio) {
        const Matrix4f model = transform.Matrix();
        const Matrix4f view  = camera.ViewMatrix();
        const Matrix4f proj  = camera.ProjMatrix(aspectRatio);

        pipeline.SetUniform("uModel", model);
        pipeline.SetUniform("uView", view);
        pipeline.SetUniform("uProj", proj);
    }

    void Renderer::SetMaterialUniforms(Pipeline &pipeline, const MaterialProperties &material) {
        pipeline.SetUniform("uColor", material.color);
        pipeline.SetUniform("uMetallic", material.metallic);
        pipeline.SetUniform("uRoughness", material.roughness);

        if (material.colorMap) {
            pipeline.SetUniform("uColorMap", *material.colorMap);
        }
        if (material.metallicMap) {
            pipeline.SetUniform("uMetallicMap", *material.metallicMap);
        }
        if (material.roughnessMap) {
            pipeline.SetUniform("uRoughnessMap", *material.roughnessMap);
        }
    }

    void Renderer::SetLightUniforms(Pipeline &pipeline, std::vector<Light> lights, ShadowConfig shadowConfig) {
        pipeline.SetUniform("uNumLights", static_cast<i32>(lights.size()));
        i32 shadowIdx = 0;
        for (usize i = 0; i < lights.size(); i++) {
            const auto &[lightPosition, color, intensity, radius, hasShadows] = lights[i];

            std::string idx = "[" + std::to_string(i) + "]";
            pipeline.SetUniform("uLightPositions" + idx, lightPosition);
            pipeline.SetUniform("uLightColors" + idx, color);
            pipeline.SetUniform("uLightIntensities" + idx, intensity);
            pipeline.SetUniform("uLightRadii" + idx, radius);

            if (hasShadows && shadowConfig.enabled) {
                pipeline.SetUniform("uLightShadowMapIndices" + idx, shadowIdx);
                shadowIdx++;
            } else {
                pipeline.SetUniform("uLightShadowMapIndices" + idx, -1);
            }
        }
    }

    std::vector<Light> Renderer::NearestLights(std::vector<Light> lights, const Vector3f center, const usize count) {
        std::sort(lights.begin(), lights.end(), [&](const Light &lhs, const Light &rhs) -> bool {
            return (center - lhs.position).Magnitude() < (center - rhs.position).Magnitude();
        });

        if (lights.size() > count) {
            lights.resize(count);
        }

        return lights;
    }

    ShadowData Renderer::GenerateShadowMaps(
        const RenderList &        commands,
        const std::vector<Light> &lights,
        const ShadowConfig        shadowConfig,
        const Vector3f            shadowCenter
    ) {
        std::vector<Light> shadowLights;
        for (const auto &light: lights) {
            if (light.hasShadows && light.radius == 0.0F) {
                shadowLights.push_back(light);
            }
        }

        ShadowData data;

        data.shadowMaps = TextureArray::Create(
            shadowLights.size() * shadowConfig.cascadeRanges.size(),
            shadowConfig.resolution,
            {.format = TextureFormat::Depth}
        );

        data.spaceMatrices.resize(shadowLights.size() * shadowConfig.cascadeRanges.size());

        const f32 aspectRatio = static_cast<f32>(shadowConfig.resolution.x) / static_cast<f32>(shadowConfig.resolution.y);
        for (usize i = 0; i < shadowLights.size(); i++) {
            for (usize r = 0; r < shadowConfig.cascadeRanges.size(); r++) {
                const f32 range = shadowConfig.cascadeRanges[r];
                const i32 idx   = i * shadowConfig.cascadeRanges.size() + r;

                GenerateShadowMap(commands, data.shadowMaps, idx, shadowLights[i], shadowCenter, range);
                data.spaceMatrices[idx] = shadowLights[i].LightSpaceMatrix(range, aspectRatio, shadowCenter);
            }
        }

        return data;
    }

    bool Renderer::GenerateShadowMap(
        const RenderList &  commands,
        const TextureArray &textureArray,
        const u32           index,
        const Light &       light,
        const Vector3f      shadowCenter,
        const f32           range
    ) {
        static Framebuffer framebuffer = Framebuffer::Create().value();

        if (!framebuffer.Attach(Attachment::Depth, textureArray, index)) {
            Debug::LogErr("Renderer::GenerateShadowMap: Failed to attach depth texture!");
            return false;
        }

        if (!SetFramebuffer(&framebuffer)) {
            Debug::LogErr("Renderer::GenerateShadowMap: Failed to bind framebuffer!");
            return false;
        }

        const RenderConfig config = {
            .viewport = {{0, 0}, {textureArray.Size().x, textureArray.Size().y}},
            .clear    = {
                .clearColor = false
            }
        };

        ConfigureFramebuffer(config);

        const f32      aspectRatio = static_cast<f32>(textureArray.Size().x) / static_cast<f32>(textureArray.Size().y);
        const Matrix4f spaceMat    = light.LightSpaceMatrix(range, aspectRatio, shadowCenter);

        static const Shader vert     = Shader::Create(VertexShader, s_ShadowVertShader).value();
        static const Shader frag     = Shader::Create(FragmentShader, s_ShadowFragShader).value();
        static Pipeline     pipeline = Pipeline::Create(vert, frag).value();

        for (auto &cmd: commands) {
            if (!cmd.mesh) {
                continue;
            }

            const Matrix4f model = cmd.transform.Matrix();

            pipeline.SetUniform("uModel", model);
            pipeline.SetUniform("uView", Matrix4f{});
            pipeline.SetUniform("uProj", spaceMat);

            RenderMesh(*cmd.mesh, pipeline);
        }

        Mesh::Unbind();
        Pipeline::Unbind();
        Framebuffer::Unbind();

        return true;
    }

    bool Renderer::RenderMesh(const Mesh &mesh, const Pipeline &pipeline) {
        if (!pipeline.Bind()) {
            Debug::LogErr("Render command failed: Unable to bind pipeline!");
            return false;
        }

        if (!mesh.Bind()) {
            Debug::LogErr("Render command failed: Unable to bind mesh!");
            return false;
        }

        FLK_GL_CALL(glDrawElements(GL_TRIANGLES, mesh.IndexCount(), GL_UNSIGNED_INT, nullptr));
        return true;
    }

    bool Renderer::RenderSkybox(const CubeMap &cubeMap, const Matrix4f &view, const Matrix4f &proj) {
        const Mesh cube = Mesh::Box(Vector3f::One());

        static const Shader vert     = Shader::Create(VertexShader, s_SkyboxVertShader).value();
        static const Shader frag     = Shader::Create(FragmentShader, s_SkyboxFragShader).value();
        static Pipeline     pipeline = Pipeline::Create(vert, frag).value();

        pipeline.SetUniform("uSkybox", cubeMap);
        pipeline.SetUniform("uView", view);
        pipeline.SetUniform("uProj", proj);

        return RenderMesh(cube, pipeline);
    }
}
