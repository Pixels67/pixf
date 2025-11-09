#ifndef INPUTMANAGER_HPP
#define INPUTMANAGER_HPP

#include "Common.hpp"
#include "Event/Event.hpp"
#include "Graphics/Gl/Gl.hpp"
#include "Graphics/Gl/Window.hpp"

namespace Pixf::Core::Input {
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

    inline std::ostream &operator<<(std::ostream &os, const Key key) {
        switch (key) {
            case Key::Space:
                os << " ";
                break;
            case Key::Apostrophe:
                os << "\'";
                break;
            case Key::Comma:
                os << ",";
                break;
            case Key::Minus:
                os << "-";
                break;
            case Key::Period:
                os << ".";
                break;
            case Key::Slash:
                os << "/";
                break;
            case Key::Alpha0:
                os << "0";
                break;
            case Key::Alpha1:
                os << "1";
                break;
            case Key::Alpha2:
                os << "2";
                break;
            case Key::Alpha3:
                os << "3";
                break;
            case Key::Alpha4:
                os << "4";
                break;
            case Key::Alpha5:
                os << "5";
                break;
            case Key::Alpha6:
                os << "6";
                break;
            case Key::Alpha7:
                os << "7";
                break;
            case Key::Alpha8:
                os << "8";
                break;
            case Key::Alpha9:
                os << "9";
                break;
            case Key::Semicolon:
                os << ";";
                break;
            case Key::Equal:
                os << "=";
                break;
            case Key::A:
                os << "A";
                break;
            case Key::B:
                os << "B";
                break;
            case Key::C:
                os << "C";
                break;
            case Key::D:
                os << "D";
                break;
            case Key::E:
                os << "E";
                break;
            case Key::F:
                os << "F";
                break;
            case Key::G:
                os << "G";
                break;
            case Key::H:
                os << "H";
                break;
            case Key::I:
                os << "I";
                break;
            case Key::J:
                os << "J";
                break;
            case Key::K:
                os << "K";
                break;
            case Key::L:
                os << "L";
                break;
            case Key::M:
                os << "M";
                break;
            case Key::N:
                os << "N";
                break;
            case Key::O:
                os << "O";
                break;
            case Key::P:
                os << "P";
                break;
            case Key::Q:
                os << "Q";
                break;
            case Key::R:
                os << "R";
                break;
            case Key::S:
                os << "S";
                break;
            case Key::T:
                os << "T";
                break;
            case Key::U:
                os << "U";
                break;
            case Key::V:
                os << "V";
                break;
            case Key::W:
                os << "W";
                break;
            case Key::X:
                os << "X";
                break;
            case Key::Y:
                os << "Y";
                break;
            case Key::Z:
                os << "Z";
                break;
            case Key::LBracket:
                os << "[";
                break;
            case Key::Backslash:
                os << "\\";
                break;
            case Key::RBracket:
                os << "]";
                break;
            case Key::GraveAccent:
                os << "GraveAccent";
                break;
            case Key::World1:
                os << "World1";
                break;
            case Key::World2:
                os << "World2";
                break;
            case Key::Escape:
                os << "Escape";
                break;
            case Key::Return:
                os << "Return";
                break;
            case Key::Tab:
                os << "Tab";
                break;
            case Key::Backspace:
                os << "Backspace";
                break;
            case Key::Insert:
                os << "Insert";
                break;
            case Key::Delete:
                os << "Delete";
                break;
            case Key::Right:
                os << "Right";
                break;
            case Key::Left:
                os << "Left";
                break;
            case Key::Down:
                os << "Down";
                break;
            case Key::Up:
                os << "Up";
                break;
            case Key::PageUp:
                os << "PgUp";
                break;
            case Key::PageDown:
                os << "PgDown";
                break;
            case Key::Home:
                os << "Home";
                break;
            case Key::End:
                os << "End";
                break;
            case Key::CapsLock:
                os << "CapsLock";
                break;
            case Key::ScrollLock:
                os << "ScrollLock";
                break;
            case Key::NumLock:
                os << "NumLock";
                break;
            case Key::PrintScreen:
                os << "PrintScreen";
                break;
            case Key::Pause:
                os << "Pause";
                break;
            case Key::F1:
                os << "F1";
                break;
            case Key::F2:
                os << "F2";
                break;
            case Key::F3:
                os << "F3";
                break;
            case Key::F4:
                os << "F4";
                break;
            case Key::F5:
                os << "F5";
                break;
            case Key::F6:
                os << "F6";
                break;
            case Key::F7:
                os << "F7";
                break;
            case Key::F8:
                os << "F8";
                break;
            case Key::F9:
                os << "F9";
                break;
            case Key::F10:
                os << "F10";
                break;
            case Key::F11:
                os << "F11";
                break;
            case Key::F12:
                os << "F12";
                break;
            case Key::F13:
                os << "F13";
                break;
            case Key::F14:
                os << "F14";
                break;
            case Key::F15:
                os << "F15";
                break;
            case Key::F16:
                os << "F16";
                break;
            case Key::F17:
                os << "F17";
                break;
            case Key::F18:
                os << "F18";
                break;
            case Key::F19:
                os << "F19";
                break;
            case Key::F20:
                os << "F20";
                break;
            case Key::F21:
                os << "F21";
                break;
            case Key::F22:
                os << "F22";
                break;
            case Key::F23:
                os << "F23";
                break;
            case Key::F24:
                os << "F24";
                break;
            case Key::F25:
                os << "F25";
                break;
            case Key::Kp0:
                os << "Kp0";
                break;
            case Key::Kp1:
                os << "Kp1";
                break;
            case Key::Kp2:
                os << "Kp2";
                break;
            case Key::Kp3:
                os << "Kp3";
                break;
            case Key::Kp4:
                os << "Kp4";
                break;
            case Key::Kp5:
                os << "Kp5";
                break;
            case Key::Kp6:
                os << "Kp6";
                break;
            case Key::Kp7:
                os << "Kp7";
                break;
            case Key::Kp8:
                os << "Kp8";
                break;
            case Key::Kp9:
                os << "Kp9";
                break;
            case Key::KpDecimal:
                os << "KpDecimal";
                break;
            case Key::KpDivide:
                os << "KpDivide";
                break;
            case Key::KpMultiply:
                os << "KpMultiply";
                break;
            case Key::KpSubtract:
                os << "KpSubtract";
                break;
            case Key::KpAdd:
                os << "KpAdd";
                break;
            case Key::KpReturn:
                os << "KpReturn";
                break;
            case Key::KpEqual:
                os << "KpEqual";
                break;
            case Key::LShift:
                os << "LShift";
                break;
            case Key::LCtrl:
                os << "LCtrl";
                break;
            case Key::LAlt:
                os << "LAlt";
                break;
            case Key::LSuper:
                os << "LSuper";
                break;
            case Key::RShift:
                os << "RShift";
                break;
            case Key::RCtrl:
                os << "RCtrl";
                break;
            case Key::RAlt:
                os << "RAlt";
                break;
            case Key::RSuper:
                os << "RSuper";
                break;
            case Key::Menu:
                os << "Menu";
                break;
        }

        return os;
    }

    enum class KeyAction : uint8_t {
        Press = GLFW_PRESS,
        Release = GLFW_RELEASE,
    };

    inline std::ostream &operator<<(std::ostream &os, const KeyAction action) {
        switch (action) {
            case KeyAction::Press:
                os << "Press";
                break;
            case KeyAction::Release:
                os << "Release";
                break;
        }

        return os;
    }

    class KeyEvent final : public Event::Event {
    public:
        Key key;
        KeyAction action;

        explicit KeyEvent(const Key key, const KeyAction action) : key(key), action(action) {}
    };

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

    inline std::ostream &operator<<(std::ostream &os, const MouseKey key) {
        switch (key) {
            case MouseKey::Mouse1:
                os << "Mouse1";
                break;
            case MouseKey::Mouse2:
                os << "Mouse2";
                break;
            case MouseKey::Mouse3:
                os << "Mouse3";
                break;
            case MouseKey::Mouse4:
                os << "Mouse4";
                break;
            case MouseKey::Mouse5:
                os << "Mouse5";
                break;
            case MouseKey::Mouse6:
                os << "Mouse6";
                break;
            case MouseKey::Mouse7:
                os << "Mouse7";
                break;
            case MouseKey::Mouse8:
                os << "Mouse8";
                break;
        }

        return os;
    }

    class MouseKeyEvent final : public Event::Event {
    public:
        MouseKey key;
        KeyAction action;

        explicit MouseKeyEvent(const MouseKey key, const KeyAction action) : key(key), action(action) {}
    };

    class MouseMovedEvent final : public Event::Event {
    public:
        vec2 position;

        explicit MouseMovedEvent(const vec2 position) : position(position) {}
    };

    class CharacterEvent final : public Event::Event {
    public:
        unsigned int codepoint;

        explicit CharacterEvent(const unsigned int codepoint) : codepoint(codepoint) {}
    };

    struct InputManager {
        explicit InputManager(Event::EventManager &eventManager, Graphics::Gl::Window &window);

        InputManager(const InputManager &) = delete;
        InputManager(InputManager &&) = delete;
        InputManager &operator=(const InputManager &) = delete;
        InputManager &operator=(InputManager &&) = delete;

        ~InputManager() = default;

        bool IsKeyDown(Key key) const;
        bool IsKeyUp(Key key) const;

        bool IsMouseKeyDown(MouseKey key) const;
        bool IsMouseKeyUp(MouseKey key) const;

        dvec2 GetMousePosition() const;
        dvec2 GetMouseDelta() const;

        void Update();

    private:
        dvec2 m_MousePos = {};
        dvec2 m_MouseDelta = {};
        Event::EventManager *m_EventManager;
        Graphics::Gl::Window *m_Window;

        void SetupCallbacks(const Graphics::Gl::Window &window) const;
    };

} // namespace Pixf::Core::Input

#endif // INPUTMANAGER_HPP
