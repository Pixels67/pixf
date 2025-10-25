#ifndef WORLDMANAGER_HPP
#define WORLDMANAGER_HPP

#include <string>
#include <unordered_map>

#include "Entities/World.hpp"
#include "Error/Result.hpp"

namespace Pixf::Core {
    enum class WorldError : uint8_t {
        None = 0,
        WorldNotFound,
        NoActiveWorld,
    };

    class WorldManager {
    public:
        WorldManager() = default;

        WorldManager(const WorldManager &) = delete;
        WorldManager(WorldManager &&) = delete;
        WorldManager &operator=(const WorldManager &) = delete;
        WorldManager &operator=(WorldManager &&) = delete;

        ~WorldManager() = default;

        void CreateWorld(const std::string &name, const Entities::World &world);
        Error::Result<std::shared_ptr<Entities::World>, WorldError> GetWorld(const std::string &name);
        void DeleteWorld(const std::string &name);

        WorldError SetActiveWorld(const std::string &name);
        Error::Result<std::shared_ptr<Entities::World>, WorldError> GetActiveWorld();

        void Clear();

    private:
        std::unordered_map<std::string, std::shared_ptr<Entities::World>> m_Worlds;
        std::string m_ActiveWorld;
    };

} // namespace Pixf::Core

#endif // WORLDMANAGER_HPP
