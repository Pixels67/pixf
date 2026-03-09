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
        Color3u8 color     = {100, 100, 100};
        f32      intensity = 0.1F;
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
}

#endif //FLK_LIGHT_HPP
