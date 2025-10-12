#pragma once

#include "entity_manager.h"
#include "event/event.h"
#include "systems_manager.h"
#include "ui/render_window.h"

namespace pixf {

struct InstanceConfig {
  ui::WindowConfig window_config;
};

class QuitEvent final : public event::Event {
 public:
  ~QuitEvent() override = default;
};

class Instance final : event::EventListener {
 public:
  explicit Instance(const InstanceConfig& config,
                    void (*on_init)(EntityManager&, SystemsManager&) = nullptr);
  ~Instance() override = default;

  EntityManager& GetEntityManager();
  void Run();

  template <typename T>
  void AddSystem() {
    systems_manager_.AddSystem<T>();
  }

  void OnEvent(std::shared_ptr<event::Event> event) override;

 private:
  EntityManager entity_manager_;
  ui::RenderWindow window_;
  InstanceConfig config_;
  SystemsManager systems_manager_;
  void (*on_init_)(EntityManager&, SystemsManager&) = nullptr;

  void Init();
  void Update();
  void Terminate();
};

}  // namespace pixf