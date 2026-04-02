#ifndef FLK_FILEIO_WORLD_HPP
#define FLK_FILEIO_WORLD_HPP

#include <filesystem>
#include <optional>

#include "Ecs/World.hpp"

namespace Flock::FileIo {
    std::optional<Ecs::World> ReadWorld(const std::filesystem::path &filePath);
    bool                      WriteWorld(const std::filesystem::path &filePath, Ecs::World &world);
}

#endif //FLK_FILEIO_WORLD_HPP
