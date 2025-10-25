#include "SystemsManager.hpp"

#include <unordered_map>

namespace Pixf::Core::Entities {
    void SystemsManager::RemoveSystem(const unsigned int systemId) { m_Systems.erase(systemId); }
    void SystemsManager::OnAwake(EntityManager &entityManager) {
        for (auto &[_, system]: m_Systems) {
            system->OnAwake(entityManager);
        }
    }

    void SystemsManager::OnUpdate(EntityManager &entityManager, const double deltaTime) {
        for (auto &[_, system]: m_Systems) {
            system->OnUpdate(entityManager, deltaTime);
        }
    }

    void SystemsManager::OnLateUpdate(EntityManager &entityManager, const double deltaTime) {
        for (auto &[_, system]: m_Systems) {
            system->OnLateUpdate(entityManager, deltaTime);
        }
    }

    void SystemsManager::OnRender(EntityManager &entityManager) {
        for (auto &[_, system]: m_Systems) {
            system->OnRender(entityManager);
        }
    }
} // namespace Pixf::Core::Entities
