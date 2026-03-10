#ifndef FLK_RENDERER_HPP
#define FLK_RENDERER_HPP

#include "Framebuffer.hpp"
#include "Material.hpp"
#include "Mesh.hpp"
#include "Asset/AssetLoader.hpp"
#include "Math/Rect.hpp"
#include "Math/Transform.hpp"
#include "Camera.hpp"
#include "Light.hpp"

namespace Flock::Graphics {
    enum class DepthFunc {
        Always,
        Never,
        Equal,
        NEqual,
        Less,
        Greater,
        LEqual,
        GEqual,
    };

    i32 ToGlType(DepthFunc depthFunc);

    struct SceneData {
        Camera             camera       = {};
        std::vector<Light> lights       = {};
        AmbientLight       ambientLight = {};
    };

    struct ShadowConfig {
        f32      shadowRange         = 50.0F;
        Vector2u shadowMapResolution = {4096, 4096};
    };

    struct RenderPassConfig {
        std::optional<Color3u8> clearColor = Color3u8{10, 20, 25};
        DepthFunc               depthFunc  = DepthFunc::Less;
        bool                    clearDepth = true;
        bool                    blending   = true;
        Rect2u                  viewport   = {{0, 0}, {800, 600}};
    };

    struct FLK_API RenderObject {
        Mesh *    mesh = nullptr;
        Transform transform;
    };

    struct FLK_API RenderCommand {
        Mesh *    mesh = nullptr;
        Material  material;
        Transform transform;
    };

    using RenderQueue = std::queue<RenderCommand>;

    class FLK_API Renderer {
        RenderQueue                     m_RenderQueue;
        std::optional<RenderPassConfig> m_RenderPass;
        TextureArray                    m_ShadowMaps;
        std::vector<u32>                m_LightShadowMapIndices;

    public:
        Renderer &BeginPass(const RenderPassConfig &config);
        Renderer &Submit(const RenderCommand &command);
        Renderer &Render(
            SceneData &              scene,
            ShadowConfig             shadowConfig,
            Asset::AssetLoader &     assetLoader,
            OptionalRef<Framebuffer> framebuffer = std::nullopt
        );

    private:
        static void               ClearFrame(OptionalRef<Framebuffer> framebuffer = std::nullopt);
        static void               ConfigureFrame(RenderPassConfig config);
        static std::vector<Light> GetNearestLights(std::vector<Light> lights, Vector3f center, usize count);

        void GenerateShadowMaps(
            ShadowConfig                     shadowConfig,
            const std::vector<RenderObject> &objects,
            const std::vector<Light> &       lights,
            Vector3f                         offset
        );

        static bool GenerateShadowMap(
            const std::vector<RenderObject>& objects,
            const TextureArray &      textureArray,
            u32                       index,
            const Light &             light,
            Vector3f                  offset,
            f32                       range
        );

        static bool RenderMesh(const Mesh &mesh, const Pipeline &pipeline);
    };
}

#endif //FLK_RENDERER_HPP
