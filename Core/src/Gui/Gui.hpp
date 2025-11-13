#ifndef GUIMANAGER_HPP
#define GUIMANAGER_HPP

#include <filesystem>
#include <optional>

#include <imgui.h>

#include "Json/Json.hpp"
#include "Window.hpp"

namespace Pixf::Core::Gui {
    using namespace ImGui;

    PIXF_API void Init(Event::EventManager &eventManager);
    PIXF_API void Terminate();

    PIXF_API void BeginRenderGui(double deltaTime);
    PIXF_API void EndRenderGui();
    PIXF_API Json::value DrawJsonValue(Json::value value, const std::string &name);
    PIXF_API std::optional<std::filesystem::path> DrawDirectoryRecursive(const std::filesystem::path &path,
                                                                         const std::filesystem::path &selected);
} // namespace Pixf::Core::Gui

#endif // GUIMANAGER_HPP
