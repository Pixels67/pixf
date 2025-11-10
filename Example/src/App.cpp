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
        // for (auto query = world.GetEntityManager().Query<Components::Audio::AudioSource>().Unwrap();
        //      auto &[id, comp]: query) {
        //     world.GetAudioManager().PlayAudioClip(comp->clip, comp->config);
        // }
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
        const Entity backpack = world.GetEntityManager().CreateEntity();

        Transform transform{};
        transform.Translate(vec3(0.0F, -0.5F, 6.0F));
        entityManager.AddComponent<Transform>(backpack, transform);

        model = GetAssetManager().ImportModel("Assets/backpack.obj").Unwrap();
        entityManager.AddComponent<ModelRenderer>(backpack, ModelRenderer(model));

        Camera camera{};
        camera.aspect = 1080.0F / 720.0F;
        camera.fov = 60.0F;
        camera.type = CameraType::Perspective;

        entityManager.CreateSingleton<Camera>(camera);
        entityManager.CreateSingleton<AmbientLight>();

        entityManager.CreateEntityWithComponent<PointLight>(
                PointLight(vec3(2.0F, 0.0F, 4.0F), vec3(1.0F, 0.5F, 0.0F), 1.0F, 0.1F, 0.032F));

        entityManager.CreateEntityWithComponent<PointLight>(
                PointLight(vec3(-2.0F, 0.0F, 4.0F), vec3(0.0F, 0.5F, 1.0F), 1.0F, 0.1F, 0.032F));

        const Entity audioSource = entityManager.CreateEntityWithComponent<Components::Audio::AudioSource>();
        entityManager.GetComponent<Components::Audio::AudioSource>(audioSource).Unwrap()->clip =
                world.GetAudioManager().ImportAudioClip("Assets/sound.wav").Unwrap();

        world.GetSystemsManager().AddSystem<MusicPlayer>();
        world.GetSystemsManager().AddSystem<CameraController>();

        GetWorldManager().CreateWorld("1", world);
        GetWorldManager().SetActiveWorld("1");

        GetEventManager().Subscribe<Input::KeyEvent>([&](auto event) {
            if (event.action != Input::KeyAction::Press) return;

            if (event.key == Input::Key::Escape) {
                Exit();
            }

            if (event.key == Input::Key::T) {
                Transform trans{};
                trans.position = pos;
                trans.rotation = quat(vec3(radians(rot.x), radians(rot.y), radians(rot.z)));
                trans.scale = scale;

                const Json::object json = trans.Serialize();
                File::WriteFile("transform.json", Json::ToPrettyJson(json));
            }

            if (event.key == Input::Key::R) {
                const std::string str = File::ReadFile("transform.json").Unwrap();
                const Json::object json = Json::parse(str).as_object();

                Transform trans{};
                trans.Deserialize(json, GetAssetManager());
                pos = trans.position;
                rot = degrees(eulerAngles(trans.rotation));
                scale = trans.scale;
            }
        });
    }

    void OnUpdate(const double deltaTime) override {
        EntityManager &entityManager = GetWorldManager().GetActiveWorld().Unwrap()->GetEntityManager();

        for (auto query = entityManager.Query<Transform>().Unwrap(); auto &[id, comp]: query) {
            comp->rotation = quat(vec3(radians(rot.x), radians(rot.y), radians(rot.z)));
            comp->position = pos;
            comp->scale = scale;
        }
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
};

PIXF_RUN_APPLICATION(App);
