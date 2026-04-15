#ifndef FLK_PHYSICSENGINE_HPP
#define FLK_PHYSICSENGINE_HPP

#include <reactphysics3d/engine/PhysicsCommon.h>
#include <memory>
#include <vector>

#include "Collider.hpp"
#include "Common.hpp"
#include "RigidBody.hpp"
#include "Math/Transform.hpp"

namespace Flock {
namespace Physics {
struct Collider;
struct RigidBody;
}  // namespace Physics
struct Transform;
}  // namespace Flock
namespace reactphysics3d {
class PhysicsWorld;
class RigidBody;
}  // namespace reactphysics3d

namespace Flock::Physics {
    namespace rp = reactphysics3d;

    struct FLK_API PhysicsObject {
        Transform *transform = nullptr;
        RigidBody *rigidBody = nullptr;
        Collider * collider  = nullptr;
    };

    class FLK_API PhysicsEngine {
        std::unique_ptr<rp::PhysicsCommon> m_Common = nullptr;
        rp::PhysicsWorld *                 m_World  = nullptr;

        std::vector<rp::RigidBody *> m_Bodies;
        std::vector<PhysicsObject>     m_Scene;

    public:
        static PhysicsEngine Create();

        PhysicsEngine()  = default;
        ~PhysicsEngine();

        PhysicsEngine(const PhysicsEngine &other)     = delete;
        PhysicsEngine(PhysicsEngine &&other) noexcept;

        PhysicsEngine &operator=(const PhysicsEngine &other)     = delete;
        PhysicsEngine &operator=(PhysicsEngine &&other) noexcept;

        void Clear();

        void SetScene(const std::vector<PhysicsObject> &objects);
        void Update(f32 timeStep) const;
    };
}

#endif //FLK_PHYSICSENGINE_HPP
