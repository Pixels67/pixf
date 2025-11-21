#include "System.hpp"

namespace Pixf::Core::Entities {
    void System::Awake(Registry &registry) {}
    void System::Start(Registry &registry) {}
    void System::Update(Registry &registry, double deltaTime) {}
    void System::Render(Registry &registry, double deltaTime) {}
    void System::RenderGui(Registry &registry, double deltaTime) {}
    void System::OnEvent(Registry &registry, Event::Event &event) {}
} // namespace Pixf::Core::Entities
