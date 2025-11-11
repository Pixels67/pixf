#include "WorldManager.hpp"

#include <string>
#include <unordered_map>

#include "Application.hpp"
#include "Entities/World.hpp"
#include "Error/Result.hpp"
#include "File/File.hpp"

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
        GetActiveWorld().Unwrap()->Awake();
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

    void WorldManager::SaveWorld(const std::string &path, const std::string &name) const {
        if (m_Worlds.contains(name)) {
            const Json::object json = m_Worlds.at(name)->Serialize();
            File::WriteFile(path, Json::ToPrettyJson(json));
        }
    }

    void WorldManager::LoadWorld(const std::string &path, const std::string &name, const Entities::Blueprint &blueprint) {
        const std::string str = File::ReadFile(path).Unwrap();
        const Json::object json = Json::parse(str).as_object();

        Entities::World world(m_Application, blueprint);
        world.Deserialize(json, m_Application.GetAssetManager());
        CreateWorld(name, world);
    }
} // namespace Pixf::Core
