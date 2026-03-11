#ifndef FLK_COLLIDER_HPP
#define FLK_COLLIDER_HPP

#include <reactphysics3d/reactphysics3d.h>

#include "Common.hpp"
#include "Math/Math.hpp"
#include "Math/RigidTransform.hpp"

namespace Flock::Physics {
    namespace rp3d = reactphysics3d;

    struct FLK_API Collider {
        virtual ~Collider() = default;

        virtual rp3d::CollisionShape *BuildShape(rp3d::PhysicsCommon &common, Vector3f scale) = 0;
        virtual RigidTransform        GetTransform() = 0;
    };

    struct FLK_API BoxCollider : Collider {
        RigidTransform transform   = {};
        Vector3f       halfExtents = Vector3f::One();

        explicit BoxCollider(const RigidTransform &transform = {}, const Vector3f halfExtents = Vector3f::One())
            : transform(transform), halfExtents(halfExtents) {
        }

        rp3d::CollisionShape *BuildShape(rp3d::PhysicsCommon &common, const Vector3f scale) override {
            return common.createBoxShape(reactphysics3d::Vector3(
                halfExtents.x * scale.x, halfExtents.y * scale.y, halfExtents.z * scale.z
            ));
        }

        RigidTransform GetTransform() override {
            return transform;
        }
    };

    struct FLK_API SphereCollider : Collider {
        RigidTransform transform = {};
        f32            radius    = 1.0F;

        explicit SphereCollider(const RigidTransform &transform = {}, const f32 radius = 1.0F)
            : transform(transform), radius(radius) {
        }

        rp3d::CollisionShape *BuildShape(rp3d::PhysicsCommon &common, const Vector3f scale) override {
            const f32 s = std::min({scale.x, scale.y, scale.z});
            return common.createSphereShape(radius * s);
        }

        RigidTransform GetTransform() override {
            return transform;
        }
    };

    inline auto Reflect(BoxCollider &collider) {
        return Reflectable{
            "BoxCollider",
            std::make_tuple(
                Field{"transform", &collider.transform},
                Field{"halfExtents", &collider.halfExtents}
            )
        };
    }

    inline auto Reflect(SphereCollider &collider) {
        return Reflectable{
            "SphereCollider",
            std::make_tuple(
                Field{"transform", &collider.transform},
                Field{"radius", &collider.radius}
            )
        };
    }
}

#endif //FLK_COLLIDER_HPP
