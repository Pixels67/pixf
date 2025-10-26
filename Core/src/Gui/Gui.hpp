#ifndef GUIMANAGER_HPP
#define GUIMANAGER_HPP

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "Graphics/Gl/Window.hpp"

namespace Pixf::Core::Gui {
    void Init(const Graphics::Gl::Window &window);
    void Terminate();

    void BeginRenderGui();
    void EndRenderGui();
} // namespace Pixf::Core::Gui

#endif // GUIMANAGER_HPP
