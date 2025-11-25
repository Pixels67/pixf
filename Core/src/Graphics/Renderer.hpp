#ifndef PIXF_RENDERER_HPP
#define PIXF_RENDERER_HPP

#include "Common.hpp"
#include "Gl/Viewport.hpp"
#include "LightingEnvironment.hpp"
#include "RenderCommand.hpp"
#include "RenderPass.hpp"
#include "Resources.hpp"

namespace Pixf::Core::Graphics {
    class PIXF_API Renderer {
    public:
        void BeginPass(const RenderPass &pass);
        void AddLights(const LightingEnvironment &lights);
        void Submit(const RenderCommand &cmd);
        void Render(const Gl::Viewport &viewport, Resources &resources);

    private:
        std::optional<RenderPass> m_CurrentPass;
        std::optional<LightingEnvironment> m_LightEnv;
        std::queue<RenderCommand> m_RenderQueue;

        static void ProcessPass(const RenderPass &pass);
        static void BindMaterial(const Material &material, Resources &resources);
        static void SetLights(const Gl::Shader &shader, const LightingEnvironment &lights);
    };
} // namespace Pixf::Core::Graphics

#endif // PIXF_RENDERER_HPP
