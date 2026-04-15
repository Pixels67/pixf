#ifndef FLK_LIGHT_HPP
#define FLK_LIGHT_HPP

#include "Common.hpp"
#include "Math/Math.hpp"
#include "Serial/Archive.hpp"

namespace Flock::Graphics {
    struct FLK_API Light {
        Vector3f position   = {};
        Color3u8 color      = Color3u8::White();
        f32      intensity  = 1.0F;
        f32      radius     = 0.0F; // 0 Means light is directional, -position is the direction.
        bool     hasShadows = true; // Only works for directional lights.

        [[nodiscard]] Matrix4f
        LightSpaceMatrix(const f32 range, const f32 aspectRatio, const Vector3f center) const {
            const Vector3f lightDir = -position.Normalized();
            const Vector3f up       = std::abs(lightDir.Dot(Vector3f::Up())) < 0.99F ? Vector3f::Up() : Vector3f::Forward();
            Matrix4f       view     = Matrix4f::LookAt(center - lightDir * range, center, up);

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
        Color3u8 color     = {150, 180, 210};
        f32      intensity = 0.05F;
    };

    struct FLK_API PointLight {
        Vector3f position  = {};
        Color3u8 color     = Color3u8::White();
        f32      intensity = 1.0F;
        f32      radius    = 10.0F;

        [[nodiscard]] Light Light() const {
            return {
                .position   = position,
                .color      = color,
                .intensity  = intensity,
                .radius     = radius > 0.0F ? radius : 0.1F,
                .hasShadows = false,
            };
        }
    };

    struct FLK_API DirectionalLight {
        Vector3f position   = {};
        Color3u8 color      = Color3u8::White();
        f32      intensity  = 1.0F;
        bool     hasShadows = true;

        [[nodiscard]] Light Light() const {
            return {
                .position   = position,
                .color      = color,
                .intensity  = intensity,
                .radius     = 0.0F,
                .hasShadows = hasShadows,
            };
        }
    };

    FLK_ARCHIVE(Light, position, color, intensity, radius, hasShadows)
    FLK_ARCHIVE(AmbientLight, color, intensity)
    FLK_ARCHIVE(PointLight, position, color, intensity, radius)
    FLK_ARCHIVE(DirectionalLight, position, color, intensity, hasShadows)
}

#endif //FLK_LIGHT_HPP
