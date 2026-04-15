#include "World.hpp"

#include <optional>
#include <string>
#include <string_view>

#include "App.hpp"
#include "Audio/AudioListener.hpp"
#include "Audio/AudioSource.hpp"
#include "Event/EventRegistry.hpp"
#include "FileIo/File.hpp"
#include "Graphics/Camera.hpp"
#include "Graphics/Light.hpp"
#include "Graphics/ModelRenderer.hpp"
#include "Graphics/Skybox.hpp"
#include "Graphics/SpriteRenderer.hpp"
#include "Gui/Box.hpp"
#include "Gui/Button.hpp"
#include "Gui/Image.hpp"
#include "Gui/Text.hpp"
#include "Input/Input.hpp"
#include "Math/Transform.hpp"
#include "Physics/Collider.hpp"
#include "Physics/RigidBody.hpp"
#include "Serial/JsonArchive.hpp"
#include "Time/Time.hpp"
#include "Ecs/Registry.hpp"
#include "Gui/RectTransform.hpp"
#include "Math/RigidTransform.hpp"
#include "Serial/Json.hpp"

namespace Flock::Ecs {
    World World::Default() {
        World world;
        world.SetDefaults();

        return world;
    }

    void World::SetDefaults() {
        InsertResource<Time::TimeState>();
        InsertResource<Input::InputState>();
        InsertResource<Graphics::Camera>();
        InsertResource<Graphics::AmbientLight>();
        InsertResource<Graphics::Skybox>();
        InsertResource<Audio::AudioListener>();
        InsertResource<Application>();
        InsertResource<Event::EventRegistry>();

        Registry().Register<Transform>();
        Registry().Register<RigidTransform>();
        Registry().Register<Gui::RectTransform>();
        Registry().Register<Graphics::SpriteRenderer>();
        Registry().Register<Graphics::ModelRenderer>();
        Registry().Register<Physics::BoxCollider>();
        Registry().Register<Physics::SphereCollider>();
        Registry().Register<Physics::RigidBody>();
        Registry().Register<Audio::AudioSource>();
        Registry().Register<Gui::Text>();
        Registry().Register<Gui::Button>();
        Registry().Register<Gui::Image>();
        Registry().Register<Gui::Box>();
    }

    Registry &World::Registry() {
        return m_Registry;
    }

    void World::Archive(Serial::IArchive &archive) {
        archive.BeginObject("registry");
        m_Registry.Archive(archive);
        archive.EndObject();

        archive.BeginObject("resources");
        for (auto &[typeId, fn]: m_ArchiveFns) {
            if (!m_Resources.contains(typeId)) {
                continue;
            }

            fn(archive, m_Resources.at(typeId));
        }

        archive.EndObject();
    }

    bool World::Load(const std::filesystem::path &filePath) {
        const auto result = FileIo::ReadText(filePath);
        if (!result) {
            return false;
        }

        const std::string file = result.value();
        if (!Serial::Json::Parse(file)) {
            return false;
        }

        const Serial::Json json = Serial::Json::Parse(file).value();
        Serial::JsonReader reader(json);

        Archive(reader);
        return true;
    }

    bool World::Save(const std::filesystem::path &filePath) {
        Serial::JsonWriter writer;
        Archive(writer);

        const Serial::Json json = writer.Output();

        return FileIo::WriteText(filePath, json.ToString());
    }
}
