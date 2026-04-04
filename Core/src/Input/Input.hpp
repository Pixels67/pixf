#ifndef FLK_INPUT_HPP
#define FLK_INPUT_HPP

#include <set>

#include "Graphics/Gl.hpp"
#include "Common.hpp"
#include "Event/EventHandler.hpp"
#include "Math/Math.hpp"
#include "Math/Rect.hpp"

namespace Flock::Input {
    enum class Key {
        // Printable Keys
        None,
        Space,
        Apostrophe,
        Comma,
        Minus,
        Period,
        Slash,
        A0,
        A1,
        A2,
        A3,
        A4,
        A5,
        A6,
        A7,
        A8,
        A9,
        Semicolon,
        Equal,
        A,
        B,
        C,
        D,
        E,
        F,
        G,
        H,
        I,
        J,
        K,
        L,
        M,
        N,
        O,
        P,
        Q,
        R,
        S,
        T,
        U,
        V,
        W,
        X,
        Y,
        Z,
        LBracket,
        Backslash,
        RBracket,
        GraveAccent,
        World1,
        World2,
        // Function Keys
        Escape,
        Return,
        Tab,
        Backspace,
        Insert,
        Delete,
        Right,
        Left,
        Down,
        Up,
        PageUp,
        PageDown,
        Home,
        End,
        CapsLock,
        ScrollLock,
        NumLock,
        PrintScreen,
        Pause,
        F1,
        F2,
        F3,
        F4,
        F5,
        F6,
        F7,
        F8,
        F9,
        F10,
        F11,
        F12,
        F13,
        F14,
        F15,
        F16,
        F17,
        F18,
        F19,
        F20,
        F21,
        F22,
        F23,
        F24,
        F25,
        Np0,
        Np1,
        Np2,
        Np3,
        Np4,
        Np5,
        Np6,
        Np7,
        Np8,
        Np9,
        NpDecimal,
        NpDivide,
        NpMultiply,
        NpSubtract,
        NpAdd,
        NpReturn,
        NpEqual,
        LShift,
        LControl,
        LAlt,
        LSuper,
        RShift,
        RControl,
        RAlt,
        RSuper,
        Menu,
    };

    enum class MouseButton : u16 {
        None   = 0,
        Mouse0 = 1,
        Mouse1 = 2,
        Mouse2 = 3,
        Mouse3 = 4,
        Mouse4 = 5,
        Mouse5 = 6,
        Mouse6 = 7,
        Mouse7 = 8,
        Left   = Mouse0,
        Right  = Mouse1,
        Middle = Mouse2,
    };

    enum class Action : u8 {
        None = 0,
        Press,
        Release,
        Repeat,
    };

    inline i32 ToGlfwType(const Key key) {
        switch (key) {
            case Key::None: return 0;
            case Key::Space: return GLFW_KEY_SPACE;
            case Key::Apostrophe: return GLFW_KEY_APOSTROPHE;
            case Key::Comma: return GLFW_KEY_COMMA;
            case Key::Minus: return GLFW_KEY_MINUS;
            case Key::Period: return GLFW_KEY_PERIOD;
            case Key::Slash: return GLFW_KEY_SLASH;
            case Key::A0: return GLFW_KEY_0;
            case Key::A1: return GLFW_KEY_1;
            case Key::A2: return GLFW_KEY_2;
            case Key::A3: return GLFW_KEY_3;
            case Key::A4: return GLFW_KEY_4;
            case Key::A5: return GLFW_KEY_5;
            case Key::A6: return GLFW_KEY_6;
            case Key::A7: return GLFW_KEY_7;
            case Key::A8: return GLFW_KEY_8;
            case Key::A9: return GLFW_KEY_9;
            case Key::Semicolon: return GLFW_KEY_SEMICOLON;
            case Key::Equal: return GLFW_KEY_EQUAL;
            case Key::A: return GLFW_KEY_A;
            case Key::B: return GLFW_KEY_B;
            case Key::C: return GLFW_KEY_C;
            case Key::D: return GLFW_KEY_D;
            case Key::E: return GLFW_KEY_E;
            case Key::F: return GLFW_KEY_F;
            case Key::G: return GLFW_KEY_G;
            case Key::H: return GLFW_KEY_H;
            case Key::I: return GLFW_KEY_I;
            case Key::J: return GLFW_KEY_J;
            case Key::K: return GLFW_KEY_K;
            case Key::L: return GLFW_KEY_L;
            case Key::M: return GLFW_KEY_M;
            case Key::N: return GLFW_KEY_N;
            case Key::O: return GLFW_KEY_O;
            case Key::P: return GLFW_KEY_P;
            case Key::Q: return GLFW_KEY_Q;
            case Key::R: return GLFW_KEY_R;
            case Key::S: return GLFW_KEY_S;
            case Key::T: return GLFW_KEY_T;
            case Key::U: return GLFW_KEY_U;
            case Key::V: return GLFW_KEY_V;
            case Key::W: return GLFW_KEY_W;
            case Key::X: return GLFW_KEY_X;
            case Key::Y: return GLFW_KEY_Y;
            case Key::Z: return GLFW_KEY_Z;
            case Key::LBracket: return GLFW_KEY_LEFT_BRACKET;
            case Key::Backslash: return GLFW_KEY_BACKSLASH;
            case Key::RBracket: return GLFW_KEY_RIGHT_BRACKET;
            case Key::GraveAccent: return GLFW_KEY_GRAVE_ACCENT;
            case Key::World1: return GLFW_KEY_WORLD_1;
            case Key::World2: return GLFW_KEY_WORLD_2;
            case Key::Escape: return GLFW_KEY_ESCAPE;
            case Key::Return: return GLFW_KEY_ENTER;
            case Key::Tab: return GLFW_KEY_TAB;
            case Key::Backspace: return GLFW_KEY_BACKSPACE;
            case Key::Insert: return GLFW_KEY_INSERT;
            case Key::Delete: return GLFW_KEY_DELETE;
            case Key::Right: return GLFW_KEY_RIGHT;
            case Key::Left: return GLFW_KEY_LEFT;
            case Key::Down: return GLFW_KEY_DOWN;
            case Key::Up: return GLFW_KEY_UP;
            case Key::PageUp: return GLFW_KEY_PAGE_UP;
            case Key::PageDown: return GLFW_KEY_PAGE_DOWN;
            case Key::Home: return GLFW_KEY_HOME;
            case Key::End: return GLFW_KEY_END;
            case Key::CapsLock: return GLFW_KEY_CAPS_LOCK;
            case Key::ScrollLock: return GLFW_KEY_SCROLL_LOCK;
            case Key::NumLock: return GLFW_KEY_NUM_LOCK;
            case Key::PrintScreen: return GLFW_KEY_PRINT_SCREEN;
            case Key::Pause: return GLFW_KEY_PAUSE;
            case Key::F1: return GLFW_KEY_F1;
            case Key::F2: return GLFW_KEY_F2;
            case Key::F3: return GLFW_KEY_F3;
            case Key::F4: return GLFW_KEY_F4;
            case Key::F5: return GLFW_KEY_F5;
            case Key::F6: return GLFW_KEY_F6;
            case Key::F7: return GLFW_KEY_F7;
            case Key::F8: return GLFW_KEY_F8;
            case Key::F9: return GLFW_KEY_F9;
            case Key::F10: return GLFW_KEY_F10;
            case Key::F11: return GLFW_KEY_F11;
            case Key::F12: return GLFW_KEY_F12;
            case Key::F13: return GLFW_KEY_F13;
            case Key::F14: return GLFW_KEY_F14;
            case Key::F15: return GLFW_KEY_F15;
            case Key::F16: return GLFW_KEY_F16;
            case Key::F17: return GLFW_KEY_F17;
            case Key::F18: return GLFW_KEY_F18;
            case Key::F19: return GLFW_KEY_F19;
            case Key::F20: return GLFW_KEY_F20;
            case Key::F21: return GLFW_KEY_F21;
            case Key::F22: return GLFW_KEY_F22;
            case Key::F23: return GLFW_KEY_F23;
            case Key::F24: return GLFW_KEY_F24;
            case Key::F25: return GLFW_KEY_F25;
            case Key::Np0: return GLFW_KEY_KP_0;
            case Key::Np1: return GLFW_KEY_KP_1;
            case Key::Np2: return GLFW_KEY_KP_2;
            case Key::Np3: return GLFW_KEY_KP_3;
            case Key::Np4: return GLFW_KEY_KP_4;
            case Key::Np5: return GLFW_KEY_KP_5;
            case Key::Np6: return GLFW_KEY_KP_6;
            case Key::Np7: return GLFW_KEY_KP_7;
            case Key::Np8: return GLFW_KEY_KP_8;
            case Key::Np9: return GLFW_KEY_KP_9;
            case Key::NpDecimal: return GLFW_KEY_KP_DECIMAL;
            case Key::NpDivide: return GLFW_KEY_KP_DIVIDE;
            case Key::NpMultiply: return GLFW_KEY_KP_MULTIPLY;
            case Key::NpSubtract: return GLFW_KEY_KP_SUBTRACT;
            case Key::NpAdd: return GLFW_KEY_KP_ADD;
            case Key::NpReturn: return GLFW_KEY_KP_ENTER;
            case Key::NpEqual: return GLFW_KEY_KP_EQUAL;
            case Key::LShift: return GLFW_KEY_LEFT_SHIFT;
            case Key::LControl: return GLFW_KEY_LEFT_CONTROL;
            case Key::LAlt: return GLFW_KEY_LEFT_ALT;
            case Key::LSuper: return GLFW_KEY_LEFT_SUPER;
            case Key::RShift: return GLFW_KEY_RIGHT_SHIFT;
            case Key::RControl: return GLFW_KEY_RIGHT_CONTROL;
            case Key::RAlt: return GLFW_KEY_RIGHT_ALT;
            case Key::RSuper: return GLFW_KEY_RIGHT_SUPER;
            case Key::Menu: return GLFW_KEY_MENU;
            default: return 0;
        }
    }

    inline i32 ToGlfwType(const MouseButton button) {
        switch (button) {
            case MouseButton::Mouse0: return 0;
            case MouseButton::Mouse1: return 1;
            case MouseButton::Mouse2: return 2;
            case MouseButton::Mouse3: return 3;
            case MouseButton::Mouse4: return 4;
            case MouseButton::Mouse5: return 5;
            case MouseButton::Mouse6: return 6;
            case MouseButton::Mouse7: return 7;
            default: return 0;
        }
    }

    inline i32 ToGlfwType(const Action action) {
        switch (action) {
            case Action::Press: return GLFW_PRESS;
            case Action::Release: return GLFW_RELEASE;
            case Action::Repeat: return GLFW_REPEAT;
            default: return 0;
        }
    }

    inline Key KeyFromGlfwType(const i32 key) {
        switch (key) {
            case GLFW_KEY_SPACE: return Key::Space;
            case GLFW_KEY_APOSTROPHE: return Key::Apostrophe;
            case GLFW_KEY_COMMA: return Key::Comma;
            case GLFW_KEY_MINUS: return Key::Minus;
            case GLFW_KEY_PERIOD: return Key::Period;
            case GLFW_KEY_SLASH: return Key::Slash;
            case GLFW_KEY_0: return Key::A0;
            case GLFW_KEY_1: return Key::A1;
            case GLFW_KEY_2: return Key::A2;
            case GLFW_KEY_3: return Key::A3;
            case GLFW_KEY_4: return Key::A4;
            case GLFW_KEY_5: return Key::A5;
            case GLFW_KEY_6: return Key::A6;
            case GLFW_KEY_7: return Key::A7;
            case GLFW_KEY_8: return Key::A8;
            case GLFW_KEY_9: return Key::A9;
            case GLFW_KEY_SEMICOLON: return Key::Semicolon;
            case GLFW_KEY_EQUAL: return Key::Equal;
            case GLFW_KEY_A: return Key::A;
            case GLFW_KEY_B: return Key::B;
            case GLFW_KEY_C: return Key::C;
            case GLFW_KEY_D: return Key::D;
            case GLFW_KEY_E: return Key::E;
            case GLFW_KEY_F: return Key::F;
            case GLFW_KEY_G: return Key::G;
            case GLFW_KEY_H: return Key::H;
            case GLFW_KEY_I: return Key::I;
            case GLFW_KEY_J: return Key::J;
            case GLFW_KEY_K: return Key::K;
            case GLFW_KEY_L: return Key::L;
            case GLFW_KEY_M: return Key::M;
            case GLFW_KEY_N: return Key::N;
            case GLFW_KEY_O: return Key::O;
            case GLFW_KEY_P: return Key::P;
            case GLFW_KEY_Q: return Key::Q;
            case GLFW_KEY_R: return Key::R;
            case GLFW_KEY_S: return Key::S;
            case GLFW_KEY_T: return Key::T;
            case GLFW_KEY_U: return Key::U;
            case GLFW_KEY_V: return Key::V;
            case GLFW_KEY_W: return Key::W;
            case GLFW_KEY_X: return Key::X;
            case GLFW_KEY_Y: return Key::Y;
            case GLFW_KEY_Z: return Key::Z;
            case GLFW_KEY_LEFT_BRACKET: return Key::LBracket;
            case GLFW_KEY_BACKSLASH: return Key::Backslash;
            case GLFW_KEY_RIGHT_BRACKET: return Key::RBracket;
            case GLFW_KEY_GRAVE_ACCENT: return Key::GraveAccent;
            case GLFW_KEY_WORLD_1: return Key::World1;
            case GLFW_KEY_WORLD_2: return Key::World2;
            case GLFW_KEY_ESCAPE: return Key::Escape;
            case GLFW_KEY_ENTER: return Key::Return;
            case GLFW_KEY_TAB: return Key::Tab;
            case GLFW_KEY_BACKSPACE: return Key::Backspace;
            case GLFW_KEY_INSERT: return Key::Insert;
            case GLFW_KEY_DELETE: return Key::Delete;
            case GLFW_KEY_RIGHT: return Key::Right;
            case GLFW_KEY_LEFT: return Key::Left;
            case GLFW_KEY_DOWN: return Key::Down;
            case GLFW_KEY_UP: return Key::Up;
            case GLFW_KEY_PAGE_UP: return Key::PageUp;
            case GLFW_KEY_PAGE_DOWN: return Key::PageDown;
            case GLFW_KEY_HOME: return Key::Home;
            case GLFW_KEY_END: return Key::End;
            case GLFW_KEY_CAPS_LOCK: return Key::CapsLock;
            case GLFW_KEY_SCROLL_LOCK: return Key::ScrollLock;
            case GLFW_KEY_NUM_LOCK: return Key::NumLock;
            case GLFW_KEY_PRINT_SCREEN: return Key::PrintScreen;
            case GLFW_KEY_PAUSE: return Key::Pause;
            case GLFW_KEY_F1: return Key::F1;
            case GLFW_KEY_F2: return Key::F2;
            case GLFW_KEY_F3: return Key::F3;
            case GLFW_KEY_F4: return Key::F4;
            case GLFW_KEY_F5: return Key::F5;
            case GLFW_KEY_F6: return Key::F6;
            case GLFW_KEY_F7: return Key::F7;
            case GLFW_KEY_F8: return Key::F8;
            case GLFW_KEY_F9: return Key::F9;
            case GLFW_KEY_F10: return Key::F10;
            case GLFW_KEY_F11: return Key::F11;
            case GLFW_KEY_F12: return Key::F12;
            case GLFW_KEY_F13: return Key::F13;
            case GLFW_KEY_F14: return Key::F14;
            case GLFW_KEY_F15: return Key::F15;
            case GLFW_KEY_F16: return Key::F16;
            case GLFW_KEY_F17: return Key::F17;
            case GLFW_KEY_F18: return Key::F18;
            case GLFW_KEY_F19: return Key::F19;
            case GLFW_KEY_F20: return Key::F20;
            case GLFW_KEY_F21: return Key::F21;
            case GLFW_KEY_F22: return Key::F22;
            case GLFW_KEY_F23: return Key::F23;
            case GLFW_KEY_F24: return Key::F24;
            case GLFW_KEY_F25: return Key::F25;
            case GLFW_KEY_KP_0: return Key::Np0;
            case GLFW_KEY_KP_1: return Key::Np1;
            case GLFW_KEY_KP_2: return Key::Np2;
            case GLFW_KEY_KP_3: return Key::Np3;
            case GLFW_KEY_KP_4: return Key::Np4;
            case GLFW_KEY_KP_5: return Key::Np5;
            case GLFW_KEY_KP_6: return Key::Np6;
            case GLFW_KEY_KP_7: return Key::Np7;
            case GLFW_KEY_KP_8: return Key::Np8;
            case GLFW_KEY_KP_9: return Key::Np9;
            case GLFW_KEY_KP_DECIMAL: return Key::NpDecimal;
            case GLFW_KEY_KP_DIVIDE: return Key::NpDivide;
            case GLFW_KEY_KP_MULTIPLY: return Key::NpMultiply;
            case GLFW_KEY_KP_SUBTRACT: return Key::NpSubtract;
            case GLFW_KEY_KP_ADD: return Key::NpAdd;
            case GLFW_KEY_KP_ENTER: return Key::NpReturn;
            case GLFW_KEY_KP_EQUAL: return Key::NpEqual;
            case GLFW_KEY_LEFT_SHIFT: return Key::LShift;
            case GLFW_KEY_LEFT_CONTROL: return Key::LControl;
            case GLFW_KEY_LEFT_ALT: return Key::LAlt;
            case GLFW_KEY_LEFT_SUPER: return Key::LSuper;
            case GLFW_KEY_RIGHT_SHIFT: return Key::RShift;
            case GLFW_KEY_RIGHT_CONTROL: return Key::RControl;
            case GLFW_KEY_RIGHT_ALT: return Key::RAlt;
            case GLFW_KEY_RIGHT_SUPER: return Key::RSuper;
            case GLFW_KEY_MENU: return Key::Menu;
            default: return Key::None;
        }
    }

    inline MouseButton MouseButtonFromGlfwType(const i32 button) {
        switch (button) {
            case GLFW_MOUSE_BUTTON_1: return MouseButton::Mouse0;
            case GLFW_MOUSE_BUTTON_2: return MouseButton::Mouse1;
            case GLFW_MOUSE_BUTTON_3: return MouseButton::Mouse2;
            case GLFW_MOUSE_BUTTON_4: return MouseButton::Mouse3;
            case GLFW_MOUSE_BUTTON_5: return MouseButton::Mouse4;
            case GLFW_MOUSE_BUTTON_6: return MouseButton::Mouse5;
            case GLFW_MOUSE_BUTTON_7: return MouseButton::Mouse6;
            case GLFW_MOUSE_BUTTON_8: return MouseButton::Mouse7;
            default: return MouseButton::None;
        }
    }

    inline Action ActionFromGlfwType(const i32 action) {
        switch (action) {
            case GLFW_PRESS: return Action::Press;
            case GLFW_RELEASE: return Action::Release;
            case GLFW_REPEAT: return Action::Repeat;
            default: return Action::None;
        }
    }

    class FLK_API KeyEvent : public Event::Event {
    public:
        Key    key;
        Action action;

        KeyEvent(const Key key, const Action action) : key(key), action(action) {
        }
    };

    class FLK_API MouseButtonEvent : public Event::Event {
    public:
        MouseButton button;
        Action      action;

        MouseButtonEvent(const MouseButton button, const Action action) : button(button), action(action) {
        }
    };

    class FLK_API CursorPositionEvent : public Event::Event {
    public:
        Vector2f position;

        CursorPositionEvent(const Vector2f position) : position(position) {
        }
    };

    class FLK_API MouseScrollEvent : public Event::Event {
    public:
        Vector2f offset;

        MouseScrollEvent(const Vector2f offset) : offset(offset) {
        }
    };

    enum class CursorMode {
        Normal,
        Captured,
        Disabled,
        Hidden
    };

    inline i32 ToGlfwType(const CursorMode cursorMode) {
        switch (cursorMode) {
            case CursorMode::Normal:
                return GLFW_CURSOR_NORMAL;
            case CursorMode::Captured:
                return GLFW_CURSOR_CAPTURED;
            case CursorMode::Disabled:
                return GLFW_CURSOR_DISABLED;
            case CursorMode::Hidden:
                return GLFW_CURSOR_HIDDEN;
            default:
                FLK_ASSERT(false);
        }
    }

    struct FLK_API InputState {
        std::set<Key> heldKeys;
        std::set<Key> pressedKeys;
        std::set<Key> releasedKeys;

        std::set<MouseButton> heldMouseButtons;
        std::set<MouseButton> pressedMouseButtons;
        std::set<MouseButton> releasedMouseButtons;

        CursorMode cursorMode = CursorMode::Normal;

        Vector2f cursorPosition;
        Vector2f cursorDelta;
        Vector2f scrollOffset;

        bool IsKeyPressed(const Key key) const {
            return pressedKeys.contains(key);
        }

        bool IsKeyReleased(const Key key) const {
            return releasedKeys.contains(key);;
        }

        bool IsKeyDown(const Key key) const {
            return heldKeys.contains(key);;
        }

        bool IsKeyUp(const Key key) const {
            return !heldKeys.contains(key);;
        }

        bool IsMousePressed(const MouseButton button = MouseButton::Left) const {
            return pressedMouseButtons.contains(button);
        }

        bool IsMouseReleased(const MouseButton button = MouseButton::Left) const {
            return releasedMouseButtons.contains(button);
        }

        bool IsMouseDown(const MouseButton button = MouseButton::Left) const {
            return heldMouseButtons.contains(button);
        }

        bool IsMouseUp(const MouseButton button = MouseButton::Left) const {
            return !heldMouseButtons.contains(button);
        }

        bool IsCursorInRect(Rect2f rect) const {
            auto [x, y] = rect.origin;
            auto [w, h] = rect.aspect;

            return cursorPosition.x >= x && cursorPosition.x < x + w && cursorPosition.y >= y && cursorPosition.y < y + h;
        }

        Vector2f GetCursorPosition() const {
            return cursorPosition;
        }

        Vector2f GetCursorDelta() const {
            return cursorDelta;
        }

        Vector2f GetScrollOffset() const {
            return scrollOffset;
        }
    };
}

#endif //FLK_INPUT_HPP
