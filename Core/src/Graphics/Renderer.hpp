#ifndef PIXF_RENDERER_HPP
#define PIXF_RENDERER_HPP

#include "Common.hpp"
#include "Gl/Viewport.hpp"
#include "RenderCommand.hpp"
#include "RenderPass.hpp"
#include "Resources.hpp"

namespace Pixf::Core::Graphics {
    class PIXF_API Renderer {
    public:
        void BeginPass(const RenderPass &pass);
        void Submit(const RenderCommand &cmd);
        void Render(const Gl::Viewport &viewport, Resources &resources);

    private:
        std::optional<RenderPass> m_CurrentPass;
        std::queue<RenderCommand> m_RenderQueue;

        static void BindMaterial(const Material &material, Resources &resources);
    };
} // namespace Pixf::Core::Graphics

#endif // PIXF_RENDERER_HPP
