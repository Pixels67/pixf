#include "Renderer.hpp"

namespace Flock::Graphics {
    static constexpr usize s_MaxLightsPerObject = 2;

    static constexpr auto s_DefaultVertShader = R"(
#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

void main() {
    vec4 worldPos = vec4(aPosition, 1.0) * uModel;
    gl_Position = worldPos * uView * uProj;
}
)";

    static constexpr auto s_DefaultFragShader = R"(
#version 330 core

out vec4 FragColor;

void main() {
    FragColor = vec4(1.0);
}
)";

    Renderer &Renderer::BeginPass(const RenderPassConfig &config) {
        m_RenderQueue.clear();
        m_RenderPass = config;

        return *this;
    }

    Renderer &Renderer::Submit(const RenderCommand &command) {
        m_RenderQueue.push_back(command);

        return *this;
    }

    Renderer &Renderer::Render(Asset::AssetLoader &assetLoader, const OptionalRef<Framebuffer> framebuffer) {
        if (!m_RenderPass) {
            return *this;
        }

        std::sort(m_RenderPass->lights.begin(), m_RenderPass->lights.end(),
                  [&](const Light &lhs, const Light &rhs) -> bool {
                      return lhs.position.Magnitude() < rhs.position.Magnitude();
                  });

        const i32          lightCount = std::min(m_RenderPass->lights.size(), s_MaxLightsPerObject);
        std::vector<Light> lights     = m_RenderPass->lights;
        lights.resize(lightCount);

        std::vector<RenderObject> objects;
        for (auto cmd: m_RenderQueue) {
            objects.push_back(
                {.mesh = cmd.mesh, .position = cmd.position, .rotation = cmd.rotation, .scale = cmd.scale});
        }

        f32      shadowRange  = m_RenderPass->shadowRange;
        Vector3f shadowCenter = m_RenderPass->cameraPosition;
        GenerateShadowMaps(objects, lights, shadowCenter, shadowRange);

        if (framebuffer) {
            if (!framebuffer->get().Bind()) {
                Debug::LogErr("Render failed: Unable to bind framebuffer!");
                return *this;
            }
        } else {
            Framebuffer::Unbind();
        }

        auto [origin, aspect] = m_RenderPass->viewport;

        // Hardcoded for now
        FLK_GL_CALL(glEnable(GL_CULL_FACE));
        FLK_GL_CALL(glCullFace(GL_BACK));
        FLK_GL_CALL(glFrontFace(GL_CCW));
        FLK_GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        FLK_GL_CALL(glViewport(origin.x, origin.y, aspect.x, aspect.y));

        if (m_RenderPass->clearColor) {
            const auto vec = m_RenderPass->clearColor->ToVector();
            FLK_GL_CALL(glClearColor(vec.x, vec.y, vec.z, 1.0F));
            FLK_GL_CALL(glClear(GL_COLOR_BUFFER_BIT));
        }

        FLK_GL_CALL(glDepthFunc(ToGlType(m_RenderPass->depthFunc)));

        if (m_RenderPass->clearDepth) {
            FLK_GL_CALL(glEnable(GL_DEPTH_TEST));
            FLK_GL_CALL(glClear(GL_DEPTH_BUFFER_BIT));
        } else {
            FLK_GL_CALL(glDisable(GL_DEPTH_TEST));
        }

        auto queue = m_RenderQueue;
        std::reverse(queue.begin(), queue.end());
        while (!queue.empty()) {
            auto [mesh, material, position, rotation, scale] = queue.back();
            queue.pop_back();

            Pipeline &pipeline = assetLoader.Get(material.pipeline).value();

            Matrix4f model = Matrix4f::Identity() *
                             Matrix4f::Rotate(rotation) *
                             Matrix4f::Scale(scale) *
                             Matrix4f::Translate(position);

            Matrix4f view = Matrix4f::Identity() *
                            Matrix4f::Translate(-m_RenderPass->cameraPosition) *
                            Matrix4f::Rotate(m_RenderPass->cameraRotation.Inverse());

            Matrix4f proj;

            f32 aspectRatio = static_cast<f32>(aspect.x - origin.x) / static_cast<f32>(aspect.y - origin.y);
            if (m_RenderPass->projection == Projection::Orthographic) {
                proj = Matrix4f::Orthographic(
                    -aspectRatio * m_RenderPass->size,
                    aspectRatio * m_RenderPass->size,
                    -m_RenderPass->size,
                    m_RenderPass->size,
                    m_RenderPass->nearZ,
                    m_RenderPass->farZ
                );
            }

            if (m_RenderPass->projection == Projection::Perspective) {
                proj = Matrix4f::Perspective(
                    m_RenderPass->fovY,
                    aspectRatio,
                    m_RenderPass->nearZ,
                    m_RenderPass->farZ
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

            pipeline.SetUniform("uAmbientColor", m_RenderPass->ambientColor);
            pipeline.SetUniform("uAmbientIntensity", m_RenderPass->ambientIntensity);

            pipeline.SetUniform("uNumLights", lightCount);
            for (i32 i = 0; i < lightCount; i++) {
                const auto &[lightPosition, color, intensity, radius, isStatic, hasShadows] = lights[i];

                Matrix4f spaceMat =
                        GetShadowMapView(lightPosition, shadowCenter, shadowRange) * GetShadowMapProj(
                            aspectRatio, shadowRange);

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

            pipeline.SetUniform("uCameraPosition", m_RenderPass->cameraPosition);

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

    void Renderer::GenerateShadowMaps(
        const std::vector<RenderObject> &objects,
        const std::vector<Light> &       lights,
        const Vector3f                   centerPosition,
        const f32                        range
    ) {
        m_LightShadowMapIndices.resize(lights.size());

        std::vector<usize> lightIndices;
        u32                counter = 0;
        for (usize i = 0; i < lights.size(); i++) {
            if (lights[i].hasShadows) {
                m_LightShadowMapIndices[i] = counter;
                lightIndices.push_back(i);
                counter++;
            } else {
                m_LightShadowMapIndices[i] = ~1u;
            }
        }

        bool mapsReset = false;
        if (m_ShadowMaps.GetLayerCount() != counter || m_ShadowMaps.GetSize() != m_RenderPass->shadowMapResolution) {
            m_ShadowMaps = TextureArray::Create(
                counter,
                m_RenderPass->shadowMapResolution,
                {.format = TextureFormat::Depth}
            );

            mapsReset = true;
        }

        for (usize i = 0; i < lightIndices.size(); i++) {
            if (lights[i].isStatic && !mapsReset) {
                continue;
            }

            GenerateShadowMap(objects, m_ShadowMaps, lightIndices[i], lights[i].position, centerPosition, range);
        }
    }

    bool Renderer::GenerateShadowMap(
        std::vector<RenderObject> objects,
        const TextureArray &      textureArray,
        const u32                 index,
        const Vector3f            lightPosition,
        const Vector3f            centerPosition,
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

        const Matrix4f view = GetShadowMapView(lightPosition, centerPosition, range);
        const Matrix4f proj = GetShadowMapProj(aspectRatio, range);

        static const Shader vert     = Shader::Create(VertexShader, s_DefaultVertShader).value();
        static const Shader frag     = Shader::Create(FragmentShader, s_DefaultFragShader).value();
        static Pipeline     pipeline = Pipeline::Create(vert, frag).value();

        for (auto &[mesh, position, rotation, scale]: objects) {
            const Matrix4f model =
                    Matrix4f::Scale(scale) *
                    Matrix4f::Rotate(rotation) *
                    Matrix4f::Translate(position);

            pipeline.SetUniform("uModel", model);
            pipeline.SetUniform("uView", view);
            pipeline.SetUniform("uProj", proj);

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

    Matrix4f Renderer::GetShadowMapView(const Vector3f lightPosition, const Vector3f centerPosition, const f32 range) {
        const Vector3f lightDir = (-lightPosition).Normalized();
        const Vector3f target   = centerPosition;
        const Vector3f eye      = target - lightDir * range;
        const Vector3f up       = std::abs(lightDir.y) > 0.99F ? Vector3f::Forward() : Vector3f::Up();

        return Matrix4f::LookAt(eye, target, up);
    }

    Matrix4f Renderer::GetShadowMapProj(f32 aspectRatio, f32 range) {
        return Matrix4f::Orthographic(
            -aspectRatio * range,
            aspectRatio * range,
            -range,
            range,
            0.0F,
            range * 2.0F
        );
    }

    void Renderer::DumpDepthTexture(const Texture2D &texture, const Vector2u resolution) {
        std::vector<f32> pixels(resolution.x * resolution.y);

        glBindTexture(GL_TEXTURE_2D, texture.GetGlId());
        glGetTexImage(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, GL_FLOAT, pixels.data());
        glBindTexture(GL_TEXTURE_2D, 0);

        std::vector<u8> output(pixels.size());
        const f32       minV = *std::min_element(pixels.begin(), pixels.end());
        const f32       maxV = *std::max_element(pixels.begin(), pixels.end());

        for (u32 y = 0; y < resolution.y; ++y) {
            u32 srcY = (resolution.y - 1) - y;
            for (u32 x = 0; x < resolution.x; ++x) {
                size_t i                     = srcY * resolution.x + x;
                float  v                     = pixels[i];
                output[y * resolution.x + x] =
                        static_cast<u8>(((v - minV) / (maxV - minV)) * 255.0f);
            }
        }

        std::ofstream file("depth_debug.pgm", std::ios::binary);
        file << "P5\n" << resolution.x << " " << resolution.y << "\n255\n";
        file.write(reinterpret_cast<const char *>(output.data()), output.size());
    }
}
