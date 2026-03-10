#include "PhysicsEngine.hpp"

namespace Flock::Physics {
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

            const Quaternion rot = (quat * Quaternion::Euler(euler)).Normalized();

            rp3d::Vector3    rbPos(pos.x, pos.y, pos.z);
            rp3d::Quaternion rbRot(rot.x, rot.y, rot.z, rot.w);
            rp3d::Transform  rbTrans(rbPos, rbRot);

            rp3d::RigidBody *body = m_World->createRigidBody(rbTrans);
            if (m_Bodies[i]) {
                m_World->destroyRigidBody(m_Bodies[i]);
            }

            m_Bodies[i] = body;

            body->addCollider(m_Scene[i].collider->BuildShape(*m_Common, scale), rp3d::Transform::identity());
            body->setType(ToRp3dType(m_Scene[i].rigidBody->mode));
            body->enableGravity(m_Scene[i].rigidBody->useGravity);
            body->setMass(m_Scene[i].rigidBody->mass);

            const Vector3f linVlc = m_Scene[i].rigidBody->linearVelocity;
            const Vector3f angVlc = m_Scene[i].rigidBody->linearVelocity;

            body->setLinearVelocity(rp3d::Vector3(linVlc.x, linVlc.y, linVlc.z));
            body->setAngularVelocity(rp3d::Vector3(angVlc.x, angVlc.y, angVlc.z));
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

            m_Scene[i].transform->position    = Vector3f{pos.x, pos.y, pos.z};
            m_Scene[i].transform->rotation    = Quaternion{rot.x, rot.y, rot.z, rot.w}.Normalized();
            m_Scene[i].transform->eulerAngles = {};

            m_Scene[i].rigidBody->linearVelocity  = {linVlc.x, linVlc.y, linVlc.z};
            m_Scene[i].rigidBody->angularVelocity = {angVlc.x, angVlc.y, angVlc.z};
        }
    }
}
