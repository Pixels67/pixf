#include "Gui.hpp"

#include <imgui.h>
#include <imgui_impl_opengl3.h>

#include "Input/InputManager.hpp"

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

    void Initialize() {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::StyleColorsDark();

        ImGuiIO &io = ImGui::GetIO();
        io.IniFilename = nullptr;

        Event::EventManager::Subscribe<Graphics::Gl::WindowSizeChangedEvent>(
                [&](const Graphics::Gl::WindowSizeChangedEvent &event) {
                    io.DisplaySize.x = event.newSize.x;
                    io.DisplaySize.y = event.newSize.y;
                });

        Event::EventManager::Subscribe<Input::KeyEvent>([&](const Input::KeyEvent &event) {
            io.AddKeyEvent(ToImGuiKey(event.key), event.action == Input::KeyAction::Press);
        });

        Event::EventManager::Subscribe<Input::MouseKeyEvent>([&](const Input::MouseKeyEvent &event) {
            io.AddMouseButtonEvent(ToImGuiMouseKey(event.key), event.action == Input::KeyAction::Press);
        });

        Event::EventManager::Subscribe<Input::MouseMovedEvent>(
                [&](const Input::MouseMovedEvent &event) { io.AddMousePosEvent(event.position.x, event.position.y); });

        Event::EventManager::Subscribe<Input::CharacterEvent>(
                [&](const Input::CharacterEvent &event) { io.AddInputCharacter(event.codepoint); });

        ImGui_ImplOpenGL3_Init("#version 330");
    }

    void Terminate() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui::DestroyContext();
    }

    void BeginRenderGui(const double deltaTime) {
        ImGuiIO &io = ImGui::GetIO();
        io.DeltaTime = deltaTime;

        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();
    }

    void EndRenderGui() {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void BeginWindow(const std::string &label) {
        ImGui::Begin(label.c_str());
    }

    void EndWindow() {
        ImGui::End();
    }

    void BeginChild(const std::string &label, const Math::Vector2u size) {
        ImGui::BeginChild(label.c_str(), ImVec2(size.x, size.y));
    }

    void EndChild() {
        ImGui::EndChild();
    }

    bool Button(const std::string &label) {
        return ImGui::Button(label.c_str());
    }

    bool Checkbox(const std::string &label, bool *enabled) {
        return ImGui::Checkbox(label.c_str(), enabled);
    }

    bool InputText(const std::string &label, std::string &outputString, const unsigned int maxCharCount) {
        std::vector<char> buffer(maxCharCount);
        const bool result = ImGui::InputText(label.c_str(), buffer.data(), buffer.size());
        if (result) {
            outputString = buffer.data();
        }

        return result;
    }

    void InputInt(const std::string &label, int &number) {
        ImGui::InputInt(label.c_str(), &number);
    }

    void InputFloat(const std::string &label, float &number) {
        ImGui::InputFloat(label.c_str(), &number);
    }

    void DragInt(const std::string &label, int &number) {
        ImGui::DragInt(label.c_str(), &number);
    }

    void DragFloat(const std::string &label, float &number) {
        ImGui::DragFloat(label.c_str(), &number);
    }

    void DragVector3f(const std::string &label, Math::Vector3f &vector) {
        ImGui::DragFloat3(label.c_str(), &vector.x);
    }

    void DragVector3i(const std::string &label, Math::Vector3i &vector) {
        ImGui::DragInt3(label.c_str(), &vector.x);
    }

    void ColorEditor(const std::string &label, Math::Color3u8 &color) {
        Math::Vector3f vec = color.ToVector();
        ImGui::ColorEdit3(label.c_str(), &vec.x);
        color.FromVector(vec);
    }

    void ColorEditor(const std::string &label, Math::Color4u8 &color) {
        Math::Vector4f vec = color.ToVector();
        ImGui::ColorEdit4(label.c_str(), &vec.x);
        color.FromVector(vec);
    }
} // namespace Pixf::Core::Gui
