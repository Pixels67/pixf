#include "WorldManager.hpp"

#include "Files/File.hpp"
#include "Serial/JsonArchive.hpp"

namespace Pixf::Core::Entities {
    void WorldManager::SaveWorld(const size_t worldIndex, const std::string &filepath) {
        if (worldIndex >= m_Worlds.size()) {
            PIXF_CORE_LOG_ERROR("Failed to save world: Index {} out of range", worldIndex);
            return;
        }

        Serial::JsonOutputArchive archive;
        archive("world", m_Worlds.at(worldIndex));

        Files::WriteFile(filepath, archive.Get().ToString());
    }

    size_t WorldManager::LoadWorld(Blueprint blueprint, const std::string &worldFile) {
        m_Worlds.push_back({});
        blueprint.Get()(m_Worlds.back());

        Serial::JsonInputArchive archive(Json::Json::Parse(Files::ReadFile(worldFile)));
        archive("world", m_Worlds.back());

        return m_Worlds.size() - 1;
    }

    void WorldManager::SetActive(size_t worldIndex) { m_CurrentWorld = worldIndex; }
    std::optional<size_t> WorldManager::GetActiveIndex() const { return m_CurrentWorld; }

    World &WorldManager::GetAt(const size_t worldIndex) {
        return m_Worlds.at(worldIndex);
    }

    World &WorldManager::GetActiveWorld() {
        return GetAt(GetActiveIndex().value());
    }
} // namespace Pixf::Core::Entities
