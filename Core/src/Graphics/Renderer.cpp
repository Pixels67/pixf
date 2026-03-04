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

    Renderer &Renderer::BeginPass(const RenderConfig &config) {
        m_RenderQueue.clear();
        m_RenderConfig = config;

        return *this;
    }

    Renderer &Renderer::Submit(const RenderCommand &command) {
        m_RenderQueue.push_back(command);

        return *this;
    }

    Renderer &Renderer::Render(Asset::AssetLoader &assetLoader, const OptionalRef<Framebuffer> framebuffer) {
        if (!m_RenderConfig) {
            ClearFrame();
            return *this;
        }

        std::sort(m_RenderConfig->lights.begin(), m_RenderConfig->lights.end(),
                  [&](const Light &lhs, const Light &rhs) -> bool {
                      return lhs.position.Magnitude() < rhs.position.Magnitude();
                  });

        const i32          lightCount = std::min(m_RenderConfig->lights.size(), s_MaxLightsPerObject);
        std::vector<Light> lights     = m_RenderConfig->lights;
        lights.resize(lightCount);

        std::vector<RenderObject> objects;
        for (auto cmd: m_RenderQueue) {
            objects.push_back(
                {.mesh = cmd.mesh, .transform = cmd.transform});
        }

        f32      shadowRange  = m_RenderConfig->shadowRange;
        Vector3f shadowCenter = m_RenderConfig->camera.position;
        GenerateShadowMaps(objects, lights, shadowCenter, shadowRange);

        if (framebuffer) {
            if (!framebuffer->get().Bind()) {
                Debug::LogErr("Render failed: Unable to bind framebuffer!");
                ClearFrame();
                return *this;
            }
        } else {
            Framebuffer::Unbind();
        }

        auto [origin, aspect] = m_RenderConfig->viewport;

        // Hardcoded for now
        FLK_GL_CALL(glEnable(GL_CULL_FACE));
        FLK_GL_CALL(glCullFace(GL_BACK));
        FLK_GL_CALL(glFrontFace(GL_CCW));
        FLK_GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        FLK_GL_CALL(glViewport(origin.x, origin.y, aspect.x, aspect.y));

        if (m_RenderConfig->clearColor) {
            const auto vec = m_RenderConfig->clearColor->ToVector();
            FLK_GL_CALL(glClearColor(vec.x, vec.y, vec.z, 1.0F));
            FLK_GL_CALL(glClear(GL_COLOR_BUFFER_BIT));
        }

        FLK_GL_CALL(glDepthFunc(ToGlType(m_RenderConfig->depthFunc)));

        if (m_RenderConfig->clearDepth) {
            FLK_GL_CALL(glEnable(GL_DEPTH_TEST));
            FLK_GL_CALL(glClear(GL_DEPTH_BUFFER_BIT));
        } else {
            FLK_GL_CALL(glDisable(GL_DEPTH_TEST));
        }

        auto queue = m_RenderQueue;
        std::reverse(queue.begin(), queue.end());
        while (!queue.empty()) {
            auto [mesh, material, trans] = queue.back();
            queue.pop_back();

            Pipeline &pipeline = assetLoader.Get(material.pipeline).value();

            Matrix4f model = Matrix4f::Identity() *
                             Matrix4f::Scale(trans.scale) *
                             Matrix4f::Rotate(trans.rotation) *
                             Matrix4f::Translate(trans.position);

            Matrix4f view = Matrix4f::Identity() *
                            Matrix4f::Translate(-m_RenderConfig->camera.position) *
                            Matrix4f::Rotate(m_RenderConfig->camera.rotation.Inverse());

            Matrix4f proj;

            f32 aspectRatio = static_cast<f32>(aspect.x - origin.x) / static_cast<f32>(aspect.y - origin.y);
            if (m_RenderConfig->camera.projection == Projection::Orthographic) {
                proj = Matrix4f::Orthographic(
                    -aspectRatio * m_RenderConfig->camera.size,
                    aspectRatio * m_RenderConfig->camera.size,
                    -m_RenderConfig->camera.size,
                    m_RenderConfig->camera.size,
                    m_RenderConfig->camera.size,
                    m_RenderConfig->camera.size
                );
            }

            if (m_RenderConfig->camera.projection == Projection::Perspective) {
                proj = Matrix4f::Perspective(
                    m_RenderConfig->camera.fovY,
                    aspectRatio,
                    m_RenderConfig->camera.nearZ,
                    m_RenderConfig->camera.farZ
                );
            }

            pipeline.SetUniform("uModel", model);
            pipeline.SetUniform("uView", view);
            pipeline.SetUniform("uProj", proj);

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

            pipeline.SetUniform("uAmbientColor", m_RenderConfig->ambientColor);
            pipeline.SetUniform("uAmbientIntensity", m_RenderConfig->ambientIntensity);

            pipeline.SetUniform("uNumLights", lightCount);
            for (i32 i = 0; i < lightCount; i++) {
                const auto &[lightPosition, color, intensity, radius, isStatic, hasShadows] = lights[i];

                const f32 shadowAspect = static_cast<f32>(m_RenderConfig->shadowMapResolution.x) /
                                         static_cast<f32>(m_RenderConfig->shadowMapResolution.y);

                Vector3f offset   = m_RenderConfig->camera.position;
                Matrix4f spaceMat = lights[i].GetLightSpaceMatrix(shadowRange, shadowAspect, offset);

                pipeline.SetUniform("uLightPositions[" + std::to_string(i) + "]", lightPosition);
                pipeline.SetUniform("uLightColors[" + std::to_string(i) + "]", color);
                pipeline.SetUniform("uLightIntensities[" + std::to_string(i) + "]", intensity);
                pipeline.SetUniform("uLightRadii[" + std::to_string(i) + "]", radius);
                pipeline.SetUniform("uLightSpaceMatrices[" + std::to_string(i) + "]", spaceMat);

                if (m_LightShadowMapIndices[i] == ~1u) {
                    pipeline.SetUniform("uLightShadowMapIndices[" + std::to_string(i) + "]", -1);
                } else {
                    pipeline.SetUniform("uLightShadowMapIndices[" + std::to_string(i) + "]",
                                        static_cast<i32>(m_LightShadowMapIndices[i]));
                }
            }

            pipeline.SetUniform("uCameraPosition", m_RenderConfig->camera.position);

            if (m_ShadowMaps.GetLayerCount() > 0) {
                if (!pipeline.SetUniform("uShadowMaps", m_ShadowMaps)) {
                    Debug::LogErr("Render: Failed to upload shadow maps!");
                    ClearFrame();
                    return *this;
                }
            }

            RenderMesh(*mesh, pipeline);
        }

        Framebuffer::Unbind();
        Mesh::Unbind();
        Pipeline::Unbind();

        m_RenderConfig = std::nullopt;

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

    void Renderer::GenerateShadowMaps(
        const std::vector<RenderObject> &objects,
        const std::vector<Light> &       lights,
        const Vector3f                   offset,
        const f32                        range
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
                m_LightShadowMapIndices[i] = ~1u;
            }
        }

        bool mapsReset = false;
        if (m_ShadowMaps.GetLayerCount() != counter || m_ShadowMaps.GetSize() != m_RenderConfig->shadowMapResolution) {
            m_ShadowMaps = TextureArray::Create(
                counter,
                m_RenderConfig->shadowMapResolution,
                {.format = TextureFormat::Depth}
            );

            mapsReset = true;
        }

        for (const usize idx: lightIndices) {
            if (lights[idx].isStatic && !mapsReset) {
                continue;
            }

            GenerateShadowMap(objects, m_ShadowMaps, idx, lights[idx], offset, range);
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
