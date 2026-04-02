#include "World.hpp"

#include "File.hpp"
#include "Serial/JsonArchive.hpp"

namespace Flock::FileIo {
    std::optional<Ecs::World> ReadWorld(const std::filesystem::path &filePath) {
        using namespace Ecs;

        const auto result = ReadText(filePath);
        if (!result) {
            return std::nullopt;
        }

        const std::string file = result.value();
        if (!Serial::Json::Parse(file)) {
            return std::nullopt;
        }

        const Serial::Json json = Serial::Json::Parse(file).value();
        Serial::JsonReader reader(json);

        World world = World::Default();
        world.Archive(reader);

        return world;
    }

    bool WriteWorld(const std::filesystem::path &filePath, Ecs::World &world) {
        Serial::JsonWriter writer;
        world.Archive(writer);

        const Serial::Json json = writer.GetOutput();

        return WriteText(filePath, json.ToString());
    }
}
