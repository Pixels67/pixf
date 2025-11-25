#ifndef PIXF_WORLDMANAGER_HPP
#define PIXF_WORLDMANAGER_HPP

#include "Blueprint.hpp"
#include "World.hpp"
#include "Common.hpp"

namespace Pixf::Core::Entities {
    class PIXF_API WorldManager {
    public:
        WorldManager() = default;

        WorldManager(const WorldManager &) = delete;
        WorldManager(WorldManager &&) = delete;
        WorldManager &operator=(const WorldManager &) = delete;
        WorldManager &operator=(WorldManager &&) = delete;

        ~WorldManager() = default;

        void SaveWorld(size_t worldIndex, const std::string &filepath);
        size_t LoadWorld(Blueprint blueprint, const std::string &worldFile);

        void SetActive(size_t worldIndex);
        std::optional<size_t> GetActiveIndex() const;

        World &GetAt(size_t worldIndex);
        World &GetActiveWorld();

    private:
        std::vector<World> m_Worlds;
        std::optional<size_t> m_CurrentWorld;
    };
} // namespace Pixf::Core::Entities

#endif // PIXF_WORLDMANAGER_HPP
