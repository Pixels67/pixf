#include "Pixf.hpp"

using namespace Pixf::Core;
using namespace Pixf::Core::Graphics;
using namespace Pixf::Core::Audio;
using namespace Pixf::Core::Entities;
using namespace Pixf::Core::Entities::Components;
using namespace Pixf::Core::Entities::Components::Graphics;
using namespace Pixf::Core::Entities::Components::Audio;

struct AudioPlayer final : System {
    void OnAwake(World &world) override {
        world.GetEntityManager().ForEach<AudioSource>([&](const AudioSource &comp) {
            world.GetAssetManager().GetAudioClip(comp.clip).Unwrap()->Play(comp.config);
        });
    }
};

struct CameraController final : System {
    void OnAwake(World &world) override {
        world.GetEventManager().Subscribe<WindowSizeChangedEvent>([&](const WindowSizeChangedEvent &event) {
            world.GetEntityManager().GetSingleton<Camera>().UnwrapOr({})->aspect =
                    static_cast<float>(event.newWidth) / static_cast<float>(event.newHeight);
        });
    }

    void OnUpdate(World &world, const double deltaTime) override {
        EntityManager &entityManager = world.GetEntityManager();

        if (world.GetInputManager().IsKeyDown(Input::Key::W)) {
            entityManager.GetSingleton<Camera>().Unwrap()->transform.Translate(vec3(0.0F, 0.0F, 20.0F * deltaTime));
        }

        if (world.GetInputManager().IsKeyDown(Input::Key::S)) {
            entityManager.GetSingleton<Camera>().Unwrap()->transform.Translate(vec3(0.0F, 0.0F, -20.0F * deltaTime));
        }

        if (world.GetInputManager().IsKeyDown(Input::Key::D)) {
            entityManager.GetSingleton<Camera>().Unwrap()->transform.Translate(vec3(20.0F * deltaTime, 0.0F, 0.0F));
        }

        if (world.GetInputManager().IsKeyDown(Input::Key::A)) {
            entityManager.GetSingleton<Camera>().Unwrap()->transform.Translate(vec3(-20.0F * deltaTime, 0.0F, 0.0F));
        }
    }
};

struct Backpack final : Component, Serialization::Serializable {
    PIXF_SERIALIZABLE(Backpack)

    Json::object Serialize() override { return Json::object{}; }
    void Deserialize(const Json::object &json, Assets::AssetManager &assetManager) override {}
};

class App final : public Application {
public:
    Blueprint blueprint{};
    Transform transform{};
    vec3 rotation{};

    explicit App() :
        Application({.windowConfig = {.title = "Title", .size = uvec2(1080, 720)},
                     .rendererConfig = {.viewportOrigin = ivec2(0, 0), .viewportAspect = ivec2(1080, 720)}}) {}

    void OnAwake() override {
        Debug::Logger::Init({.logLevel = Debug::Severity::Trace});

        blueprint.Configure([](EntityManager &entityManager, SystemsManager &systemsManager) {
            entityManager.RegisterComponent<Transform>();
            entityManager.RegisterComponent<ModelRenderer>();
            entityManager.RegisterComponent<PointLight>();
            entityManager.RegisterComponent<AudioSource>();
            entityManager.RegisterComponent<Backpack>();

            Camera camera{};
            camera.aspect = 1080.0F / 720.0F;
            camera.fov = 60.0F;
            camera.type = CameraType::Perspective;

            entityManager.CreateSingleton<Camera>(camera);
            entityManager.CreateSingleton<AmbientLight>();

            systemsManager.AddSystem<CameraController>();
            systemsManager.AddSystem<AudioPlayer>();
        });

        GetEventManager().Subscribe<Input::KeyEvent>([&](auto event) {
            if (event.action != Input::KeyAction::Press)
                return;

            if (event.key == Input::Key::Escape) {
                Exit();
            }

            if (event.key == Input::Key::T) {
                GetWorldManager().SaveWorld("world.json", "world");
            }

            if (event.key == Input::Key::R) {
                GetWorldManager().LoadWorld("world.json", "world", blueprint);
                GetWorldManager().SetActiveWorld("world");
            }
        });
    }

    void OnUpdate(double deltaTime) override {
        if (const auto activeWorld = GetWorldManager().GetActiveWorld(); activeWorld.IsSuccess()) {
            auto entityManager = activeWorld.Unwrap()->GetEntityManager();
            entityManager.ForEachEntity<Backpack>([&](const Entity &entity, Backpack &backpack) {
                *entityManager.GetComponent<Transform>(entity).Unwrap() = transform;
            });
        }
    }

    void OnRenderGui(const double deltaTime) override {
        Gui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
        Gui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_Always);

        Gui::Begin("Debug", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
        Gui::Text("FPS: %d", static_cast<int>(1.0 / deltaTime));

        Gui::DragFloat3("Position", &transform.position.x, 0.05F);

        Gui::DragFloat3("Rotation", &rotation.x, 1.0F);
        transform.rotation = quat(radians(rotation));

        Gui::DragFloat3("Scale", &transform.scale.x, 0.05F);

        Gui::End();
    }
};

PIXF_RUN_APPLICATION(App);
