#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "Assets/AssetManager.hpp"
#include "RenderQueue.hpp"

namespace Pixf::Core::Graphics {
    struct RendererConfig {
        ivec2 viewportOrigin = ivec2(0, 0);
        ivec2 viewportAspect = ivec2(1080, 720);
        vec3 backgroundColor = vec3(0.12f, 0.12f, 0.12f);
    };

    class Renderer {
    public:
        explicit Renderer(const RendererConfig &config);

        Renderer(const Renderer &) = delete;
        Renderer(Renderer &&) = delete;
        Renderer &operator=(const Renderer &) = delete;
        Renderer &operator=(Renderer &&) = delete;

        ~Renderer() = default;

        RenderQueue &GetRenderQueue();

        void Render(Assets::AssetManager &assetManager);

    private:
        RenderQueue m_RenderQueue;
        RendererConfig m_RenderConfig;
    };
} // namespace Pixf::Core::Graphics

#endif // RENDERER_HPP
