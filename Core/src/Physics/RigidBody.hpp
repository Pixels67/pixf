#ifndef FLK_RIGIDBODY_HPP
#define FLK_RIGIDBODY_HPP

#include "Common.hpp"
#include "Reflect.hpp"

namespace Flock::Physics {
    namespace rp = reactphysics3d;

    enum class SimulationMode {
        None,
        Dynamic,
        Kinematic,
        Static
    };

    inline rp::BodyType ToRp3dType(const SimulationMode mode) {
        switch (mode) {
            case SimulationMode::Dynamic:
                return rp::BodyType::DYNAMIC;
            case SimulationMode::Kinematic:
                return rp::BodyType::KINEMATIC;
            case SimulationMode::Static:
                return rp::BodyType::STATIC;
            default:
                FLK_ASSERT(false);
        }
    }

    struct FLK_API RigidBody {
        Vector3f       linearVelocity  = {};
        Vector3f       angularVelocity = {};
        f32            mass            = 1.0F;
        SimulationMode mode            = SimulationMode::Dynamic;
        bool           useGravity      = true;
    };

    inline auto Reflect(RigidBody &rigidBody) {
        return Reflectable{
            "RigidBody",
            std::make_tuple(
                Field{"linearVelocity", &rigidBody.linearVelocity},
                Field{"angularVelocity", &rigidBody.angularVelocity},
                Field{"mass", &rigidBody.mass},
                Field{"mode", &rigidBody.mode},
                Field{"useGravity", &rigidBody.useGravity}
            )
        };
    }
}

#endif //FLK_RIGIDBODY_HPP
