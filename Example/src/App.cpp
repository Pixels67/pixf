#include "File/File.hpp"
#include "Json/Json.hpp"
#include "Pixf.hpp"

using namespace Pixf::Core;
using namespace Pixf::Core::Graphics;
using namespace Pixf::Core::Entities;
using namespace Pixf::Core::Entities::Components;
using namespace Pixf::Core::Entities::Components::Graphics;

struct MusicPlayer final : System {
    void OnAwake(World &world) override {
        for (auto query = world.GetEntityManager().Query<Components::Audio::AudioSource>().Unwrap();
             auto &[id, comp]: query) {
            world.GetAudioManager().PlayAudioClip(comp->clip, comp->config);
        }
    }
};

struct CameraController final : System {
    void OnAwake(World &world) override {
        world.GetEventManager().Subscribe<Gl::WindowSizeChangedEvent>([&](const Gl::WindowSizeChangedEvent &event) {
            world.GetEntityManager().GetSingleton<Camera>().Unwrap()->aspect =
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

class App final : public Application {
public:
    explicit App() :
        Application({.windowConfig = {.title = "Title", .size = uvec2(1080, 720)},
                     .rendererConfig = {.viewportOrigin = ivec2(0, 0), .viewportAspect = ivec2(1080, 720)}}) {}

    void OnAwake() override {
        PIXF_LOG_INFO("Using OpenGL ", glGetString(GL_VERSION));

        World world(*this);

        EntityManager &entityManager = world.GetEntityManager();
        backpack = world.GetEntityManager().CreateEntity();

        world.GetEntityManager().CreateEntity();
        world.GetEntityManager().CreateEntity();

        Camera camera{};
        camera.aspect = 1080.0F / 720.0F;
        camera.fov = 60.0F;
        camera.type = CameraType::Perspective;

        entityManager.CreateSingleton<Camera>(camera);
        entityManager.CreateSingleton<AmbientLight>();

        //const Entity audioSource = entityManager.CreateEntityWithComponent<Components::Audio::AudioSource>();
        //entityManager.GetComponent<Components::Audio::AudioSource>(audioSource).Unwrap()->clip =
        //        world.GetAudioManager().ImportAudioClip("Assets/sound.wav").Unwrap();

        //world.GetSystemsManager().AddSystem<MusicPlayer>();

        world.GetSystemsManager().AddSystem<CameraController>();

        world.GetEntityManager().RegisterComponent<Transform>();
        world.GetEntityManager().RegisterComponent<ModelRenderer>();
        world.GetEntityManager().RegisterComponent<PointLight>();

        GetWorldManager().CreateWorld("1", world);
        GetWorldManager().SetActiveWorld("1");

        GetEventManager().Subscribe<Input::KeyEvent>([&](auto event) {
            if (event.action != Input::KeyAction::Press)
                return;

            if (event.key == Input::Key::Escape) {
                Exit();
            }

            if (event.key == Input::Key::T) {
                auto &em = GetWorldManager().GetActiveWorld().Unwrap()->GetEntityManager();
                const auto comp = em.GetComponent<Transform>(backpack).Unwrap();

                comp->rotation = quat(vec3(radians(rot.x), radians(rot.y), radians(rot.z)));
                comp->position = pos;
                comp->scale = scale;

                auto &comps = em.GetComponentManager();
                const Json::object json = comps.Serialize();
                File::WriteFile("components.json", Json::ToPrettyJson(json));
            }

            if (event.key == Input::Key::R) {
                auto &em = GetWorldManager().GetActiveWorld().Unwrap()->GetEntityManager();

                const std::string str = File::ReadFile("components.json").Unwrap();
                const Json::object json = Json::parse(str).as_object();

                auto &comps = em.GetComponentManager();
                comps.Deserialize(json, GetAssetManager());
            }
        });
    }

    void OnRenderGui(const double deltaTime) override {
        ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_Always);

        ImGui::Begin("Debug", nullptr,
                     ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
        ImGui::Text("FPS: %d", static_cast<int>(1.0 / deltaTime));

        ImGui::DragFloat3("Position", &pos.x, 0.1F, 0, 0, "%.1f");
        ImGui::DragFloat3("Rotation", &rot.x, 0.1F, 0, 0, "%.1f");
        ImGui::DragFloat3("Scale", &scale.x, 0.1F, 0, 0, "%.1f");

        ImGui::End();
    }

    vec3 pos = vec3();
    vec3 rot = vec3();
    vec3 scale = vec3(1.0F);
    Assets::AssetHandle model;
    Entity backpack = Entity{};
};

PIXF_RUN_APPLICATION(App);
