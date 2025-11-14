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
    PIXF_API Json::value DrawJsonValue(Json::value value);

    PIXF_API Json::value DrawJsonObject(Json::value value, const std::string &name);
    PIXF_API Json::value DrawJsonArray(Json::value value, const std::string &name);
    PIXF_API Json::value DrawJsonString(Json::value value, const std::string &name);
    PIXF_API Json::value DrawJsonFloat(Json::value value, const std::string &name);
    PIXF_API Json::value DrawJsonInt(Json::value value, const std::string &name);
    PIXF_API Json::value DrawJsonUint(Json::value value, const std::string &name);
    PIXF_API Json::value DrawJsonBool(Json::value value, const std::string &name);

    PIXF_API Json::value DrawVec3(Json::value value, const std::string &name);
    PIXF_API Json::value DrawVec4(Json::value value, const std::string &name);
    PIXF_API Json::value DrawQuat(Json::value value, const std::string &name);
    PIXF_API Json::value DrawRgbColor(Json::value value, const std::string &name);
    PIXF_API Json::value DrawRgbaColor(Json::value value, const std::string &name);

    PIXF_API std::optional<std::filesystem::path> DrawDirectoryRecursive(const std::filesystem::path &path);
} // namespace Pixf::Core::Gui

#endif // GUIMANAGER_HPP
