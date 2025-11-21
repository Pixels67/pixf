#ifndef PIXF_INPUTMANAGER_HPP
#define PIXF_INPUTMANAGER_HPP

#include "Common.hpp"
#include "Event/Event.hpp"
#include "Graphics/Gl/Window.hpp"

namespace Pixf::Core::Input {

#pragma region KeyEnum
    enum class Key : uint16_t {
        Space = GLFW_KEY_SPACE,
        Apostrophe = GLFW_KEY_APOSTROPHE,
        Comma = GLFW_KEY_COMMA,
        Minus = GLFW_KEY_MINUS,
        Period = GLFW_KEY_PERIOD,
        Slash = GLFW_KEY_SLASH,
        Alpha0 = GLFW_KEY_0,
        Alpha1 = GLFW_KEY_1,
        Alpha2 = GLFW_KEY_2,
        Alpha3 = GLFW_KEY_3,
        Alpha4 = GLFW_KEY_4,
        Alpha5 = GLFW_KEY_5,
        Alpha6 = GLFW_KEY_6,
        Alpha7 = GLFW_KEY_7,
        Alpha8 = GLFW_KEY_8,
        Alpha9 = GLFW_KEY_9,
        Semicolon = GLFW_KEY_SEMICOLON,
        Equal = GLFW_KEY_EQUAL,
        A = GLFW_KEY_A,
        B = GLFW_KEY_B,
        C = GLFW_KEY_C,
        D = GLFW_KEY_D,
        E = GLFW_KEY_E,
        F = GLFW_KEY_F,
        G = GLFW_KEY_G,
        H = GLFW_KEY_H,
        I = GLFW_KEY_I,
        J = GLFW_KEY_J,
        K = GLFW_KEY_K,
        L = GLFW_KEY_L,
        M = GLFW_KEY_M,
        N = GLFW_KEY_N,
        O = GLFW_KEY_O,
        P = GLFW_KEY_P,
        Q = GLFW_KEY_Q,
        R = GLFW_KEY_R,
        S = GLFW_KEY_S,
        T = GLFW_KEY_T,
        U = GLFW_KEY_U,
        V = GLFW_KEY_V,
        W = GLFW_KEY_W,
        X = GLFW_KEY_X,
        Y = GLFW_KEY_Y,
        Z = GLFW_KEY_Z,
        LBracket = GLFW_KEY_LEFT_BRACKET,
        Backslash = GLFW_KEY_BACKSLASH,
        RBracket = GLFW_KEY_RIGHT_BRACKET,
        GraveAccent = GLFW_KEY_GRAVE_ACCENT,
        World1 = GLFW_KEY_WORLD_1,
        World2 = GLFW_KEY_WORLD_2,
        Escape = GLFW_KEY_ESCAPE,
        Return = GLFW_KEY_ENTER,
        Tab = GLFW_KEY_TAB,
        Backspace = GLFW_KEY_BACKSPACE,
        Insert = GLFW_KEY_INSERT,
        Delete = GLFW_KEY_DELETE,
        Right = GLFW_KEY_RIGHT,
        Left = GLFW_KEY_LEFT,
        Down = GLFW_KEY_DOWN,
        Up = GLFW_KEY_UP,
        PageUp = GLFW_KEY_PAGE_UP,
        PageDown = GLFW_KEY_PAGE_DOWN,
        Home = GLFW_KEY_HOME,
        End = GLFW_KEY_END,
        CapsLock = GLFW_KEY_CAPS_LOCK,
        ScrollLock = GLFW_KEY_SCROLL_LOCK,
        NumLock = GLFW_KEY_NUM_LOCK,
        PrintScreen = GLFW_KEY_PRINT_SCREEN,
        Pause = GLFW_KEY_PAUSE,
        F1 = GLFW_KEY_F1,
        F2 = GLFW_KEY_F2,
        F3 = GLFW_KEY_F3,
        F4 = GLFW_KEY_F4,
        F5 = GLFW_KEY_F5,
        F6 = GLFW_KEY_F6,
        F7 = GLFW_KEY_F7,
        F8 = GLFW_KEY_F8,
        F9 = GLFW_KEY_F9,
        F10 = GLFW_KEY_F10,
        F11 = GLFW_KEY_F11,
        F12 = GLFW_KEY_F12,
        F13 = GLFW_KEY_F13,
        F14 = GLFW_KEY_F14,
        F15 = GLFW_KEY_F15,
        F16 = GLFW_KEY_F16,
        F17 = GLFW_KEY_F17,
        F18 = GLFW_KEY_F18,
        F19 = GLFW_KEY_F19,
        F20 = GLFW_KEY_F20,
        F21 = GLFW_KEY_F21,
        F22 = GLFW_KEY_F22,
        F23 = GLFW_KEY_F23,
        F24 = GLFW_KEY_F24,
        F25 = GLFW_KEY_F25,
        Kp0 = GLFW_KEY_KP_0,
        Kp1 = GLFW_KEY_KP_1,
        Kp2 = GLFW_KEY_KP_2,
        Kp3 = GLFW_KEY_KP_3,
        Kp4 = GLFW_KEY_KP_4,
        Kp5 = GLFW_KEY_KP_5,
        Kp6 = GLFW_KEY_KP_6,
        Kp7 = GLFW_KEY_KP_7,
        Kp8 = GLFW_KEY_KP_8,
        Kp9 = GLFW_KEY_KP_9,
        KpDecimal = GLFW_KEY_KP_DECIMAL,
        KpDivide = GLFW_KEY_KP_DIVIDE,
        KpMultiply = GLFW_KEY_KP_MULTIPLY,
        KpSubtract = GLFW_KEY_KP_SUBTRACT,
        KpAdd = GLFW_KEY_KP_ADD,
        KpReturn = GLFW_KEY_KP_ENTER,
        KpEqual = GLFW_KEY_KP_EQUAL,
        LShift = GLFW_KEY_LEFT_SHIFT,
        LCtrl = GLFW_KEY_LEFT_CONTROL,
        LAlt = GLFW_KEY_LEFT_ALT,
        LSuper = GLFW_KEY_LEFT_SUPER,
        RShift = GLFW_KEY_RIGHT_SHIFT,
        RCtrl = GLFW_KEY_RIGHT_CONTROL,
        RAlt = GLFW_KEY_RIGHT_ALT,
        RSuper = GLFW_KEY_RIGHT_SUPER,
        Menu = GLFW_KEY_MENU,
    };

    inline std::string ToString(const Key key) {
        switch (key) {
            case Key::Space:
                return " ";
            case Key::Apostrophe:
                return "\'";
            case Key::Comma:
                return ",";
            case Key::Minus:
                return "-";
            case Key::Period:
                return ".";
            case Key::Slash:
                return "/";
            case Key::Alpha0:
                return "0";
            case Key::Alpha1:
                return "1";
            case Key::Alpha2:
                return "2";
            case Key::Alpha3:
                return "3";
            case Key::Alpha4:
                return "4";
            case Key::Alpha5:
                return "5";
            case Key::Alpha6:
                return "6";
            case Key::Alpha7:
                return "7";
            case Key::Alpha8:
                return "8";
            case Key::Alpha9:
                return "9";
            case Key::Semicolon:
                return ";";
            case Key::Equal:
                return "=";
            case Key::A:
                return "A";
            case Key::B:
                return "B";
            case Key::C:
                return "C";
            case Key::D:
                return "D";
            case Key::E:
                return "E";
            case Key::F:
                return "F";
            case Key::G:
                return "G";
            case Key::H:
                return "H";
            case Key::I:
                return "I";
            case Key::J:
                return "J";
            case Key::K:
                return "K";
            case Key::L:
                return "L";
            case Key::M:
                return "M";
            case Key::N:
                return "N";
            case Key::O:
                return "O";
            case Key::P:
                return "P";
            case Key::Q:
                return "Q";
            case Key::R:
                return "R";
            case Key::S:
                return "S";
            case Key::T:
                return "T";
            case Key::U:
                return "U";
            case Key::V:
                return "V";
            case Key::W:
                return "W";
            case Key::X:
                return "X";
            case Key::Y:
                return "Y";
            case Key::Z:
                return "Z";
            case Key::LBracket:
                return "[";
            case Key::Backslash:
                return "\\";
            case Key::RBracket:
                return "]";
            case Key::GraveAccent:
                return "GraveAccent";
            case Key::World1:
                return "World1";
            case Key::World2:
                return "World2";
            case Key::Escape:
                return "Escape";
            case Key::Return:
                return "Return";
            case Key::Tab:
                return "Tab";
            case Key::Backspace:
                return "Backspace";
            case Key::Insert:
                return "Insert";
            case Key::Delete:
                return "Delete";
            case Key::Right:
                return "Right";
            case Key::Left:
                return "Left";
            case Key::Down:
                return "Down";
            case Key::Up:
                return "Up";
            case Key::PageUp:
                return "PgUp";
            case Key::PageDown:
                return "PgDown";
            case Key::Home:
                return "Home";
            case Key::End:
                return "End";
            case Key::CapsLock:
                return "CapsLock";
            case Key::ScrollLock:
                return "ScrollLock";
            case Key::NumLock:
                return "NumLock";
            case Key::PrintScreen:
                return "PrintScreen";
            case Key::Pause:
                return "Pause";
            case Key::F1:
                return "F1";
            case Key::F2:
                return "F2";
            case Key::F3:
                return "F3";
            case Key::F4:
                return "F4";
            case Key::F5:
                return "F5";
            case Key::F6:
                return "F6";
            case Key::F7:
                return "F7";
            case Key::F8:
                return "F8";
            case Key::F9:
                return "F9";
            case Key::F10:
                return "F10";
            case Key::F11:
                return "F11";
            case Key::F12:
                return "F12";
            case Key::F13:
                return "F13";
            case Key::F14:
                return "F14";
            case Key::F15:
                return "F15";
            case Key::F16:
                return "F16";
            case Key::F17:
                return "F17";
            case Key::F18:
                return "F18";
            case Key::F19:
                return "F19";
            case Key::F20:
                return "F20";
            case Key::F21:
                return "F21";
            case Key::F22:
                return "F22";
            case Key::F23:
                return "F23";
            case Key::F24:
                return "F24";
            case Key::F25:
                return "F25";
            case Key::Kp0:
                return "Kp0";
            case Key::Kp1:
                return "Kp1";
            case Key::Kp2:
                return "Kp2";
            case Key::Kp3:
                return "Kp3";
            case Key::Kp4:
                return "Kp4";
            case Key::Kp5:
                return "Kp5";
            case Key::Kp6:
                return "Kp6";
            case Key::Kp7:
                return "Kp7";
            case Key::Kp8:
                return "Kp8";
            case Key::Kp9:
                return "Kp9";
            case Key::KpDecimal:
                return "KpDecimal";
            case Key::KpDivide:
                return "KpDivide";
            case Key::KpMultiply:
                return "KpMultiply";
            case Key::KpSubtract:
                return "KpSubtract";
            case Key::KpAdd:
                return "KpAdd";
            case Key::KpReturn:
                return "KpReturn";
            case Key::KpEqual:
                return "KpEqual";
            case Key::LShift:
                return "LShift";
            case Key::LCtrl:
                return "LCtrl";
            case Key::LAlt:
                return "LAlt";
            case Key::LSuper:
                return "LSuper";
            case Key::RShift:
                return "RShift";
            case Key::RCtrl:
                return "RCtrl";
            case Key::RAlt:
                return "RAlt";
            case Key::RSuper:
                return "RSuper";
            case Key::Menu:
                return "Menu";
            default:
                return "";
        }
    }

#pragma endregion

#pragma region KeyActionEnum
    enum class KeyAction : uint8_t {
        Press = GLFW_PRESS,
        Release = GLFW_RELEASE,
    };

    inline std::string ToString(const KeyAction action) {
        switch (action) {
            case KeyAction::Press:
                return "Press";
            case KeyAction::Release:
                return "Release";
            default:
                return "";
        }
    }

#pragma endregion

#pragma region MouseKeyEnum
    enum class MouseKey : uint8_t {
        Mouse1 = GLFW_MOUSE_BUTTON_1,
        Mouse2 = GLFW_MOUSE_BUTTON_2,
        Mouse3 = GLFW_MOUSE_BUTTON_3,
        Mouse4 = GLFW_MOUSE_BUTTON_4,
        Mouse5 = GLFW_MOUSE_BUTTON_5,
        Mouse6 = GLFW_MOUSE_BUTTON_6,
        Mouse7 = GLFW_MOUSE_BUTTON_7,
        Mouse8 = GLFW_MOUSE_BUTTON_8,
    };

    inline std::string ToString(const MouseKey key) {
        switch (key) {
            case MouseKey::Mouse1:
                return "Mouse1";
            case MouseKey::Mouse2:
                return "Mouse2";
            case MouseKey::Mouse3:
                return "Mouse3";
            case MouseKey::Mouse4:
                return "Mouse4";
            case MouseKey::Mouse5:
                return "Mouse5";
            case MouseKey::Mouse6:
                return "Mouse6";
            case MouseKey::Mouse7:
                return "Mouse7";
            case MouseKey::Mouse8:
                return "Mouse8";
            default:
                return "";
        }
    }

#pragma endregion

    class PIXF_API KeyEvent final : public Event::Event {
    public:
        Key key;
        KeyAction action;

        explicit KeyEvent(const Key key, const KeyAction action) : key(key), action(action) {}
    };

    class PIXF_API MouseKeyEvent final : public Event::Event {
    public:
        MouseKey key;
        KeyAction action;

        explicit MouseKeyEvent(const MouseKey key, const KeyAction action) : key(key), action(action) {}
    };

    class PIXF_API MouseMovedEvent final : public Event::Event {
    public:
        Math::Vector2d position;

        explicit MouseMovedEvent(const Math::Vector2d position) : position(position) {}
    };

    class PIXF_API CharacterEvent final : public Event::Event {
    public:
        unsigned int codepoint;

        explicit CharacterEvent(const unsigned int codepoint) : codepoint(codepoint) {}
    };

    class PIXF_API InputManager {
    public:
        static void Initialize();

        static bool IsKeyDown(Key key);
        static bool IsMouseKeyDown(MouseKey mouseKey);

        static Math::Vector2d GetMousePosition();
        static Math::Vector2d GetMouseDelta();

    private:
        inline static Math::Vector2d s_LastMousePosition{};

        static void HandleKeyEvent(const Graphics::Gl::KeyEvent &keyEvent);
        static void HandleMouseKeyEvent(const Graphics::Gl::MouseKeyEvent &mouseKeyEvent);
        static void HandleMouseMovedEvent(const Graphics::Gl::MouseMovedEvent &mouseMovedEvent);
        static void HandleCharEvent(const Graphics::Gl::CharEvent &charEvent);
    };

} // namespace Pixf::Core::Input

#endif // PIXF_INPUTMANAGER_HPP
