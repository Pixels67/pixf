#ifndef FLK_RENDERER_HPP
#define FLK_RENDERER_HPP

#include "Framebuffer.hpp"
#include "Material.hpp"
#include "Mesh.hpp"
#include "Asset/AssetLoader.hpp"
#include "Math/Rect.hpp"

namespace Flock::Graphics {
    enum class Projection {
        Orthographic,
        Perspective,
    };

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

    inline i32 ToGlType(const DepthFunc depthFunc) {
        switch (depthFunc) {
            case DepthFunc::Always:
                return GL_ALWAYS;
            case DepthFunc::Never:
                return GL_NEVER;
            case DepthFunc::Equal:
                return GL_EQUAL;
            case DepthFunc::NEqual:
                return GL_NOTEQUAL;
            case DepthFunc::Less:
                return GL_LESS;
            case DepthFunc::Greater:
                return GL_GREATER;
            case DepthFunc::LEqual:
                return GL_LEQUAL;
            case DepthFunc::GEqual:
                return GL_GEQUAL;
            default:
                return 0;
        }
    }

    struct FLK_API RenderObject {
        Mesh *     mesh;
        Vector3f   position = {};
        Quaternion rotation = {};
        Vector3f   scale    = Vector3f::One();
    };

    struct FLK_API ShadowMap {
        Texture2D texture;
        Matrix4f  lightSpaceMatrix;
    };

    struct FLK_API RenderCommand {
        Mesh *     mesh;
        Material   material;
        Vector3f   position = {};
        Quaternion rotation = {};
        Vector3f   scale    = Vector3f::One();
    };

    struct FLK_API Light {
        Vector3f position   = {};
        Color3u8 color      = Color3u8::White();
        f32      intensity  = 1.0F;
        f32      radius     = 0.0F;
        bool     isStatic   = false;
        bool     hasShadows = true;
    };

    struct FLK_API RenderPassConfig {
        std::optional<Color3u8> clearColor          = Color3u8{30, 30, 30};
        DepthFunc               depthFunc           = DepthFunc::Less;
        bool                    clearDepth          = true;
        bool                    blending            = true;
        Vector3f                cameraPosition      = {};
        Quaternion              cameraRotation      = {};
        Projection              projection          = Projection::Orthographic;
        Rect2u                  viewport            = {{0, 0}, {800, 600}};
        f32                     size                = 5.0F;
        f32                     fovY                = 60.0F;
        f32                     nearZ               = 0.1F;
        f32                     farZ                = 1000.0F;
        Color3u8                ambientColor        = Color3u8::White();
        f32                     ambientIntensity    = 0.1F;
        std::vector<Light>      lights              = {};
        f32                     shadowRange         = 1.0F;
        Vector2u                shadowMapResolution = {2048, 2048};
    };

    using RenderQueue = std::vector<RenderCommand>;

    class FLK_API Renderer {
        RenderQueue                     m_RenderQueue;
        std::optional<RenderPassConfig> m_RenderPass;
        TextureArray                    m_ShadowMaps;
        std::vector<u32>                m_LightShadowMapIndices;

    public:
        Renderer &BeginPass(const RenderPassConfig &config);

        Renderer &Submit(const RenderCommand &command);

        Renderer &Render(Asset::AssetLoader &assetLoader, OptionalRef<Framebuffer> framebuffer = std::nullopt);

    private:
        void GenerateShadowMaps(
            const std::vector<RenderObject> &objects,
            const std::vector<Light> &       lights,
            Vector3f                         centerPosition,
            f32                              range
        );

        static bool GenerateShadowMap(
            std::vector<RenderObject> objects,
            const TextureArray &      textureArray,
            u32                       index,
            Vector3f                  lightPosition,
            Vector3f                  centerPosition,
            f32                       range
        );

        static bool RenderMesh(const Mesh &mesh, const Pipeline &pipeline);

        static Matrix4f GetShadowMapView(Vector3f lightPosition, Vector3f centerPosition, f32 range);
        static Matrix4f GetShadowMapProj(f32 aspectRatio, f32 range);

        static void DumpDepthTexture(const Texture2D &texture, Vector2u resolution);
    };
}

#endif //FLK_RENDERER_HPP
