#include "Renderer.hpp"

namespace Pixf::Core::Graphics {
    void Renderer::BeginPass(const RenderPass &pass) { m_CurrentPass = pass; }

    void Renderer::Submit(const RenderCommand &cmd) { m_RenderQueue.push(cmd); }

    void Renderer::Render(Resources &resources) {
        if (!m_CurrentPass.has_value()) {
            PIXF_CORE_LOG_ERROR("Failed to render: Called Render without calling BeginPass");
            return;
        }

        const Math::Color3u8 clearColor = m_CurrentPass.value().clearColor;
        PIXF_GL_CALL(glClearColor(clearColor.r / 255.0F, clearColor.g / 255.0F, clearColor.b / 255.0F, 1.0F));

        unsigned int glClearFlags = 0;
        if (m_CurrentPass.value().clearFlags & ClearColor) {
            glClearFlags |= GL_COLOR_BUFFER_BIT;
        }

        if (m_CurrentPass.value().clearFlags & ClearDepth) {
            glClearFlags |= GL_DEPTH_BUFFER_BIT;
        }

        if (m_CurrentPass.value().clearFlags & ClearStencil) {
            glClearFlags |= GL_STENCIL_BUFFER_BIT;
        }

        PIXF_GL_CALL(glClear(glClearFlags));

        while (!m_RenderQueue.empty()) {
            const auto [meshHandle, matHandle, modelMatrix] = m_RenderQueue.front();
            m_RenderQueue.pop();

            Material &material = resources.materialStore.Get(matHandle);
            Gl::Shader &shader = resources.shaderStore.Get(material.GetShader());
            shader.Bind();

            shader.SetUniform("uModel", modelMatrix);
            shader.SetUniform("uView", m_CurrentPass->viewMatrix);
            shader.SetUniform("uProj", m_CurrentPass->projectionMatrix);

            resources.meshStore.Get(meshHandle).Bind();

            BindMaterial(material, resources);

            Mesh &mesh = resources.meshStore.Get(meshHandle);
            PIXF_GL_CALL(glDrawElements(GL_TRIANGLES, mesh.GetIndexCount(), GL_UNSIGNED_INT, nullptr));

            Gl::Texture2D::Unbind(0);
            Gl::Shader::Unbind();
            Mesh::Unbind();
        }
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
} // namespace Pixf::Core::Graphics
