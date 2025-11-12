#include "SystemsManager.hpp"

#include <unordered_map>

namespace Pixf::Core::Entities {
    System::~System() = default;

    void System::OnAwake(World &entityManager) {}
    void System::OnUpdate(World &entityManager, double deltaTime) {}
    void System::OnLateUpdate(World &entityManager, double deltaTime) {}
    void System::OnRender(World &entityManager, double deltaTime) {}

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

    void SystemsManager::OnRender(World &world, const double deltaTime) {
        for (auto &[_, system]: m_Systems) {
            system->OnRender(world, deltaTime);
        }
    }

    void SystemsManager::Clear() {
        m_Systems.clear();
        m_SystemCounter = 0;
    }
} // namespace Pixf::Core::Entities
