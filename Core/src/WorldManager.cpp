#include "WorldManager.hpp"

#include <string>
#include <unordered_map>

#include "Entities/World.hpp"
#include "Error/Result.hpp"

namespace Pixf::Core {
    void WorldManager::CreateWorld(const std::string &name, const Entities::World &world) {
        m_Worlds[name] = std::make_shared<Entities::World>(world);
    }

    Error::Result<std::shared_ptr<Entities::World>, WorldError> WorldManager::GetWorld(const std::string &name) {
        if (!m_Worlds.contains(name)) {
            return WorldError::WorldNotFound;
        }

        return m_Worlds.at(name);
    }

    void WorldManager::DeleteWorld(const std::string &name) {
        if (!m_Worlds.contains(name)) {
            return;
        }

        m_Worlds.erase(name);
    }

    WorldError WorldManager::SetActiveWorld(const std::string &name) {
        if (!m_Worlds.contains(name)) {
            return WorldError::WorldNotFound;
        }

        m_ActiveWorld = name;
        return WorldError::None;
    }

    Error::Result<std::shared_ptr<Entities::World>, WorldError> WorldManager::GetActiveWorld() {
        if (!m_Worlds.contains(m_ActiveWorld)) {
            return WorldError::NoActiveWorld;
        }

        return m_Worlds.at(m_ActiveWorld);
    }

    void WorldManager::Clear() {
        m_Worlds.clear();
        m_ActiveWorld.clear();
    }
} // namespace Pixf::Core
