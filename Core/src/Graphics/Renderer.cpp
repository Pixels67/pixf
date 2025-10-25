#include "Renderer.hpp"

#include "Gl/Gl.hpp"
#include "Gl/Window.hpp"
#include "Material.hpp"
#include "RenderCommand.hpp"
#include "RenderQueue.hpp"
#include "ResourceManager.hpp"

namespace Pixf::Core::Graphics {
    Renderer::Renderer(const RendererConfig &config) : m_RenderConfig(config) {
        if (gladLoadGL() == 0) {
            ASSERT(false, "Failed to initialize GLAD");
        }

        gladLoadGLLoader(reinterpret_cast<void *(*) (const char *)>(glfwGetProcAddress));
        GL_CALL(glViewport(config.viewportOrigin.x, config.viewportOrigin.y, config.viewportAspect.x,
                           config.viewportAspect.y));

        GL_CALL(glEnable(GL_MULTISAMPLE));
        GL_CALL(glEnable(GL_DEPTH_TEST));
    }

    ResourceManager &Renderer::GetResourceManager() { return m_ResourceManager; }

    RenderQueue &Renderer::GetRenderQueue() { return m_RenderQueue; }

    void Renderer::Render() {
        const auto bgColor = vec3(m_RenderConfig.backgroundColor.r, m_RenderConfig.backgroundColor.g,
                                      m_RenderConfig.backgroundColor.b);

        GL_CALL(glClearColor(bgColor.r, bgColor.g, bgColor.b, 1.0F));
        GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        while (!m_RenderQueue.IsEmpty(RenderType::Opaque)) {
            const auto [ambientLight, directionalLights, pointLights, mesh, material, model, view, projection] =
                    m_RenderQueue.Pop(RenderType::Opaque);

            const auto mat = m_ResourceManager.GetMaterial(material).Unwrap();
            mat->Bind();

            Gl::Shader &shader = *m_ResourceManager.GetShader(mat->GetShader()).Unwrap();

            shader.SetUniform("uModel", model);
            shader.SetUniform("uView", view);
            shader.SetUniform("uProj", projection);

            shader.SetUniform("uAmbientLight.color", ambientLight.color);
            shader.SetUniform("uAmbientLight.intensity", ambientLight.intensity);

            shader.SetUniform("uDirectionalLightCount", static_cast<int>(directionalLights.size()));
            for (size_t j = 0; j < directionalLights.size(); j++) {
                std::string name = "uDirectionalLights[" + std::to_string(j) + "]";

                shader.SetUniform(name + ".direction", directionalLights[j].direction);
                shader.SetUniform(name + ".color", directionalLights[j].color);
                shader.SetUniform(name + ".intensity", directionalLights[j].intensity);
            }

            shader.SetUniform("uPointLightCount", static_cast<int>(pointLights.size()));
            for (size_t j = 0; j < pointLights.size(); j++) {
                std::string name = "uPointLights[" + std::to_string(j) + "]";

                shader.SetUniform(name + ".position", pointLights[j].position);
                shader.SetUniform(name + ".color", pointLights[j].color);
                shader.SetUniform(name + ".intensity", pointLights[j].intensity);

                shader.SetUniform(name + ".linearFalloff", pointLights[j].linearFalloff);
                shader.SetUniform(name + ".quadraticFalloff", pointLights[j].quadraticFalloff);
            }

            m_ResourceManager.GetMesh(mesh).Unwrap()->Bind();

            GL_CALL(glDrawElements(GL_TRIANGLES, m_ResourceManager.GetMesh(mesh).Unwrap()->GetIndexCount(),
                                   GL_UNSIGNED_INT, nullptr));

            Material::Unbind();
            Mesh::Unbind();
        }
    }
} // namespace Pixf::Core::Graphics
