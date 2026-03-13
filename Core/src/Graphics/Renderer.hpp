#ifndef FLK_RENDERER_HPP
#define FLK_RENDERER_HPP

#include "Framebuffer.hpp"
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

    enum class CullMode {
        None,
        Front,
        Back
    };

    i32 ToGlType(CullMode faceCullMode);

    struct ClearState {
        bool     clearColor   = true;
        bool     clearDepth   = true;
        bool     clearStencil = false;
        Color4u8 color        = {20, 20, 20, 255};
        f32      depth        = 1.0F;
        u32      stencil      = 0;
    };

    struct DepthState {
        bool      enabled = true;
        DepthFunc func    = DepthFunc::LEqual;
    };

    struct BlendState {
        bool enabled = true;
    };

    struct RasterState {
        CullMode cullMode = CullMode::Back;
        bool     fill     = true;
    };

    struct SceneData {
        Camera             camera       = {};
        std::vector<Light> lights       = {};
        AmbientLight       ambientLight = {};
    };

    struct ShadowConfig {
        bool             enabled       = true;
        Vector2u         resolution    = {2048, 2048};
        std::vector<f32> cascadeRanges = {20.0F, 100.0F, 500.0F};
    };

    struct ShadowData {
        TextureArray          shadowMaps;
        std::vector<Matrix4f> spaceMatrices;
    };

    struct RenderConfig {
        Rect2u                   viewport;
        ClearState               clear       = {};
        DepthState               depth       = {};
        BlendState               blend       = {};
        RasterState              raster      = {};
        OptionalRef<Framebuffer> framebuffer = std::nullopt;
    };

    struct MaterialProperties {
        Color4u8             color        = Color4u8::White();
        f32                  metallic     = 0.25F;
        f32                  roughness    = 0.75F;
        OptionalRef<Texture> colorMap     = std::nullopt;
        OptionalRef<Texture> metallicMap  = std::nullopt;
        OptionalRef<Texture> roughnessMap = std::nullopt;
    };

    struct RenderCommand {
        Ref<Mesh>          mesh;
        Ref<Pipeline>      pipeline;
        MaterialProperties materialProperties = {};
        Transform          transform          = {};
    };

    using RenderList = std::vector<RenderCommand>;

    class FLK_API Renderer {
    public:
        Renderer &Render(const RenderList &commands, const SceneData &scene, RenderConfig config = {}, ShadowConfig shadowConfig = {});

    private:
        static bool SetFramebuffer(OptionalRef<Framebuffer> framebuffer = std::nullopt);
        static void ConfigureFramebuffer(RenderConfig config);
        static void SetMatrices(Pipeline &pipeline, const Transform &transform, const Camera &camera, f32 aspectRatio);
        static void SetMaterialUniforms(Pipeline &pipeline, const MaterialProperties &material);
        static void SetLightUniforms(Pipeline &pipeline, std::vector<Light> lights);

        static std::vector<Light> GetNearestLights(std::vector<Light> lights, Vector3f center, usize count);

        static ShadowData GenerateShadowMaps(
            const RenderList &        commands,
            const std::vector<Light> &lights,
            ShadowConfig              shadowConfig,
            Vector3f                  shadowCenter
        );

        static bool GenerateShadowMap(
            const RenderList &  commands,
            const TextureArray &textureArray,
            u32                 index,
            const Light &       light,
            Vector3f            shadowCenter,
            f32                 range
        );

        static bool RenderMesh(const Mesh &mesh, const Pipeline &pipeline);
    };
}

#endif //FLK_RENDERER_HPP
