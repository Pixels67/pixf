#include "PhysicsEngine.hpp"

#include <utility>

#include "Math/Quaternion.hpp"
#include "Math/RigidTransform.hpp"
#include "Math/Transform.hpp"
#include "Math/Vector.hpp"
#include "Physics/Collider.hpp"
#include "Physics/RigidBody.hpp"
#include "reactphysics3d/body/RigidBody.h"
#include "reactphysics3d/engine/PhysicsCommon.h"
#include "reactphysics3d/engine/PhysicsWorld.h"
#include "reactphysics3d/mathematics/Quaternion.h"
#include "reactphysics3d/mathematics/Transform.h"
#include "reactphysics3d/mathematics/Vector3.h"

namespace Flock::Physics {
    rp::Vector3 ToRp3dType(const Vector3f vec) {
        return rp::Vector3(vec.x, vec.y, vec.z);
    }

    rp::Quaternion ToRp3dType(const Quaternion quat) {
        rp::Quaternion out = rp::Quaternion(quat.x, quat.y, quat.z, quat.w).getInverse();
        out.normalize();
        return out;
    }

    Vector3f Rp3dVector(const rp::Vector3 vec) {
        return Vector3f(vec.x, vec.y, vec.z);
    }

    Quaternion Rp3dQuaternion(const rp::Quaternion quat) {
        return Quaternion(quat.x, quat.y, quat.z, quat.w).Inverse().Normalized();
    }

    PhysicsEngine PhysicsEngine::Create() {
        PhysicsEngine engine;
        engine.m_Common = std::make_unique<rp::PhysicsCommon>();
        engine.m_World  = engine.m_Common->createPhysicsWorld();

        return engine;
    }

    PhysicsEngine::~PhysicsEngine() {
        Clear();
    }

    PhysicsEngine::PhysicsEngine(PhysicsEngine &&other) noexcept {
        m_Common       = std::move(other.m_Common);
        other.m_Common = nullptr;

        m_World       = other.m_World;
        other.m_World = nullptr;
    }

    PhysicsEngine &PhysicsEngine::operator=(PhysicsEngine &&other) noexcept {
        if (this == &other) {
            return *this;
        }

        Clear();

        m_Common       = std::move(other.m_Common);
        other.m_Common = nullptr;

        m_World       = other.m_World;
        other.m_World = nullptr;

        return *this;
    }

    void PhysicsEngine::Clear() {
        if (!m_Common) {
            return;
        }

        if (m_World) {
            m_Common->destroyPhysicsWorld(m_World);
        }

        m_World  = nullptr;
        m_Common = nullptr;
    }

    void PhysicsEngine::SetScene(const std::vector<PhysicsObject> &objects) {
        if (!m_Common || !m_World) {
            return;
        }

        m_Bodies.resize(objects.size());
        m_Scene = objects;

        for (usize i = 0; i < m_Scene.size(); i++) {
            auto &[pos, quat, scale, euler] = *m_Scene[i].transform;

            const Quaternion rot = quat * Quaternion::Euler(euler);

            auto &collider = *m_Scene[i].collider;
            const RigidTransform trans = collider.Transform();

            rp::Vector3    rbPos = ToRp3dType(pos + trans.position * rot);
            rp::Quaternion rbRot = ToRp3dType(trans.rotation * rot);
            rp::Transform  rbTrans(rbPos, rbRot);

            rp::RigidBody *body = m_World->createRigidBody(rbTrans);
            if (m_Bodies[i]) {
                m_World->destroyRigidBody(m_Bodies[i]);
            }

            m_Bodies[i] = body;

            body->addCollider(collider.BuildShape(*m_Common, scale), {});
            body->setType(ToRp3dType(m_Scene[i].rigidBody->mode));
            body->enableGravity(m_Scene[i].rigidBody->useGravity);
            body->setMass(m_Scene[i].rigidBody->mass);

            const Vector3f linVlc = m_Scene[i].rigidBody->linearVelocity;
            const Vector3f angVlc = m_Scene[i].rigidBody->angularVelocity;

            body->setLinearVelocity(ToRp3dType(linVlc));
            body->setAngularVelocity(ToRp3dType(angVlc));
        }
    }

    void PhysicsEngine::Update(const f32 timeStep) const {
        if (!m_Common || !m_World) {
            return;
        }

        m_World->update(timeStep);

        for (usize i = 0; i < m_Scene.size(); i++) {
            rp::Transform        trans = m_Bodies[i]->getTransform();
            const rp::Vector3    pos   = trans.getPosition();
            const rp::Quaternion rot   = trans.getOrientation();

            const rp::Vector3 linVlc = m_Bodies[i]->getLinearVelocity();
            const rp::Vector3 angVlc = m_Bodies[i]->getAngularVelocity();

            auto &collider = *m_Scene[i].collider;
            const RigidTransform colTrans = collider.Transform();

            m_Scene[i].transform->position    = Rp3dVector(pos) - colTrans.position * Rp3dQuaternion(rot);
            m_Scene[i].transform->rotation    = colTrans.rotation.Inverse() * Rp3dQuaternion(rot);
            m_Scene[i].transform->eulerAngles = {};

            m_Scene[i].rigidBody->linearVelocity  = Rp3dVector(linVlc);
            m_Scene[i].rigidBody->angularVelocity = Rp3dVector(angVlc);
        }
    }
}
