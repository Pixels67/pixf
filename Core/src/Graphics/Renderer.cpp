#include "Renderer.hpp"

#include "Assets/AssetManager.hpp"
#include "Gl/Gl.hpp"
#include "Material.hpp"
#include "RenderCommand.hpp"
#include "RenderQueue.hpp"

namespace Pixf::Core::Graphics {
    Renderer::Renderer(const RendererConfig &config) : m_RenderConfig(config) {
        if (gladLoadGL() == 0) {
            PIXF_ASSERT(false, "Failed to initialize GLAD");
        }

        gladLoadGLLoader(reinterpret_cast<void *(*) (const char *)>(glfwGetProcAddress));
        PIXF_GL_CALL(glViewport(config.viewportOrigin.x, config.viewportOrigin.y, config.viewportAspect.x,
                                config.viewportAspect.y));

        PIXF_GL_CALL(glEnable(GL_MULTISAMPLE));
        PIXF_GL_CALL(glEnable(GL_DEPTH_TEST));
    }

    RenderQueue &Renderer::GetRenderQueue() { return m_RenderQueue; }

    void Renderer::Render(Assets::AssetManager &assetManager) {
        const auto bgColor = vec3(m_RenderConfig.backgroundColor.r, m_RenderConfig.backgroundColor.g,
                                  m_RenderConfig.backgroundColor.b);

        PIXF_GL_CALL(glClearColor(bgColor.r, bgColor.g, bgColor.b, 1.0F));
        PIXF_GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        while (!m_RenderQueue.IsEmpty(RenderType::Opaque)) {
            const auto [ambientLight, directionalLights, pointLights, mesh, material, model, view, projection] =
                    m_RenderQueue.Pop(RenderType::Opaque);
            auto worldPos = vec3(model * vec4(0.0F));

            const auto mat =
                    assetManager.GetMaterial(material).Unwrap("Failed to render: Invalid RenderCommand.material!");
            mat->Bind();

            Gl::Shader &shader = *assetManager.GetShader(mat->GetShader()).Unwrap();

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

            auto filteredPointLights = FilterPointLights(pointLights, worldPos);
            shader.SetUniform("uPointLightCount", static_cast<int>(filteredPointLights.size()));
            for (size_t j = 0; j < filteredPointLights.size(); j++) {
                std::string name = "uPointLights[" + std::to_string(j) + "]";

                shader.SetUniform(name + ".position", filteredPointLights[j].position);
                shader.SetUniform(name + ".color", filteredPointLights[j].color);
                shader.SetUniform(name + ".intensity", filteredPointLights[j].intensity);

                shader.SetUniform(name + ".linearFalloff", filteredPointLights[j].linearFalloff);
                shader.SetUniform(name + ".quadraticFalloff", filteredPointLights[j].quadraticFalloff);
            }

            assetManager.GetMesh(mesh).Unwrap("Failed to render: Invalid RenderCommand.mesh!")->Bind();

            PIXF_GL_CALL(glDrawElements(GL_TRIANGLES, assetManager.GetMesh(mesh).Unwrap()->GetIndexCount(),
                                        GL_UNSIGNED_INT, nullptr));

            Material::Unbind();
            Mesh::Unbind();
        }
    }

    std::vector<Entities::Components::Graphics::PointLight>
    Renderer::FilterPointLights(const std::vector<Entities::Components::Graphics::PointLight> &lights, vec3 position) {
        if (lights.size() <= g_MaxPointLights) {
            return lights;
        }

        auto filtered = lights;

        auto compare = [position](const auto &lhs, const auto &rhs) {
            const float dist1 = distance(lhs.position, position);
            const float dist2 = distance(rhs.position, position);
            return dist1 < dist2;
        };

        std::sort(filtered.begin(), filtered.end(), compare);

        filtered.resize(g_MaxPointLights);

        return filtered;
    }
} // namespace Pixf::Core::Graphics
