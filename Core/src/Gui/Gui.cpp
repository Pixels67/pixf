#include "Gui.hpp"

#include <imgui.h>
#include <imgui_impl_opengl3.h>

#include "Input/InputManager.hpp"
#include "Serialization/Serialization.hpp"
#include "Window.hpp"

namespace Pixf::Core::Gui {
    ImGuiKey ToImGuiKey(const Input::Key key) {
        switch (key) {
            case Input::Key::Space:
                return ImGuiKey_Space;
            case Input::Key::Apostrophe:
                return ImGuiKey_Apostrophe;
            case Input::Key::Comma:
                return ImGuiKey_Comma;
            case Input::Key::Minus:
                return ImGuiKey_Minus;
            case Input::Key::Period:
                return ImGuiKey_Period;
            case Input::Key::Slash:
                return ImGuiKey_Slash;
            case Input::Key::Alpha0:
                return ImGuiKey_0;
            case Input::Key::Alpha1:
                return ImGuiKey_1;
            case Input::Key::Alpha2:
                return ImGuiKey_2;
            case Input::Key::Alpha3:
                return ImGuiKey_3;
            case Input::Key::Alpha4:
                return ImGuiKey_4;
            case Input::Key::Alpha5:
                return ImGuiKey_5;
            case Input::Key::Alpha6:
                return ImGuiKey_6;
            case Input::Key::Alpha7:
                return ImGuiKey_7;
            case Input::Key::Alpha8:
                return ImGuiKey_8;
            case Input::Key::Alpha9:
                return ImGuiKey_9;
            case Input::Key::Semicolon:
                return ImGuiKey_Semicolon;
            case Input::Key::Equal:
                return ImGuiKey_Equal;
            case Input::Key::A:
                return ImGuiKey_A;
            case Input::Key::B:
                return ImGuiKey_B;
            case Input::Key::C:
                return ImGuiKey_C;
            case Input::Key::D:
                return ImGuiKey_D;
            case Input::Key::E:
                return ImGuiKey_E;
            case Input::Key::F:
                return ImGuiKey_F;
            case Input::Key::G:
                return ImGuiKey_G;
            case Input::Key::H:
                return ImGuiKey_H;
            case Input::Key::I:
                return ImGuiKey_I;
            case Input::Key::J:
                return ImGuiKey_J;
            case Input::Key::K:
                return ImGuiKey_K;
            case Input::Key::L:
                return ImGuiKey_L;
            case Input::Key::M:
                return ImGuiKey_M;
            case Input::Key::N:
                return ImGuiKey_N;
            case Input::Key::O:
                return ImGuiKey_O;
            case Input::Key::P:
                return ImGuiKey_P;
            case Input::Key::Q:
                return ImGuiKey_Q;
            case Input::Key::R:
                return ImGuiKey_R;
            case Input::Key::S:
                return ImGuiKey_S;
            case Input::Key::T:
                return ImGuiKey_T;
            case Input::Key::U:
                return ImGuiKey_U;
            case Input::Key::V:
                return ImGuiKey_V;
            case Input::Key::W:
                return ImGuiKey_W;
            case Input::Key::X:
                return ImGuiKey_X;
            case Input::Key::Y:
                return ImGuiKey_Y;
            case Input::Key::Z:
                return ImGuiKey_Z;
            case Input::Key::LBracket:
                return ImGuiKey_LeftBracket;
            case Input::Key::Backslash:
                return ImGuiKey_Backslash;
            case Input::Key::RBracket:
                return ImGuiKey_RightBracket;
            case Input::Key::GraveAccent:
                return ImGuiKey_GraveAccent;
            case Input::Key::Escape:
                return ImGuiKey_Escape;
            case Input::Key::Return:
                return ImGuiKey_Enter;
            case Input::Key::Tab:
                return ImGuiKey_Tab;
            case Input::Key::Backspace:
                return ImGuiKey_Backspace;
            case Input::Key::Insert:
                return ImGuiKey_Insert;
            case Input::Key::Delete:
                return ImGuiKey_Delete;
            case Input::Key::Right:
                return ImGuiKey_RightArrow;
            case Input::Key::Left:
                return ImGuiKey_LeftArrow;
            case Input::Key::Down:
                return ImGuiKey_DownArrow;
            case Input::Key::Up:
                return ImGuiKey_UpArrow;
            case Input::Key::PageUp:
                return ImGuiKey_PageUp;
            case Input::Key::PageDown:
                return ImGuiKey_PageDown;
            case Input::Key::Home:
                return ImGuiKey_Home;
            case Input::Key::End:
                return ImGuiKey_End;
            case Input::Key::CapsLock:
                return ImGuiKey_CapsLock;
            case Input::Key::ScrollLock:
                return ImGuiKey_ScrollLock;
            case Input::Key::NumLock:
                return ImGuiKey_NumLock;
            case Input::Key::PrintScreen:
                return ImGuiKey_PrintScreen;
            case Input::Key::Pause:
                return ImGuiKey_Pause;
            case Input::Key::F1:
                return ImGuiKey_F1;
            case Input::Key::F2:
                return ImGuiKey_F2;
            case Input::Key::F3:
                return ImGuiKey_F3;
            case Input::Key::F4:
                return ImGuiKey_F4;
            case Input::Key::F5:
                return ImGuiKey_F5;
            case Input::Key::F6:
                return ImGuiKey_F6;
            case Input::Key::F7:
                return ImGuiKey_F7;
            case Input::Key::F8:
                return ImGuiKey_F8;
            case Input::Key::F9:
                return ImGuiKey_F9;
            case Input::Key::F10:
                return ImGuiKey_F10;
            case Input::Key::F11:
                return ImGuiKey_F11;
            case Input::Key::F12:
                return ImGuiKey_F12;
            case Input::Key::F13:
                return ImGuiKey_F13;
            case Input::Key::F14:
                return ImGuiKey_F14;
            case Input::Key::F15:
                return ImGuiKey_F15;
            case Input::Key::F16:
                return ImGuiKey_F16;
            case Input::Key::F17:
                return ImGuiKey_F17;
            case Input::Key::F18:
                return ImGuiKey_F18;
            case Input::Key::F19:
                return ImGuiKey_F19;
            case Input::Key::F20:
                return ImGuiKey_F20;
            case Input::Key::F21:
                return ImGuiKey_F21;
            case Input::Key::F22:
                return ImGuiKey_F22;
            case Input::Key::F23:
                return ImGuiKey_F23;
            case Input::Key::F24:
                return ImGuiKey_F24;
            case Input::Key::Kp0:
                return ImGuiKey_Keypad0;
            case Input::Key::Kp1:
                return ImGuiKey_Keypad1;
            case Input::Key::Kp2:
                return ImGuiKey_Keypad2;
            case Input::Key::Kp3:
                return ImGuiKey_Keypad3;
            case Input::Key::Kp4:
                return ImGuiKey_Keypad4;
            case Input::Key::Kp5:
                return ImGuiKey_Keypad5;
            case Input::Key::Kp6:
                return ImGuiKey_Keypad6;
            case Input::Key::Kp7:
                return ImGuiKey_Keypad7;
            case Input::Key::Kp8:
                return ImGuiKey_Keypad8;
            case Input::Key::Kp9:
                return ImGuiKey_Keypad9;
            case Input::Key::KpDecimal:
                return ImGuiKey_KeypadDecimal;
            case Input::Key::KpDivide:
                return ImGuiKey_KeypadDivide;
            case Input::Key::KpMultiply:
                return ImGuiKey_KeypadMultiply;
            case Input::Key::KpSubtract:
                return ImGuiKey_KeypadSubtract;
            case Input::Key::KpAdd:
                return ImGuiKey_KeypadAdd;
            case Input::Key::KpReturn:
                return ImGuiKey_KeypadEnter;
            case Input::Key::KpEqual:
                return ImGuiKey_KeypadEqual;
            case Input::Key::LShift:
                return ImGuiKey_LeftShift;
            case Input::Key::LCtrl:
                return ImGuiKey_LeftCtrl;
            case Input::Key::LAlt:
                return ImGuiKey_LeftAlt;
            case Input::Key::LSuper:
                return ImGuiKey_LeftSuper;
            case Input::Key::RShift:
                return ImGuiKey_RightShift;
            case Input::Key::RCtrl:
                return ImGuiKey_RightCtrl;
            case Input::Key::RAlt:
                return ImGuiKey_RightAlt;
            case Input::Key::RSuper:
                return ImGuiKey_RightSuper;
            case Input::Key::Menu:
                return ImGuiKey_Menu;
            default:
                return ImGuiKey_None;
        }
    }

    ImGuiMouseButton ToImGuiMouseKey(const Input::MouseKey key) {
        switch (key) {
            case Input::MouseKey::Mouse1:
                return ImGuiMouseButton_Left;
            case Input::MouseKey::Mouse2:
                return ImGuiMouseButton_Right;
            case Input::MouseKey::Mouse3:
                return ImGuiMouseButton_Middle;
            default:
                return ImGuiMouseButton_Left;
        }
    }

    void Init(Event::EventManager &eventManager) {
        IMGUI_CHECKVERSION();
        CreateContext();
        StyleColorsDark();

        ImGuiIO &io = GetIO();
        io.IniFilename = nullptr;

        eventManager.Subscribe<WindowSizeChangedEvent>([&](const WindowSizeChangedEvent &event) {
            io.DisplaySize.x = event.newWidth;
            io.DisplaySize.y = event.newHeight;
        });

        eventManager.Subscribe<Input::KeyEvent>([&](const Input::KeyEvent &event) {
            io.AddKeyEvent(ToImGuiKey(event.key), event.action == Input::KeyAction::Press);
        });

        eventManager.Subscribe<Input::MouseKeyEvent>([&](const Input::MouseKeyEvent &event) {
            io.AddMouseButtonEvent(ToImGuiMouseKey(event.key), event.action == Input::KeyAction::Press);
        });

        eventManager.Subscribe<Input::MouseMovedEvent>(
                [&](const Input::MouseMovedEvent &event) { io.AddMousePosEvent(event.position.x, event.position.y); });

        eventManager.Subscribe<Input::CharacterEvent>(
                [&](const Input::CharacterEvent &event) { io.AddInputCharacter(event.codepoint); });

        ImGui_ImplOpenGL3_Init("#version 330");
    }

    void Terminate() {
        ImGui_ImplOpenGL3_Shutdown();
        DestroyContext();
    }

    void BeginRenderGui(const double deltaTime) {
        ImGuiIO &io = GetIO();
        io.DeltaTime = deltaTime;

        ImGui_ImplOpenGL3_NewFrame();
        NewFrame();
    }

    void EndRenderGui() {
        Render();
        ImGui_ImplOpenGL3_RenderDrawData(GetDrawData());
    }

    Json::value DrawJsonValue(Json::value value, const std::string &name) {
        using namespace Json;

        if (value.is_object()) {
            return DrawJsonObject(value, name);
        }

        if (value.is_array()) {
            return DrawJsonArray(value, name);
        }

        if (value.is_string()) {
            return DrawJsonString(value, name);
        }

        if (value.is_double()) {
            return DrawJsonFloat(value, name);
        }

        if (value.is_int64()) {
            return DrawJsonInt(value, name);
        }

        if (value.is_uint64()) {
            return DrawJsonUint(value, name);
        }

        if (value.is_bool()) {
            return DrawJsonBool(value, name);
        }

        return value;
    }

    Json::value DrawJsonValue(Json::value value) {
        if (value.is_object()) {
            for (auto &[key, val]: value.as_object()) {
                value.as_object()[key] = DrawJsonValue(val, key);
            }
        } else if (value.is_array()) {
            size_t i = 0;
            for (const auto val: value.as_array()) {
                value.as_array()[i] = DrawJsonValue(val, std::to_string(i));
                i++;
            }
        }

        return value;
    }

    Json::value DrawJsonObject(Json::value value, const std::string &name) {
        if (!value.is_object()) {
            return value;
        }

        Json::object obj = value.as_object();

        if (obj.contains("vec3")) {
            return DrawVec3(obj, name);
        }

        if (obj.contains("vec4")) {
            return DrawVec4(obj, name);
        }

        if (obj.contains("quat")) {
            return DrawQuat(obj, name);
        }

        if (obj.contains("rgb")) {
            return DrawRgbColor(obj, name);
        }

        if (obj.contains("rgba")) {
            return DrawRgbaColor(obj, name);
        }

        if (TreeNodeEx(name.c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
            for (auto &[key, val]: obj) {
                obj[key] = DrawJsonValue(val, key);
            }

            TreePop();
        }

        return value_from(obj);
    }

    Json::value DrawJsonArray(Json::value value, const std::string &name) {
        if (!value.is_array()) {
            return value;
        }

        Json::array arr = value.as_array();
        if (TreeNode((name + " []").c_str())) {
            for (size_t i = 0; i < arr.size(); ++i) {
                arr[i] = DrawJsonValue(arr[i], std::to_string(i));
            }

            TreePop();
        }

        return value_from(arr);
    }

    Json::value DrawJsonString(Json::value value, const std::string &name) {
        if (!value.is_string()) {
            return value;
        }

        const std::string str = value.as_string().c_str();
        char buf[256];
        std::snprintf(buf, sizeof(buf), "%s", str.c_str());
        if (InputText(name.c_str(), buf, sizeof(buf))) {
            return std::string(buf).data();
        }

        return value;
    }

    Json::value DrawJsonFloat(Json::value value, const std::string &name) {
        if (!value.is_number()) {
            return value;
        }

        float val = value.to_number<float>();
        if (DragFloat(name.c_str(), &val)) {
            return val;
        }

        return value;
    }

    Json::value DrawJsonInt(Json::value value, const std::string &name) {
        if (!value.is_number()) {
            return value;
        }

        int val = value.to_number<int>();
        if (DragInt(name.c_str(), &val)) {
            return val;
        }

        return value;
    }

    Json::value DrawJsonUint(Json::value value, const std::string &name) {
        if (!value.is_number()) {
            return value;
        }

        unsigned int val = value.to_number<unsigned int>();
        if (DragScalar(name.c_str(), ImGuiDataType_U32, &val)) {
            return val;
        }

        return value;
    }

    Json::value DrawJsonBool(Json::value value, const std::string &name) {
        if (!value.is_bool()) {
            return value;
        }

        bool val = value.as_bool();
        if (Checkbox(name.c_str(), &val)) {
            return val;
        }

        return value;
    }

    Json::value DrawVec3(Json::value value, const std::string &name) {
        if (!value.is_object()) {
            return value;
        }

        if (const auto &obj = value.as_object(); obj.contains("vec3")) {
            vec3 vec = Serialization::DeserializeVec3(obj);
            if (DragFloat3(name.c_str(), &vec.x)) {
                return value_from(Serialization::SerializeVec3(vec));
            }
        }

        return value;
    }

    Json::value DrawVec4(Json::value value, const std::string &name) {
        if (!value.is_object()) {
            return value;
        }

        if (const auto &obj = value.as_object(); obj.contains("vec4")) {
            vec4 vec = Serialization::DeserializeVec4(obj);
            if (DragFloat4(name.c_str(), &vec.x)) {
                return value_from(Serialization::SerializeVec4(vec));
            }
        }

        return value;
    }

    Json::value DrawQuat(Json::value value, const std::string &name) {
        if (!value.is_object()) {
            return value;
        }

        if (const auto &obj = value.as_object(); obj.contains("quat")) {
            vec3 vec = Serialization::DeserializeEuler(obj);
            if (DragFloat3(name.c_str(), &vec.x), 0.1F, 0, 0, "%.1f") {
                return value_from(Serialization::SerializeEuler(vec));
            }
        }

        return value;
    }

    Json::value DrawRgbColor(Json::value value, const std::string &name) {
        if (!value.is_object()) {
            return value;
        }

        if (const auto &obj = value.as_object(); obj.contains("rgb")) {
            vec3 col = Serialization::DeserializeColorRgb(obj);
            if (ColorEdit3(name.c_str(), &col.x)) {
                return value_from(Serialization::SerializeColorRgb(col));
            }
        }

        return value;
    }

    Json::value DrawRgbaColor(Json::value value, const std::string &name) {
        if (!value.is_object()) {
            return value;
        }

        if (const auto &obj = value.as_object(); obj.contains("rgba")) {
            vec4 col = Serialization::DeserializeColorRgba(obj);
            if (ColorEdit4(name.c_str(), &col.x)) {
                return value_from(Serialization::SerializeColorRgba(col));
            }
        }

        return value;
    }

    std::optional<std::filesystem::path> DrawDirectoryRecursive(const std::filesystem::path &path) {
        static std::optional<std::filesystem::path> result = std::nullopt;

        for (auto &entry: std::filesystem::directory_iterator(path)) {
            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanFullWidth;

            std::string label = entry.path().filename().string();
            if (entry.is_directory()) {
                flags |= ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanFullWidth;

                if (TreeNodeEx(label.c_str(), flags)) {
                    if (IsItemClicked()) {
                        result = entry.path();
                    }

                    DrawDirectoryRecursive(entry.path());
                    TreePop();
                }
            } else {
                flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

                std::string extension = entry.path().extension().string();
                std::remove(extension.begin(), extension.end(), extension.find_last_not_of('.') + 1);
                if (extension == ".meta") {
                    continue;
                }

                TreeNodeEx(label.c_str(), flags);
                if (IsItemClicked()) {
                    result = entry.path();
                }
            }
        }

        return result;
    }
} // namespace Pixf::Core::Gui
