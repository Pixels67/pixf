#ifndef PIXF_GUI_HPP
#define PIXF_GUI_HPP

#include <fmt/format.h>
#include <imgui.h>

#include "Common.hpp"
#include "Math/Color.hpp"
#include "Math/Vector.hpp"
#include "Rect.hpp"

namespace Pixf::Core::Gui {
    PIXF_API void Initialize();
    PIXF_API void Terminate();

    PIXF_API void BeginRenderGui(double deltaTime);
    PIXF_API void EndRenderGui();

    PIXF_API void BeginWindow(const std::string &label, const Rect &rect);
    PIXF_API void EndWindow();

    PIXF_API void BeginChild(const std::string &label, Math::Vector2u size);
    PIXF_API void EndChild();

    template<typename... Args>
    void Text(const std::string &format, const Args &...args) {
        const std::string formatted = fmt::format(fmt::runtime(format), args...);
        ImGui::Text(formatted.c_str());
    }

    template<typename... Args>
    void ColoredText(Math::Color4u8 color, const std::string &format, const Args &...args) {
        const std::string formatted = fmt::format(fmt::runtime(format), args...);
        const Math::Vector4f colorVec = color.ToVector();
        ImGui::TextColored(ImVec4(colorVec.x, colorVec.y, colorVec.z, colorVec.w), formatted.c_str());
    }

    template<typename... Args>
    bool SelectableText(const std::string &format, const bool selected, const Args &...args) {
        const std::string formatted = fmt::format(fmt::runtime(format), args...);
        return ImGui::Selectable(formatted.c_str(), selected);
    }

    PIXF_API bool Button(const std::string &label);
    PIXF_API bool Checkbox(const std::string &label, bool *enabled = nullptr);

    PIXF_API bool InputText(const std::string &label, std::string &outputString, unsigned int maxCharCount = 256);

    PIXF_API void InputInt(const std::string &label, int &number);
    PIXF_API void InputFloat(const std::string &label, float &number);

    PIXF_API void DragInt(const std::string &label, int &number);
    PIXF_API void DragFloat(const std::string &label, float &number);

    PIXF_API void DragVector3f(const std::string &label, Math::Vector3f &vector);
    PIXF_API void DragVector3i(const std::string &label, Math::Vector3i &vector);

    PIXF_API void ColorEditor(const std::string &label, Math::Color3u8 &color);
    PIXF_API void ColorEditor(const std::string &label, Math::Color4u8 &color);

} // namespace Pixf::Core::Gui

#endif // PIXF_GUI_HPP
