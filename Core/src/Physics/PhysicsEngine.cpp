#include "PhysicsEngine.hpp"

namespace Flock::Physics {
    rp3d::Vector3 ToRp3dType(const Vector3f vec) {
        return rp3d::Vector3(vec.x, vec.y, vec.z);
    }

    rp3d::Quaternion ToRp3dType(const Quaternion quat) {
        rp3d::Quaternion out = rp3d::Quaternion(quat.x, quat.y, quat.z, quat.w).getInverse();
        out.normalize();
        return out;
    }

    Vector3f Rp3dVector(const rp3d::Vector3 vec) {
        return Vector3f(vec.x, vec.y, vec.z);
    }

    Quaternion Rp3dQuaternion(const rp3d::Quaternion quat) {
        return Quaternion(quat.x, quat.y, quat.z, quat.w).Inverse().Normalized();
    }

    PhysicsEngine PhysicsEngine::Create() {
        PhysicsEngine engine;
        engine.m_Common = std::make_unique<rp3d::PhysicsCommon>();
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
            const RigidTransform trans = collider.GetTransform();

            rp3d::Vector3    rbPos = ToRp3dType(pos + trans.position * rot);
            rp3d::Quaternion rbRot = ToRp3dType(trans.rotation * rot);
            rp3d::Transform  rbTrans(rbPos, rbRot);

            rp3d::RigidBody *body = m_World->createRigidBody(rbTrans);
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
            rp3d::Transform        trans = m_Bodies[i]->getTransform();
            const rp3d::Vector3    pos   = trans.getPosition();
            const rp3d::Quaternion rot   = trans.getOrientation();

            const rp3d::Vector3 linVlc = m_Bodies[i]->getLinearVelocity();
            const rp3d::Vector3 angVlc = m_Bodies[i]->getAngularVelocity();

            auto &collider = *m_Scene[i].collider;
            const RigidTransform colTrans = collider.GetTransform();

            m_Scene[i].transform->position    = Rp3dVector(pos) - colTrans.position * Rp3dQuaternion(rot);
            m_Scene[i].transform->rotation    = colTrans.rotation.Inverse() * Rp3dQuaternion(rot);
            m_Scene[i].transform->eulerAngles = {};

            m_Scene[i].rigidBody->linearVelocity  = Rp3dVector(linVlc);
            m_Scene[i].rigidBody->angularVelocity = Rp3dVector(angVlc);
        }
    }
}
