#ifndef FLK_PHYSICSENGINE_HPP
#define FLK_PHYSICSENGINE_HPP

#include <reactphysics3d/engine/PhysicsCommon.h>

#include "Collider.hpp"
#include "Common.hpp"
#include "RigidBody.hpp"
#include "Math/Transform.hpp"

namespace Flock::Physics {
    namespace rp3d = reactphysics3d;

    struct FLK_API PhysicsObject {
        Transform *transform = nullptr;
        RigidBody *rigidBody = nullptr;
        Collider * collider  = nullptr;
    };

    class FLK_API PhysicsEngine {
        std::unique_ptr<rp3d::PhysicsCommon> m_Common = nullptr;
        rp3d::PhysicsWorld *                 m_World  = nullptr;

        std::vector<rp3d::RigidBody *> m_Bodies;
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
