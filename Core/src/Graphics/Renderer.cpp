#include "Renderer.hpp"

namespace Flock::Graphics {
    static constexpr usize s_MaxLightsPerObject = 16;

    static constexpr auto s_DefaultVertShader = R"(
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

    static constexpr auto s_DefaultFragShader = R"(
#version 330 core

out vec4 FragColor;

uniform vec3 uColor;

void main() {
    FragColor = vec4(uColor, 1.0);
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

    Renderer &Renderer::Render(const RenderList &commands, const SceneData &scene, RenderConfig config, const ShadowConfig shadowConfig) {
        const auto     lights       = GetNearestLights(scene.lights, scene.camera.transform.position, s_MaxLightsPerObject);
        const Vector3f shadowCenter = scene.camera.transform.position;
        TextureArray   shadowMaps;
        if (shadowConfig.enabled) {
            shadowMaps = GenerateShadowMaps(commands, lights, shadowConfig, shadowCenter);
        }

        SetFramebuffer(config.framebuffer);
        ConfigureFramebuffer(config);

        for (auto &cmd: commands) {
            auto &[mesh, pipeline, mat, trans] = cmd;

            pipeline.get().ResetUniforms();

            auto      [origin, aspect] = config.viewport;
            const f32 aspectRatio      = static_cast<f32>(aspect.x - origin.x) /
                                    static_cast<f32>(aspect.y - origin.y);

            SetMatrices(pipeline, trans, scene.camera, aspectRatio);

            pipeline.get().SetUniform("uCameraPosition", scene.camera.transform.position);
            pipeline.get().SetUniform("uAmbientColor", scene.ambientLight.color);
            pipeline.get().SetUniform("uAmbientIntensity", scene.ambientLight.intensity);

            SetMaterialUniforms(pipeline, mat);
            SetLightUniforms(pipeline, lights, shadowConfig, shadowCenter);

            if (shadowMaps.GetLayerCount() > 0) {
                if (!pipeline.get().SetUniform("uShadowMaps", shadowMaps)) {
                    Debug::LogErr("Render: Failed to upload shadow maps!");
                    return *this;
                }
            }

            RenderMesh(mesh, pipeline);
        }

        Framebuffer::Unbind();
        Mesh::Unbind();
        Pipeline::Unbind();

        return *this;
    }

    bool Renderer::SetFramebuffer(const OptionalRef<Framebuffer> framebuffer) {
        if (framebuffer) {
            if (!framebuffer->get().Bind()) {
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
            FLK_GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
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
        const Matrix4f model = transform.GetMatrix();
        const Matrix4f view  = camera.GetViewMatrix();
        const Matrix4f proj  = camera.GetProjMatrix(aspectRatio);

        pipeline.SetUniform("uModel", model);
        pipeline.SetUniform("uView", view);
        pipeline.SetUniform("uProj", proj);
    }

    void Renderer::SetMaterialUniforms(Pipeline &pipeline, const MaterialProperties &material) {
        pipeline.SetUniform("uColor", material.color);
        pipeline.SetUniform("uMetallic", material.metallic);
        pipeline.SetUniform("uRoughness", material.roughness);

        if (material.colorMap) {
            pipeline.SetUniform("uColorMap", material.colorMap.value());
        }
        if (material.metallicMap) {
            pipeline.SetUniform("uMetallicMap", material.metallicMap.value());
        }
        if (material.roughnessMap) {
            pipeline.SetUniform("uRoughnessMap", material.roughnessMap.value());
        }
    }

    void Renderer::SetLightUniforms(Pipeline &pipeline, std::vector<Light> lights, const ShadowConfig config, const Vector3f shadowCenter) {
        pipeline.SetUniform("uNumLights", static_cast<i32>(lights.size()));
        i32 shadowIdx = 0;
        for (usize i = 0; i < lights.size(); i++) {
            const auto &[lightPosition, color, intensity, radius, hasShadows] = lights[i];

            const f32 shadowAspect = static_cast<f32>(config.resolution.x) /
                                     static_cast<f32>(config.resolution.y);

            const Vector3f offset   = shadowCenter;
            const Matrix4f spaceMat = lights[i].GetLightSpaceMatrix(config.range, shadowAspect, offset);

            std::string idx = "[" + std::to_string(i) + "]";
            pipeline.SetUniform("uLightPositions" + idx, lightPosition);
            pipeline.SetUniform("uLightColors" + idx, color);
            pipeline.SetUniform("uLightIntensities" + idx, intensity);
            pipeline.SetUniform("uLightRadii" + idx, radius);
            pipeline.SetUniform("uLightSpaceMatrices" + idx, spaceMat);

            if (hasShadows) {
                pipeline.SetUniform("uLightShadowMapIndices" + idx, shadowIdx);
                shadowIdx++;
            } else {
                pipeline.SetUniform("uLightShadowMapIndices" + idx, -1);
            }
        }
    }

    std::vector<Light> Renderer::GetNearestLights(std::vector<Light> lights, const Vector3f center, const usize count) {
        std::sort(lights.begin(), lights.end(), [&](const Light &lhs, const Light &rhs) -> bool {
            return (center - lhs.position).Magnitude() < (center - rhs.position).Magnitude();
        });

        if (lights.size() > count) {
            lights.resize(count);
        }

        return lights;
    }

    TextureArray Renderer::GenerateShadowMaps(
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

        TextureArray shadowMaps = TextureArray::Create(
            shadowLights.size(),
            shadowConfig.resolution,
            {.format = TextureFormat::Depth}
        );

        for (usize i = 0; i < shadowLights.size(); i++) {
            GenerateShadowMap(commands, shadowMaps, i, shadowLights[i], shadowCenter, shadowConfig.range);
        }

        return shadowMaps;
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

        if (!SetFramebuffer(framebuffer)) {
            Debug::LogErr("Renderer::GenerateShadowMap: Failed to bind framebuffer!");
            return false;
        }

        const RenderConfig config = {
            .viewport = {{0, 0}, {textureArray.GetSize().x, textureArray.GetSize().y}},
            .clear    = {
                .clearColor = false
            }
        };

        ConfigureFramebuffer(config);

        const f32      aspectRatio = static_cast<f32>(textureArray.GetSize().x) / static_cast<f32>(textureArray.GetSize().y);
        const Matrix4f spaceMat    = light.GetLightSpaceMatrix(range, aspectRatio, shadowCenter);

        static const Shader vert     = Shader::Create(VertexShader, s_DefaultVertShader).value();
        static const Shader frag     = Shader::Create(FragmentShader, s_DefaultFragShader).value();
        static Pipeline     pipeline = Pipeline::Create(vert, frag).value();

        for (auto &cmd: commands) {
            const Matrix4f model = cmd.transform.GetMatrix();

            pipeline.SetUniform("uModel", model);
            pipeline.SetUniform("uView", Matrix4f{});
            pipeline.SetUniform("uProj", spaceMat);

            RenderMesh(cmd.mesh, pipeline);
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

        FLK_GL_CALL(glDrawElements(GL_TRIANGLES, mesh.GetIndexCount(), GL_UNSIGNED_INT, nullptr));
        return true;
    }
}
