#ifndef FLK_LIGHT_HPP
#define FLK_LIGHT_HPP

#include "Common.hpp"
#include "Math/Math.hpp"
#include "Reflect.hpp"

namespace Flock::Graphics {
    struct FLK_API Light {
        Vector3f position   = {};
        Color3u8 color      = Color3u8::White();
        f32      intensity  = 1.0F;
        f32      radius     = 0.0F; // 0 Means light is directional, -position is the direction.
        bool     hasShadows = true; // Only works for directional lights.

        [[nodiscard]] Matrix4f
        GetLightSpaceMatrix(const f32 range, const f32 aspectRatio, const Vector3f center) const {
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

    inline auto Reflect(Light &light) {
        return Reflectable{
            "Light",
            std::make_tuple(
                Field{"position", &light.position},
                Field{"color", &light.color},
                Field{"intensity", &light.intensity},
                Field{"radius", &light.radius},
                Field{"hasShadows", &light.hasShadows}
            )
        };
    }

    struct FLK_API AmbientLight {
        Color3u8 color     = {40, 80, 100};
        f32      intensity = 0.05F;
    };

    inline auto Reflect(AmbientLight &light) {
        return Reflectable{
            "AmbientLight",
            std::make_tuple(
                Field{"color", &light.color},
                Field{"intensity", &light.intensity}
            )
        };
    }

    struct FLK_API PointLight {
        Vector3f position  = {};
        Color3u8 color     = Color3u8::White();
        f32      intensity = 1.0F;
        f32      radius    = 10.0F;

        [[nodiscard]] Light GetLight() const {
            return {
                .position   = position,
                .color      = color,
                .intensity  = intensity,
                .radius     = radius > 0.0F ? radius : 0.1F,
                .hasShadows = false,
            };
        }
    };

    inline auto Reflect(PointLight &light) {
        return Reflectable{
            "PointLight",
            std::make_tuple(
                Field{"position", &light.position},
                Field{"color", &light.color},
                Field{"intensity", &light.intensity},
                Field{"radius", &light.radius}
            )
        };
    }

    struct FLK_API DirectionalLight {
        Vector3f position   = {};
        Color3u8 color      = Color3u8::White();
        f32      intensity  = 1.0F;
        bool     hasShadows = true;

        [[nodiscard]] Light GetLight() const {
            return {
                .position   = position,
                .color      = color,
                .intensity  = intensity,
                .radius     = 0.0F,
                .hasShadows = hasShadows,
            };
        }
    };

    inline auto Reflect(DirectionalLight &light) {
        return Reflectable{
            "DirectionalLight",
            std::make_tuple(
                Field{"position", &light.position},
                Field{"color", &light.color},
                Field{"intensity", &light.intensity},
                Field{"hasShadows", &light.hasShadows}
            )
        };
    }
}

#endif //FLK_LIGHT_HPP
