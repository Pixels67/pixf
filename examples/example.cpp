#include "pixf.h"

using namespace pixf;
using namespace pixf::graphics;
using namespace pixf::core;
using namespace pixf::ui;
using namespace pixf::graphics::lighting;
using namespace pixf::event;

constexpr float MOVE_SPEED = 8.0F;
constexpr double ROT_SPEED = 14.0;
struct InputSystem final : System {
  void OnInit(EntityManager& entity_manager) override {}

  void OnUpdate(EntityManager& entity_manager, const double delta_time) override {
    static double v_angle = 0.0;

    constexpr auto right = glm::vec3(1.0F, 0.0F, 0.0F);
    constexpr auto up = glm::vec3(0.0F, 1.0F, 0.0F);
    constexpr auto forward = glm::vec3(0.0F, 0.0F, 1.0F);

    auto& cam_trans = entity_manager.GetSingleton<Camera>()->transform;

    const input::InputManager& input_manager = *entity_manager.GetSingleton<input::InputManager>();

    if (input_manager.IsKeyDown(input::Key::ESCAPE)) {
      entity_manager.GetSingleton<EventManager>()->DispatchEvent(std::make_shared<QuitEvent>());
    }

    if (input_manager.IsKeyDown(input::Key::W)) {
      const auto cam = entity_manager.GetSingleton<Camera>();
      cam_trans.position +=
          MOVE_SPEED * static_cast<float>(delta_time) * normalize(cam_trans.rotation * forward);
    }

    if (input_manager.IsKeyDown(input::Key::S)) {
      const auto cam = entity_manager.GetSingleton<Camera>();
      cam_trans.position -=
          MOVE_SPEED * static_cast<float>(delta_time) * normalize(cam_trans.rotation * forward);
    }

    if (input_manager.IsKeyDown(input::Key::D)) {
      const auto cam = entity_manager.GetSingleton<Camera>();
      cam_trans.position +=
          MOVE_SPEED * static_cast<float>(delta_time) * normalize(cam_trans.rotation * right);
    }

    if (input_manager.IsKeyDown(input::Key::A)) {
      const auto cam = entity_manager.GetSingleton<Camera>();
      cam_trans.position -=
          MOVE_SPEED * static_cast<float>(delta_time) * normalize(cam_trans.rotation * right);
    }

    if (input_manager.IsKeyDown(input::Key::L_SHIFT)) {
      const auto cam = entity_manager.GetSingleton<Camera>();
      cam_trans.position +=
          MOVE_SPEED * static_cast<float>(delta_time) * normalize(cam_trans.rotation * up);
    }

    if (input_manager.IsKeyDown(input::Key::L_CTRL)) {
      const auto cam = entity_manager.GetSingleton<Camera>();
      cam_trans.position -=
          MOVE_SPEED * static_cast<float>(delta_time) * normalize(cam_trans.rotation * up);
    }

    const glm::dvec2 mouse_delta = input_manager.GetMouseDelta();
    cam_trans.Rotate(mouse_delta.x * ROT_SPEED * delta_time, up);

    if (const double delta_v_angle = mouse_delta.y * ROT_SPEED * delta_time;
        v_angle + delta_v_angle > -70.0 && v_angle + delta_v_angle < 90.0) {
      cam_trans.Rotate(delta_v_angle, cam_trans.rotation * right);
      v_angle += delta_v_angle;
    }
  }
};

void Init(EntityManager& entity_manager, SystemsManager& systems_manager) {
  auto& resource_manager = *entity_manager.GetSingleton<ResourceManager>();

  Camera camera;
  camera.bg_color = glm::vec3(0.15F, 0.15F, 0.15F);
  camera.viewport_size = glm::vec2(800, 600);
  entity_manager.CreateSingleton<Camera>(camera);

  PointLight point_light;

  // Light 1
  point_light.position = glm::vec3(4.0F, -2.0F, 8.0F);
  point_light.color = glm::vec3(0.5F, 0.4F, 0.3F);
  point_light.intensity = 1.0F;
  entity_manager.AddComponentToEntity<PointLight>(entity_manager.CreateEntity(), point_light);

  // Light 2
  point_light.position = glm::vec3(-4.0F, 0.0F, 4.0F);
  point_light.color = glm::vec3(0.0F, 0.1F, 0.2F);
  point_light.intensity = 0.5F;
  entity_manager.AddComponentToEntity<PointLight>(entity_manager.CreateEntity(), point_light);

  ModelRenderer object{};
  object.model = Model{"backpack.obj", resource_manager};
  object.materials = Material::LoadFromModel("backpack.obj", resource_manager);

  Transform transform;
  transform.position = glm::vec3(0.0F, -1.2F, 4.0F);
  Entity entity = entity_manager.CreateEntity();

  entity_manager.AddComponentToEntity(entity, transform);
  entity_manager.AddComponentToEntity(entity, object);

  systems_manager.AddSystem<InputSystem>();
}

const auto CONFIG = InstanceConfig{{
    .title = "Hello Backpack!",
    .width = 800,
    .height = 600,
    .cursor_mode = CursorMode::DISABLED,
    .v_sync = true,
}};

INIT(CONFIG, Init)