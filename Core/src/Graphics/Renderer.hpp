#ifndef FLK_RENDERER_HPP
#define FLK_RENDERER_HPP

#include "Framebuffer.hpp"
#include "Material.hpp"
#include "Mesh.hpp"
#include "Asset/AssetLoader.hpp"
#include "Math/Rect.hpp"
#include "Math/Transform.hpp"

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
        Mesh *    mesh;
        Transform transform;
    };

    struct FLK_API ShadowMap {
        Texture2D texture;
        Matrix4f  lightSpaceMatrix;
    };

    struct FLK_API Light {
        Vector3f position   = {};
        Color3u8 color      = Color3u8::White();
        f32      intensity  = 1.0F;
        f32      radius     = 0.0F;
        bool     isStatic   = false;
        bool     hasShadows = true;

        [[nodiscard]] Matrix4f GetLightSpaceMatrix(const f32 range, const f32 aspectRatio, Vector3f center = {}) const {
            if (isStatic) {
                center = position;
            }

            const Vector3f lightDir = -position.Normalized();
            const Vector3f up = std::abs(lightDir.Dot(Vector3f::Up())) < 0.99F ? Vector3f::Up() : Vector3f::Forward();
            Matrix4f       view = Matrix4f::LookAt(center - lightDir * range, center, up);

            const Matrix4f proj = Matrix4f::Orthographic(
                -aspectRatio * range,
                aspectRatio * range,
                -range,
                range,
                0.0F,
                range * 2.0F
            );

            return view * proj;
        }
    };

    struct FLK_API Camera {
        Vector3f   position   = {};
        Quaternion rotation   = {};
        Projection projection = Projection::Orthographic;
        f32        size       = 5.0F;
        f32        fovY       = 60.0F;
        f32        nearZ      = 0.1F;
        f32        farZ       = 1000.0F;
    };

    struct FLK_API RenderConfig {
        std::optional<Color3u8> clearColor          = Color3u8{30, 30, 30};
        DepthFunc               depthFunc           = DepthFunc::Less;
        bool                    clearDepth          = true;
        bool                    blending            = true;
        Camera                  camera              = {};
        Rect2u                  viewport            = {{0, 0}, {800, 600}};
        Color3u8                ambientColor        = Color3u8::White();
        f32                     ambientIntensity    = 0.1F;
        std::vector<Light>      lights              = {};
        f32                     shadowRange         = 5.0F;
        Vector2u                shadowMapResolution = {2048, 2048};
    };

    struct FLK_API RenderCommand {
        Mesh *    mesh = nullptr;
        Material  material;
        Transform transform;
    };

    using RenderQueue = std::vector<RenderCommand>;

    class FLK_API Renderer {
        RenderQueue                 m_RenderQueue;
        std::optional<RenderConfig> m_RenderConfig;
        TextureArray                m_ShadowMaps;
        std::vector<u32>            m_LightShadowMapIndices;

    public:
        Renderer &BeginPass(const RenderConfig &config);
        Renderer &Submit(const RenderCommand &command);
        Renderer &Render(Asset::AssetLoader &assetLoader, OptionalRef<Framebuffer> framebuffer = std::nullopt);

    private:
        static void ClearFrame(OptionalRef<Framebuffer> framebuffer = std::nullopt);

        void GenerateShadowMaps(
            const std::vector<RenderObject> &objects,
            const std::vector<Light> &       lights,
            Vector3f                         offset,
            f32                              range
        );

        static bool GenerateShadowMap(
            std::vector<RenderObject> objects,
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
