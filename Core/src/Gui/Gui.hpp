#ifndef GUIMANAGER_HPP
#define GUIMANAGER_HPP

#include <filesystem>
#include <optional>

#include <imgui.h>

#include "Serialization/Json/Json.hpp"
#include "Window.hpp"

namespace Pixf::Core::Gui {
    using namespace ImGui;

    PIXF_API void Init(Event::EventManager &eventManager);
    PIXF_API void Terminate();

    PIXF_API void BeginRenderGui(double deltaTime);
    PIXF_API void EndRenderGui();

    PIXF_API Serialization::Json::value DrawJsonValue(Serialization::Json::value value, const std::string &name);
    PIXF_API Serialization::Json::value DrawJsonValue(Serialization::Json::value value);

    PIXF_API Serialization::Json::value DrawJsonObject(Serialization::Json::value value, const std::string &name);
    PIXF_API Serialization::Json::value DrawJsonArray(Serialization::Json::value value, const std::string &name);
    PIXF_API Serialization::Json::value DrawJsonString(Serialization::Json::value value, const std::string &name);
    PIXF_API Serialization::Json::value DrawJsonFloat(Serialization::Json::value value, const std::string &name);
    PIXF_API Serialization::Json::value DrawJsonInt(Serialization::Json::value value, const std::string &name);
    PIXF_API Serialization::Json::value DrawJsonUint(Serialization::Json::value value, const std::string &name);
    PIXF_API Serialization::Json::value DrawJsonBool(Serialization::Json::value value, const std::string &name);

    PIXF_API Serialization::Json::value DrawVec3(Serialization::Json::value value, const std::string &name);
    PIXF_API Serialization::Json::value DrawVec4(Serialization::Json::value value, const std::string &name);
    PIXF_API Serialization::Json::value DrawColor3(Serialization::Json::value value, const std::string &name);
    PIXF_API Serialization::Json::value DrawColor4(Serialization::Json::value value, const std::string &name);

    PIXF_API std::optional<std::filesystem::path> DrawDirectoryRecursive(const std::filesystem::path &path);
} // namespace Pixf::Core::Gui

#endif // GUIMANAGER_HPP
