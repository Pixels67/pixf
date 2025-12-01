#include "Renderer.hpp"

namespace Pixf::Core::Graphics {
    void Renderer::BeginPass(const RenderPass &pass) { m_CurrentPass = pass; }

    void Renderer::AddLights(const LightingEnvironment &lights) { m_LightEnv = lights; }

    void Renderer::Submit(const RenderCommand &cmd) { m_RenderQueue.push(cmd); }

    void Renderer::Render(const Gl::Viewport &viewport, Resources &resources) {
        if (!m_CurrentPass.has_value()) {
            PIXF_CORE_LOG_ERROR("Failed to render: Called Render without calling BeginPass");
            return;
        }

        viewport.Apply();
        ProcessPass(m_CurrentPass.value());

        static MeshHandle s_PreviousMesh;
        static MaterialHandle s_PreviousMaterial;
        static ShaderHandle s_PreviousShader;
        static bool s_First = true;

        ShaderHandle previousFrameShader;

        while (!m_RenderQueue.empty()) {
            const auto [meshHandle, matHandle, modelMatrix] = m_RenderQueue.front();
            m_RenderQueue.pop();

            Mesh &mesh = resources.meshStore.Get(meshHandle);
            Material &material = resources.materialStore.Get(matHandle);
            Gl::Shader &shader = resources.shaderStore.Get(material.GetShader());

            if (meshHandle.id != s_PreviousMesh.id || meshHandle.version != s_PreviousMesh.version || s_First) {
                mesh.Bind();
                s_PreviousMesh = meshHandle;
            }

            if (material.GetShader().id != s_PreviousShader.id ||
                material.GetShader().version != s_PreviousShader.version || s_First) {
                shader.Bind();

                s_PreviousShader = material.GetShader();
            }

            if (matHandle.id != s_PreviousMaterial.id || matHandle.version != s_PreviousMaterial.version || s_First) {
                if (m_LightEnv) {
                    SetLights(shader, m_LightEnv.value());
                }

                BindMaterial(material, resources);
                s_PreviousMaterial = matHandle;
                s_First = false;
            }

            shader.SetUniform("uModel", modelMatrix);
            shader.SetUniform("uView", m_CurrentPass->viewMatrix);
            shader.SetUniform("uProj", m_CurrentPass->projectionMatrix);

            PIXF_GL_CALL(glDrawElements(GL_TRIANGLES, mesh.GetIndexCount(), GL_UNSIGNED_INT, nullptr));
        }
    }

    void Renderer::ProcessPass(const RenderPass &pass) {
        if (pass.depthTest) {
            PIXF_GL_CALL(glEnable(GL_DEPTH_TEST));
        }

        PIXF_GL_CALL(glEnable(GL_MULTISAMPLE));
        PIXF_GL_CALL(glEnable(GL_CULL_FACE));

        const Math::Color3u8 clearColor = pass.clearColor;
        PIXF_GL_CALL(glClearColor(clearColor.r / 255.0F, clearColor.g / 255.0F, clearColor.b / 255.0F, 1.0F));

        unsigned int glClearFlags = 0;
        if (pass.clearFlags & ClearColor) {
            glClearFlags |= GL_COLOR_BUFFER_BIT;
        }

        if (pass.clearFlags & ClearDepth) {
            glClearFlags |= GL_DEPTH_BUFFER_BIT;
        }

        if (pass.clearFlags & ClearStencil) {
            glClearFlags |= GL_STENCIL_BUFFER_BIT;
        }

        PIXF_GL_CALL(glClear(glClearFlags));
    }

    void Renderer::BindMaterial(const Material &material, Resources &resources) {
        Gl::Shader &shader = resources.shaderStore.Get(material.GetShader());

        for (auto &[name, value]: material.GetFloatUniforms()) {
            shader.SetUniform(name, value);
        }

        for (auto &[name, value]: material.GetIntUniforms()) {
            shader.SetUniform(name, value);
        }

        for (auto &[name, value]: material.GetCharUniforms()) {
            shader.SetUniform(name, value);
        }

        for (auto &[name, value]: material.GetVector2Uniforms()) {
            shader.SetUniform(name, value);
        }

        for (auto &[name, value]: material.GetVector3Uniforms()) {
            shader.SetUniform(name, value);
        }

        for (auto &[name, value]: material.GetVector4Uniforms()) {
            shader.SetUniform(name, value);
        }

        for (auto &[name, value]: material.GetColor3Uniforms()) {
            shader.SetUniform(name, value);
        }

        for (auto &[name, value]: material.GetColor4Uniforms()) {
            shader.SetUniform(name, value);
        }

        for (auto &[name, value]: material.GetTexture2DUniforms()) {
            shader.SetUniform(name, resources.textureStore.Get(value));
        }
    }

    void Renderer::SetLights(Gl::Shader &shader, const LightingEnvironment &lights) {
        shader.SetUniform("uAmbientLight.color", lights.ambientLight.color);
        shader.SetUniform("uAmbientLight.intensity", lights.ambientLight.intensity);

        shader.SetUniform("uPointLightCount", static_cast<int>(lights.pointLights.size()));
        for (int i = 0; i < std::min(16, static_cast<int>(lights.pointLights.size())); i++) {
            std::string idx = std::to_string(i);
            shader.SetUniform("uPointLights[" + idx + "].position", lights.pointLights[i].position);
            shader.SetUniform("uPointLights[" + idx + "].color", lights.pointLights[i].color);
            shader.SetUniform("uPointLights[" + idx + "].intensity", lights.pointLights[i].intensity);
            shader.SetUniform("uPointLights[" + idx + "].linearFalloff", lights.pointLights[i].linearFalloff);
            shader.SetUniform("uPointLights[" + idx + "].quadraticFalloff", lights.pointLights[i].quadraticFalloff);
        }

        shader.SetUniform("uDirectionalLightCount", static_cast<int>(lights.directionalLights.size()));
        for (int i = 0; i < std::min(8, static_cast<int>(lights.directionalLights.size())); i++) {
            std::string idx = std::to_string(i);
            shader.SetUniform("uDirectionalLights[" + idx + "].direction", lights.directionalLights[i].direction);
            shader.SetUniform("uDirectionalLights[" + idx + "].color", lights.directionalLights[i].color);
            shader.SetUniform("uDirectionalLights[" + idx + "].intensity", lights.directionalLights[i].intensity);
        }
    }
} // namespace Pixf::Core::Graphics
