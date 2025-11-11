#ifndef GUIMANAGER_HPP
#define GUIMANAGER_HPP

#include <imgui.h>

#include "Window.hpp"

namespace Pixf::Core::Gui {
    using namespace ImGui;

    void Init(Event::EventManager &eventManager);
    void Terminate();

    void BeginRenderGui(double deltaTime);
    void EndRenderGui();
} // namespace Pixf::Core::Gui

#endif // GUIMANAGER_HPP
