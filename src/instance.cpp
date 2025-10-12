#include "instance.h"

#include <GLFW/glfw3.h>

#include "event/event.h"
#include "graphics/rendering.h"
#include "graphics/resource_manager.h"
#include "input/input_manager.h"
#include "time/time.h"

namespace pixf {
Instance::Instance(const InstanceConfig& config, void (*on_init)(EntityManager&, SystemsManager&))
    : config_(config), on_init_(on_init) {
  Init();
}

EntityManager& Instance::GetEntityManager() { return entity_manager_; }

void Instance::Run() {
  while (window_.IsRunning()) {
    Update();
  }

  Terminate();
}

void Instance::OnEvent(const std::shared_ptr<event::Event> event) {
  if (std::dynamic_pointer_cast<QuitEvent>(event)) {
    Terminate();
  }
}

void Instance::Init() {
  window_ = ui::RenderWindow(config_.window_config);
  window_.SetTargetWindow();

  entity_manager_.CreateSingleton(graphics::ResourceManager{});
  const auto event_manager = entity_manager_.CreateSingleton(event::EventManager{});
  const auto input_manager = input::InputManager{(event_manager.get()), &window_};
  entity_manager_.CreateSingleton(input_manager);

  event_manager->Subscribe(this);

  if (on_init_ != nullptr) {
    on_init_(entity_manager_, systems_manager_);
  }

  systems_manager_.AddSystem<graphics::RenderSystem>();
  systems_manager_.InitSystems(entity_manager_);
}

void Instance::Update() {
  glfwPollEvents();
  time::SetTime(glfwGetTime());
  entity_manager_.GetSingleton<event::EventManager>()->DispatchEvents();
  entity_manager_.GetSingleton<input::InputManager>()->Update(window_);
  systems_manager_.UpdateSystems(entity_manager_, time::GetDeltaTime());
  window_.SwapBuffers();
}

void Instance::Terminate() {
  const auto event_manager = entity_manager_.GetSingleton<event::EventManager>();
  event_manager->Unsubscribe(this);

  window_.Close();
  glfwTerminate();
}
}  // namespace pixf