#include "Renderer.hpp"

namespace Flock::Graphics {
    static constexpr usize s_MaxLightsPerObject = 16;

    static constexpr auto s_DefaultVertShader = R"(
#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

uniform mat4 uModel;
uniform mat4 uVP;

void main() {
    vec4 worldPos = vec4(aPosition, 1.0) * uModel;
    gl_Position = worldPos * uVP;
}
)";

    static constexpr auto s_DefaultFragShader = R"(
#version 330 core

out vec4 FragColor;

void main() {
    FragColor = vec4(1.0);
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

    Renderer &Renderer::BeginPass(const RenderPassConfig &config) {
        while (!m_RenderQueue.empty()) {
            m_RenderQueue.pop();
        }

        m_RenderPass = config;

        return *this;
    }

    Renderer &Renderer::Submit(const RenderCommand &command) {
        m_RenderQueue.push(command);

        return *this;
    }

    Renderer &Renderer::Render(
        SceneData &                    scene,
        ShadowConfig                   shadowConfig,
        Asset::AssetLoader &           assetLoader,
        const OptionalRef<Framebuffer> framebuffer
    ) {
        if (!m_RenderPass) {
            ClearFrame();
            return *this;
        }

        auto lights = GetNearestLights(scene.lights, scene.camera.position, s_MaxLightsPerObject);

        std::vector<RenderObject> objects;
        RenderQueue               queue = m_RenderQueue;
        while (!queue.empty()) {
            auto obj = queue.front();
            queue.pop();
            objects.push_back({.mesh = obj.mesh, .transform = obj.transform});
        }

        f32      shadowRange  = shadowConfig.shadowRange;
        Vector3f shadowCenter = scene.camera.position;
        GenerateShadowMaps(shadowConfig, objects, lights, shadowCenter);

        if (framebuffer) {
            if (!framebuffer->get().Bind()) {
                Debug::LogErr("Render failed: Unable to bind framebuffer!");
                ClearFrame();
                return *this;
            }
        } else {
            Framebuffer::Unbind();
        }

        ConfigureFrame(m_RenderPass.value());

        while (!m_RenderQueue.empty()) {
            auto [mesh, material, trans] = m_RenderQueue.front();
            m_RenderQueue.pop();

            Pipeline &pipeline = assetLoader.Get(material.pipeline).value();

            Matrix4f model = trans.GetMatrix();
            Matrix4f view  = scene.camera.GetViewMatrix();

            auto     [origin, aspect] = m_RenderPass->viewport;
            f32      aspectRatio      = static_cast<f32>(aspect.x - origin.x) / static_cast<f32>(aspect.y - origin.y);
            Matrix4f proj             = scene.camera.GetProjMatrix(aspectRatio);

            pipeline.SetUniform("uModel", model);
            pipeline.SetUniform("uView", view);
            pipeline.SetUniform("uProj", proj);
            pipeline.SetUniform("uCameraPosition", scene.camera.position);

            pipeline.SetUniform("uColor", material.color);
            pipeline.SetUniform("uMetallic", material.metallic);
            pipeline.SetUniform("uRoughness", material.roughness);

            if (material.colorMap) {
                pipeline.SetUniform("uColorMap", assetLoader.Get(material.colorMap.value()).value());
            }

            if (material.metallicMap) {
                pipeline.SetUniform("uMetallicMap", assetLoader.Get(material.metallicMap.value()).value());
            }

            if (material.roughnessMap) {
                pipeline.SetUniform("uRoughnessMap", assetLoader.Get(material.roughnessMap.value()).value());
            }

            pipeline.SetUniform("uAmbientColor", scene.ambientLight.color);
            pipeline.SetUniform("uAmbientIntensity", scene.ambientLight.intensity);

            pipeline.SetUniform("uNumLights", static_cast<i32>(lights.size()));
            for (i32 i = 0; i < lights.size(); i++) {
                const auto &[lightPosition, color, intensity, radius, hasShadows] = lights[i];

                const f32 shadowAspect = static_cast<f32>(shadowConfig.shadowMapResolution.x) /
                                         static_cast<f32>(shadowConfig.shadowMapResolution.y);

                const Vector3f offset   = scene.camera.position;
                Matrix4f       spaceMat = lights[i].GetLightSpaceMatrix(shadowRange, shadowAspect, offset);

                pipeline.SetUniform("uLightPositions[" + std::to_string(i) + "]", lightPosition);
                pipeline.SetUniform("uLightColors[" + std::to_string(i) + "]", color);
                pipeline.SetUniform("uLightIntensities[" + std::to_string(i) + "]", intensity);
                pipeline.SetUniform("uLightRadii[" + std::to_string(i) + "]", radius);
                pipeline.SetUniform("uLightSpaceMatrices[" + std::to_string(i) + "]", spaceMat);

                if (m_LightShadowMapIndices[i] == ~0u) {
                    pipeline.SetUniform("uLightShadowMapIndices[" + std::to_string(i) + "]", -1);
                } else {
                    pipeline.SetUniform("uLightShadowMapIndices[" + std::to_string(i) + "]",
                                        static_cast<i32>(m_LightShadowMapIndices[i]));
                }
            }

            if (m_ShadowMaps.GetLayerCount() > 0) {
                if (!pipeline.SetUniform("uShadowMaps", m_ShadowMaps)) {
                    Debug::LogErr("Render: Failed to upload shadow maps!");
                    return *this;
                }
            }

            RenderMesh(*mesh, pipeline);
        }

        Framebuffer::Unbind();
        Mesh::Unbind();
        Pipeline::Unbind();

        m_RenderPass = std::nullopt;

        return *this;
    }

    void Renderer::ClearFrame(const OptionalRef<Framebuffer> framebuffer) {
        if (framebuffer) {
            if (!framebuffer->get().Bind()) {
                return;
            }
        } else {
            Framebuffer::Unbind();
        }

        FLK_GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    }

    void Renderer::ConfigureFrame(RenderPassConfig config) {
        auto [origin, aspect] = config.viewport;

        // Hardcoded for now
        FLK_GL_CALL(glEnable(GL_CULL_FACE));
        FLK_GL_CALL(glCullFace(GL_BACK));
        FLK_GL_CALL(glFrontFace(GL_CCW));
        FLK_GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        FLK_GL_CALL(glViewport(origin.x, origin.y, aspect.x, aspect.y));

        if (config.clearColor) {
            const auto vec = config.clearColor->ToVector();
            FLK_GL_CALL(glClearColor(vec.x, vec.y, vec.z, 1.0F));
            FLK_GL_CALL(glClear(GL_COLOR_BUFFER_BIT));
        }

        FLK_GL_CALL(glDepthFunc(ToGlType(config.depthFunc)));

        if (config.clearDepth) {
            FLK_GL_CALL(glEnable(GL_DEPTH_TEST));
            FLK_GL_CALL(glClear(GL_DEPTH_BUFFER_BIT));
        } else {
            FLK_GL_CALL(glDisable(GL_DEPTH_TEST));
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

    void Renderer::GenerateShadowMaps(
        const ShadowConfig               shadowConfig,
        const std::vector<RenderObject> &objects,
        const std::vector<Light> &       lights,
        const Vector3f                   offset
    ) {
        m_LightShadowMapIndices.resize(lights.size());

        std::vector<usize> lightIndices;
        u32                counter = 0;
        for (usize i = 0; i < lights.size(); i++) {
            if (lights[i].hasShadows && lights[i].radius == 0.0F) {
                m_LightShadowMapIndices[i] = counter;
                lightIndices.push_back(i);
                counter++;
            } else {
                m_LightShadowMapIndices[i] = ~0u;
            }
        }

        m_ShadowMaps = TextureArray::Create(
            counter,
            shadowConfig.shadowMapResolution,
            {.format = TextureFormat::Depth}
        );

        for (const usize idx: m_LightShadowMapIndices) {
            GenerateShadowMap(objects, m_ShadowMaps, idx, lights[idx], offset, shadowConfig.shadowRange);
        }
    }

    bool Renderer::GenerateShadowMap(
        std::vector<RenderObject> objects,
        const TextureArray &      textureArray,
        const u32                 index,
        const Light &             light,
        const Vector3f            offset,
        const f32                 range
    ) {
        static Framebuffer framebuffer = Framebuffer::Create().value();

        if (!framebuffer.Attach(Attachment::Depth, textureArray, index)) {
            Debug::LogErr("Shadow map: Failed to attach depth texture!");
            return false;
        }

        if (!framebuffer.Bind()) {
            Debug::LogErr("Shadow map: Failed to bind framebuffer!");
            return false;
        }

        FLK_GL_CALL(glEnable(GL_CULL_FACE));
        FLK_GL_CALL(glCullFace(GL_BACK));
        FLK_GL_CALL(glFrontFace(GL_CCW));

        FLK_GL_CALL(glEnable(GL_DEPTH_TEST));
        FLK_GL_CALL(glClear(GL_DEPTH_BUFFER_BIT));
        FLK_GL_CALL(glViewport(0, 0, textureArray.GetSize().x, textureArray.GetSize().y));

        const f32 aspectRatio = static_cast<f32>(textureArray.GetSize().x) / static_cast<f32>(textureArray.GetSize().y);

        const Matrix4f spaceMat = light.GetLightSpaceMatrix(range, aspectRatio, offset);

        static const Shader vert     = Shader::Create(VertexShader, s_DefaultVertShader).value();
        static const Shader frag     = Shader::Create(FragmentShader, s_DefaultFragShader).value();
        static Pipeline     pipeline = Pipeline::Create(vert, frag).value();

        for (auto &[mesh, trans]: objects) {
            const Matrix4f model =
                    Matrix4f::Scale(trans.scale) *
                    Matrix4f::Rotate(trans.rotation) *
                    Matrix4f::Translate(trans.position);

            pipeline.SetUniform("uModel", model);
            pipeline.SetUniform("uVP", spaceMat);

            RenderMesh(*mesh, pipeline);
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
