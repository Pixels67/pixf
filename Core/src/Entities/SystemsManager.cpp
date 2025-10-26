#include "SystemsManager.hpp"

#include <unordered_map>

#include "World.hpp"

namespace Pixf::Core::Entities {
    void SystemsManager::RemoveSystem(const unsigned int systemId) { m_Systems.erase(systemId); }
    void SystemsManager::OnAwake(World &world) {
        for (auto &[_, system]: m_Systems) {
            system->OnAwake(world);
        }
    }

    void SystemsManager::OnUpdate(World &world, const double deltaTime) {
        for (auto &[_, system]: m_Systems) {
            system->OnUpdate(world, deltaTime);
        }
    }

    void SystemsManager::OnLateUpdate(World &world, const double deltaTime) {
        for (auto &[_, system]: m_Systems) {
            system->OnLateUpdate(world, deltaTime);
        }
    }

    void SystemsManager::OnRender(World &world) {
        for (auto &[_, system]: m_Systems) {
            system->OnRender(world);
        }
    }
} // namespace Pixf::Core::Entities
