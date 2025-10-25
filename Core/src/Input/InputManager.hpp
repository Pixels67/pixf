#pragma once

#include "../Common.hpp"
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

    enum class KeyAction : uint8_t {
        Press = GLFW_PRESS,
        Release = GLFW_RELEASE,
    };

    struct KeyEvent final : Event::Event {
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

    struct MouseKeyEvent final : Event::Event {
        MouseKey key;
        KeyAction action;

        explicit MouseKeyEvent(const MouseKey key, const KeyAction action) : key(key), action(action) {}
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
    };

} // namespace Pixf::Core::Input
