#ifndef GUIMANAGER_HPP
#define GUIMANAGER_HPP

#include <imgui.h>

#include "Graphics/Gl/Window.hpp"

namespace Pixf::Core::Gui {
    void Init(Event::EventManager &eventManager);
    void Terminate();

    void BeginRenderGui(double deltaTime);
    void EndRenderGui();
} // namespace Pixf::Core::Gui

#endif // GUIMANAGER_HPP
