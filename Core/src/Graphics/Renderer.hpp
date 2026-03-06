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

    struct FLK_API Light {
        Vector3f position   = {};
        Color3u8 color      = Color3u8::White();
        f32      intensity  = 1.0F;
        f32      radius     = 0.0F; // 0 Means light is directional, -position is the direction.
        bool     hasShadows = true; // Only works for directional lights.

        [[nodiscard]] Matrix4f
        GetLightSpaceMatrix(const f32 range, const f32 aspectRatio, const Vector3f center) const {
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

    struct FLK_API AmbientLight {
        Color3u8 color     = {100, 100, 100};
        f32      intensity = 0.1F;
    };

    struct FLK_API Camera {
        Vector3f   position   = {};
        Quaternion rotation   = {};
        Projection projection = Projection::Orthographic;
        f32        size       = 1.0F;
        f32        fovY       = 60.0F;
        f32        nearZ      = 0.1F;
        f32        farZ       = 1000.0F;

        Matrix4f GetViewMatrix() const {
            return Matrix4f::Translate(-position) * Matrix4f::Rotate(rotation.Inverse());
        }

        Matrix4f GetProjMatrix(f32 aspectRatio) const {
            if (projection == Projection::Orthographic) {
                return Matrix4f::Orthographic(-aspectRatio * size, aspectRatio * size, -size, size, nearZ, farZ);
            }

            return Matrix4f::Perspective(fovY, aspectRatio, nearZ, farZ);
        }
    };

    struct SceneData {
        Camera             camera       = {};
        std::vector<Light> lights       = {};
        AmbientLight       ambientLight = {};
    };

    struct ShadowConfig {
        f32      shadowRange         = 3.0F;
        Vector2u shadowMapResolution = {4096, 4096};
    };

    struct RenderPassConfig {
        std::optional<Color3u8> clearColor = Color3u8{20, 20, 20};
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
