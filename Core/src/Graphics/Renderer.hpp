#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "Assets/AssetManager.hpp"
#include "RenderQueue.hpp"

namespace Pixf::Core::Graphics {
    constexpr unsigned int g_MaxDirectionalLights = 8;
    constexpr unsigned int g_MaxPointLights = 16;

    struct PIXF_API RendererConfig {
        Math::Vector2i viewportOrigin = Math::Vector2i(0, 0);
        Math::Vector2i viewportAspect = Math::Vector2i(1080, 720);
        Math::Color3u8 backgroundColor = Math::Color3u8(30);
    };

    class PIXF_API Renderer {
    public:
        explicit Renderer(const RendererConfig &config);

        Renderer(const Renderer &) = delete;
        Renderer(Renderer &&) = delete;
        Renderer &operator=(const Renderer &) = delete;
        Renderer &operator=(Renderer &&) = delete;

        ~Renderer() = default;

        RenderQueue &GetRenderQueue();

        void Render(Assets::AssetManager &assetManager);
        void ClearViewport() const;

    private:
        RenderQueue m_RenderQueue;
        RendererConfig m_RenderConfig;

        static std::vector<Entities::Components::Graphics::PointLight>
        FilterPointLights(const std::vector<Entities::Components::Graphics::PointLight> &lights,
                          Math::Vector3f position);
    };
} // namespace Pixf::Core::Graphics

#endif // RENDERER_HPP
