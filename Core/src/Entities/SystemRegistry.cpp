#include "SystemRegistry.hpp"

namespace Pixf::Core::Entities {
    void SystemRegistry::Awake(Registry &registry) {
        for (auto &[typeId, system]: m_Systems) {
            system->Awake(registry);
        }
    }

    void SystemRegistry::Start(Registry &registry) {
        for (auto &[typeId, system]: m_Systems) {
            system->Start(registry);
        }
    }

    void SystemRegistry::Update(Registry &registry, const double deltaTime) {
        for (auto &[typeId, system]: m_Systems) {
            system->Update(registry, deltaTime);
        }
    }

    void SystemRegistry::Render(Registry &registry, const double deltaTime) {
        for (auto &[typeId, system]: m_Systems) {
            system->Render(registry, deltaTime);
        }
    }

    void SystemRegistry::RenderGui(Registry &registry, const double deltaTime) {
        for (auto &[typeId, system]: m_Systems) {
            system->RenderGui(registry, deltaTime);
        }
    }

    void SystemRegistry::OnEvent(Registry &registry, Event::Event &event) {
        for (auto &[typeId, system]: m_Systems) {
            system->OnEvent(registry, event);
        }
    }

    void SystemRegistry::Clear() { m_Systems.clear(); }
} // namespace Pixf::Core::Entities
