#include "Pixf.hpp"

using namespace Pixf::Core;
using namespace Pixf::Core::Graphics;
using namespace Pixf::Core::Entities;
using namespace Pixf::Core::Entities::Components;
using namespace Pixf::Core::Entities::Components::Graphics;

class App final : public Application {
public:
    explicit App() : Application({}) {}

    void OnAwake() override {
        World world;
        EntityManager &entityManager = world.GetEntityManager();
        const Entity backpack = world.GetEntityManager().CreateEntity();

        Transform transform{};
        transform.Translate(vec3(0.0F, -0.5F, 6.0F));
        entityManager.AddComponent<Transform>(backpack, transform);

        const ModelHandle model = GetRenderer().GetResourceManager().ImportModel("backpack.obj").Unwrap();
        entityManager.AddComponent<ModelRenderer>(backpack, ModelRenderer(model));

        Camera camera{};
        camera.aspect = 800.0f / 600.0f;
        camera.fov = 60.0f;
        camera.type = CameraType::Perspective;

        entityManager.CreateSingleton<Camera>(camera);
        entityManager.CreateSingleton<AmbientLight>();

        entityManager.CreateEntityWithComponent<PointLight>(
                PointLight(vec3(2.0F, 0.0F, 4.0F), vec3(1.0F, 0.5F, 0.0F), 1.0F, 0.1F, 0.032F));

        entityManager.CreateEntityWithComponent<PointLight>(
                PointLight(vec3(-2.0F, 0.0F, 4.0F), vec3(0.0F, 0.5F, 1.0F), 1.0F, 0.1F, 0.032F));

        GetWorldManager().CreateWorld("1", world);
        GetWorldManager().SetActiveWorld("1");

        const Audio::AudioClipHandle sound = GetAudioManager().ImportAudioClip("sound.wav").Unwrap();
        GetAudioManager().PlayAudioClip(sound, {.loop = true});
    }

    void OnUpdate(const double deltaTime) override {
        EntityManager &entityManager = GetWorldManager().GetActiveWorld().Unwrap()->GetEntityManager();

        for (auto query = entityManager.Query<Transform>().Unwrap(); auto &[id, comp]: query) {
            comp->RotateAround(vec3(0.0F, 1.0F, 0.0F), 60.0F * deltaTime);
        }

        if (GetInputManager().IsKeyDown(Input::Key::Escape)) {
            Exit();
        }

        if (GetInputManager().IsKeyDown(Input::Key::W)) {
            entityManager.GetSingleton<Camera>().Unwrap()->transform.Translate(vec3(0.0F, 0.0F, 20.0F * deltaTime));
        }

        if (GetInputManager().IsKeyDown(Input::Key::S)) {
            entityManager.GetSingleton<Camera>().Unwrap()->transform.Translate(vec3(0.0F, 0.0F, -20.0F * deltaTime));
        }

        if (GetInputManager().IsKeyDown(Input::Key::D)) {
            entityManager.GetSingleton<Camera>().Unwrap()->transform.Translate(vec3(20.0F * deltaTime, 0.0F, 0.0F));
        }

        if (GetInputManager().IsKeyDown(Input::Key::A)) {
            entityManager.GetSingleton<Camera>().Unwrap()->transform.Translate(vec3(-20.0F * deltaTime, 0.0F, 0.0F));
        }

        GetWindow().SetTitle(std::string("OpenGL") + reinterpret_cast<const char *>(glGetString(GL_VERSION)) +
                             ", FPS: " + std::to_string(static_cast<int>(1.0 / deltaTime)));
    }
};

RUN_APPLICATION(App);
