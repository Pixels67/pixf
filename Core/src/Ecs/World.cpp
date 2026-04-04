#include "World.hpp"
#include "FileIo/World.hpp"

#include "App.hpp"
#include "Asset/Assets.hpp"
#include "Audio/AudioListener.hpp"
#include "Audio/AudioSource.hpp"
#include "Event/EventRegistry.hpp"
#include "Graphics/Camera.hpp"
#include "Graphics/Light.hpp"
#include "Graphics/ModelRenderer.hpp"
#include "Graphics/Skybox.hpp"
#include "Graphics/SpriteRenderer.hpp"
#include "Gui/Button.hpp"
#include "Gui/Image.hpp"
#include "Gui/Text.hpp"
#include "Input/Input.hpp"
#include "Math/Transform.hpp"
#include "Physics/Collider.hpp"
#include "Physics/RigidBody.hpp"
#include "Time/Time.hpp"

namespace Flock::Ecs {
    World World::Default() {
        World world;
        world.SetDefaults();

        return world;
    }

    void World::SetDefaults() {
        m_Resources.clear();
        m_Registry.Clear();

        InsertResource<Time::TimeState>();
        InsertResource<Input::InputState>();
        InsertResource<Graphics::Camera>();
        InsertResource<Graphics::AmbientLight>();
        InsertResource<Graphics::Skybox>();
        InsertResource<Audio::AudioListener>();
        InsertResource<Application>();
        InsertResource<Event::EventRegistry>();

        GetRegistry().RegisterComponent<Transform>();
        GetRegistry().RegisterComponent<RigidTransform>();
        GetRegistry().RegisterComponent<Gui::RectTransform>();
        GetRegistry().RegisterComponent<Graphics::SpriteRenderer>();
        GetRegistry().RegisterComponent<Graphics::ModelRenderer>();
        GetRegistry().RegisterComponent<Physics::BoxCollider>();
        GetRegistry().RegisterComponent<Physics::SphereCollider>();
        GetRegistry().RegisterComponent<Physics::RigidBody>();
        GetRegistry().RegisterComponent<Audio::AudioSource>();
        GetRegistry().RegisterComponent<Gui::Text>();
        GetRegistry().RegisterComponent<Gui::Button>();
        GetRegistry().RegisterComponent<Gui::Image>();
    }

    Registry &World::GetRegistry() {
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
        if (!FileIo::ReadWorld(filePath)) {
            return false;
        }

        Asset::AssetLoader *loader = nullptr;
        if (HasResource<Asset::Assets>()) {
            loader = &GetResource<Asset::Assets>().loader;
        }

        *this = FileIo::ReadWorld(filePath).value();

        if (loader) {
            InsertResource(Asset::Assets{*loader});
        }

        return true;
    }

    bool World::Save(const std::filesystem::path &filePath) {
        return FileIo::WriteWorld(filePath, *this);
    }
}
