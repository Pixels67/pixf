#pragma once

#include "entity_manager.h"
#include "event/event.h"
#include "ui/render_window.h"
#include "vec2.hpp"

/* Printable keys */
#define GLFW_KEY_SPACE 32
#define GLFW_KEY_APOSTROPHE 39 /* ' */
#define GLFW_KEY_COMMA 44      /* , */
#define GLFW_KEY_MINUS 45      /* - */
#define GLFW_KEY_PERIOD 46     /* . */
#define GLFW_KEY_SLASH 47      /* / */
#define GLFW_KEY_0 48
#define GLFW_KEY_1 49
#define GLFW_KEY_2 50
#define GLFW_KEY_3 51
#define GLFW_KEY_4 52
#define GLFW_KEY_5 53
#define GLFW_KEY_6 54
#define GLFW_KEY_7 55
#define GLFW_KEY_8 56
#define GLFW_KEY_9 57
#define GLFW_KEY_SEMICOLON 59 /* ; */
#define GLFW_KEY_EQUAL 61     /* = */
#define GLFW_KEY_A 65
#define GLFW_KEY_B 66
#define GLFW_KEY_C 67
#define GLFW_KEY_D 68
#define GLFW_KEY_E 69
#define GLFW_KEY_F 70
#define GLFW_KEY_G 71
#define GLFW_KEY_H 72
#define GLFW_KEY_I 73
#define GLFW_KEY_J 74
#define GLFW_KEY_K 75
#define GLFW_KEY_L 76
#define GLFW_KEY_M 77
#define GLFW_KEY_N 78
#define GLFW_KEY_O 79
#define GLFW_KEY_P 80
#define GLFW_KEY_Q 81
#define GLFW_KEY_R 82
#define GLFW_KEY_S 83
#define GLFW_KEY_T 84
#define GLFW_KEY_U 85
#define GLFW_KEY_V 86
#define GLFW_KEY_W 87
#define GLFW_KEY_X 88
#define GLFW_KEY_Y 89
#define GLFW_KEY_Z 90
#define GLFW_KEY_LEFT_BRACKET 91  /* [ */
#define GLFW_KEY_BACKSLASH 92     /* \ */
#define GLFW_KEY_RIGHT_BRACKET 93 /* ] */
#define GLFW_KEY_GRAVE_ACCENT 96  /* ` */
#define GLFW_KEY_WORLD_1 161      /* non-US #1 */
#define GLFW_KEY_WORLD_2 162      /* non-US #2 */

/* Function keys */
#define GLFW_KEY_ESCAPE 256
#define GLFW_KEY_ENTER 257
#define GLFW_KEY_TAB 258
#define GLFW_KEY_BACKSPACE 259
#define GLFW_KEY_INSERT 260
#define GLFW_KEY_DELETE 261
#define GLFW_KEY_RIGHT 262
#define GLFW_KEY_LEFT 263
#define GLFW_KEY_DOWN 264
#define GLFW_KEY_UP 265
#define GLFW_KEY_PAGE_UP 266
#define GLFW_KEY_PAGE_DOWN 267
#define GLFW_KEY_HOME 268
#define GLFW_KEY_END 269
#define GLFW_KEY_CAPS_LOCK 280
#define GLFW_KEY_SCROLL_LOCK 281
#define GLFW_KEY_NUM_LOCK 282
#define GLFW_KEY_PRINT_SCREEN 283
#define GLFW_KEY_PAUSE 284
#define GLFW_KEY_F1 290
#define GLFW_KEY_F2 291
#define GLFW_KEY_F3 292
#define GLFW_KEY_F4 293
#define GLFW_KEY_F5 294
#define GLFW_KEY_F6 295
#define GLFW_KEY_F7 296
#define GLFW_KEY_F8 297
#define GLFW_KEY_F9 298
#define GLFW_KEY_F10 299
#define GLFW_KEY_F11 300
#define GLFW_KEY_F12 301
#define GLFW_KEY_F13 302
#define GLFW_KEY_F14 303
#define GLFW_KEY_F15 304
#define GLFW_KEY_F16 305
#define GLFW_KEY_F17 306
#define GLFW_KEY_F18 307
#define GLFW_KEY_F19 308
#define GLFW_KEY_F20 309
#define GLFW_KEY_F21 310
#define GLFW_KEY_F22 311
#define GLFW_KEY_F23 312
#define GLFW_KEY_F24 313
#define GLFW_KEY_F25 314
#define GLFW_KEY_KP_0 320
#define GLFW_KEY_KP_1 321
#define GLFW_KEY_KP_2 322
#define GLFW_KEY_KP_3 323
#define GLFW_KEY_KP_4 324
#define GLFW_KEY_KP_5 325
#define GLFW_KEY_KP_6 326
#define GLFW_KEY_KP_7 327
#define GLFW_KEY_KP_8 328
#define GLFW_KEY_KP_9 329
#define GLFW_KEY_KP_DECIMAL 330
#define GLFW_KEY_KP_DIVIDE 331
#define GLFW_KEY_KP_MULTIPLY 332
#define GLFW_KEY_KP_SUBTRACT 333
#define GLFW_KEY_KP_ADD 334
#define GLFW_KEY_KP_ENTER 335
#define GLFW_KEY_KP_EQUAL 336
#define GLFW_KEY_LEFT_SHIFT 340
#define GLFW_KEY_LEFT_CONTROL 341
#define GLFW_KEY_LEFT_ALT 342
#define GLFW_KEY_LEFT_SUPER 343
#define GLFW_KEY_RIGHT_SHIFT 344
#define GLFW_KEY_RIGHT_CONTROL 345
#define GLFW_KEY_RIGHT_ALT 346
#define GLFW_KEY_RIGHT_SUPER 347
#define GLFW_KEY_MENU 348

#define GLFW_RELEASE 0
/*! @brief The key or mouse button was pressed.
 *
 *  The key or mouse button was pressed.
 *
 *  @ingroup input
 */
#define GLFW_PRESS 1

#define GLFW_MOUSE_BUTTON_1 0
#define GLFW_MOUSE_BUTTON_2 1
#define GLFW_MOUSE_BUTTON_3 2
#define GLFW_MOUSE_BUTTON_4 3
#define GLFW_MOUSE_BUTTON_5 4
#define GLFW_MOUSE_BUTTON_6 5
#define GLFW_MOUSE_BUTTON_7 6
#define GLFW_MOUSE_BUTTON_8 7

namespace pixf::input {

enum class Key : uint16_t {
  SPACE = GLFW_KEY_SPACE,
  APOSTROPHE = GLFW_KEY_APOSTROPHE,
  COMMA = GLFW_KEY_COMMA,
  MINUS = GLFW_KEY_MINUS,
  PERIOD = GLFW_KEY_PERIOD,
  SLASH = GLFW_KEY_SLASH,
  ALPHA0 = GLFW_KEY_0,
  ALPHA1 = GLFW_KEY_1,
  ALPHA2 = GLFW_KEY_2,
  ALPHA3 = GLFW_KEY_3,
  ALPHA4 = GLFW_KEY_4,
  ALPHA5 = GLFW_KEY_5,
  ALPHA6 = GLFW_KEY_6,
  ALPHA7 = GLFW_KEY_7,
  ALPHA8 = GLFW_KEY_8,
  ALPHA9 = GLFW_KEY_9,
  SEMICOLON = GLFW_KEY_SEMICOLON,
  EQUAL = GLFW_KEY_EQUAL,
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
  L_BRACKET = GLFW_KEY_LEFT_BRACKET,
  BACKSLASH = GLFW_KEY_BACKSLASH,
  R_BRACKET = GLFW_KEY_RIGHT_BRACKET,
  GRAVE_ACCENT = GLFW_KEY_GRAVE_ACCENT,
  WORLD1 = GLFW_KEY_WORLD_1,
  WORLD2 = GLFW_KEY_WORLD_2,
  ESCAPE = GLFW_KEY_ESCAPE,
  RETURN = GLFW_KEY_ENTER,
  TAB = GLFW_KEY_TAB,
  BACKSPACE = GLFW_KEY_BACKSPACE,
  INSERT = GLFW_KEY_INSERT,
  DELETE = GLFW_KEY_DELETE,
  RIGHT = GLFW_KEY_RIGHT,
  LEFT = GLFW_KEY_LEFT,
  DOWN = GLFW_KEY_DOWN,
  UP = GLFW_KEY_UP,
  PAGE_UP = GLFW_KEY_PAGE_UP,
  PAGE_DOWN = GLFW_KEY_PAGE_DOWN,
  HOME = GLFW_KEY_HOME,
  END = GLFW_KEY_END,
  CAPS_LOCK = GLFW_KEY_CAPS_LOCK,
  SCROLL_LOCK = GLFW_KEY_SCROLL_LOCK,
  NUM_LOCK = GLFW_KEY_NUM_LOCK,
  PRINT_SCREEN = GLFW_KEY_PRINT_SCREEN,
  PAUSE = GLFW_KEY_PAUSE,
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
  KP0 = GLFW_KEY_KP_0,
  KP1 = GLFW_KEY_KP_1,
  KP2 = GLFW_KEY_KP_2,
  KP3 = GLFW_KEY_KP_3,
  KP4 = GLFW_KEY_KP_4,
  KP5 = GLFW_KEY_KP_5,
  KP6 = GLFW_KEY_KP_6,
  KP7 = GLFW_KEY_KP_7,
  KP8 = GLFW_KEY_KP_8,
  KP9 = GLFW_KEY_KP_9,
  KP_DECIMAL = GLFW_KEY_KP_DECIMAL,
  KP_DIVIDE = GLFW_KEY_KP_DIVIDE,
  KP_MULTIPLY = GLFW_KEY_KP_MULTIPLY,
  KP_SUBTRACT = GLFW_KEY_KP_SUBTRACT,
  KP_ADD = GLFW_KEY_KP_ADD,
  KP_RETURN = GLFW_KEY_KP_ENTER,
  KP_EQUAL = GLFW_KEY_KP_EQUAL,
  L_SHIFT = GLFW_KEY_LEFT_SHIFT,
  L_CTRL = GLFW_KEY_LEFT_CONTROL,
  L_ALT = GLFW_KEY_LEFT_ALT,
  L_SUPER = GLFW_KEY_LEFT_SUPER,
  R_SHIFT = GLFW_KEY_RIGHT_SHIFT,
  R_CTRL = GLFW_KEY_RIGHT_CONTROL,
  R_ALT = GLFW_KEY_RIGHT_ALT,
  R_SUPER = GLFW_KEY_RIGHT_SUPER,
  MENU = GLFW_KEY_MENU,
};

enum class KeyAction : uint8_t {
  PRESS = GLFW_PRESS,
  RELEASE = GLFW_RELEASE,
};

struct KeyEvent final : event::Event {
  Key key;
  KeyAction action;

  explicit KeyEvent(const Key key, const KeyAction action) : key(key), action(action) {}
};

enum class MouseKey : uint8_t {
  MOUSE1 = GLFW_MOUSE_BUTTON_1,
  MOUSE2 = GLFW_MOUSE_BUTTON_2,
  MOUSE3 = GLFW_MOUSE_BUTTON_3,
  MOUSE4 = GLFW_MOUSE_BUTTON_4,
  MOUSE5 = GLFW_MOUSE_BUTTON_5,
  MOUSE6 = GLFW_MOUSE_BUTTON_6,
  MOUSE7 = GLFW_MOUSE_BUTTON_7,
  MOUSE8 = GLFW_MOUSE_BUTTON_8,
};

struct MouseKeyEvent final : event::Event {
  MouseKey key;
  KeyAction action;

  explicit MouseKeyEvent(const MouseKey key, const KeyAction action) : key(key), action(action) {}
};

struct InputManager final : Component {
  InputManager() = default;
  explicit InputManager(event::EventManager* event_manager, ui::RenderWindow* window);

  bool IsKeyDown(Key key) const;
  bool IsKeyUp(Key key) const;

  bool IsMouseKeyDown(MouseKey key) const;
  bool IsMouseKeyUp(MouseKey key) const;

  glm::dvec2 GetMousePosition() const;
  glm::dvec2 GetMouseDelta() const;

  void Update(ui::RenderWindow& window);

 private:
  glm::dvec2 mouse_pos_ = {};
  glm::dvec2 mouse_delta_ = {};
  event::EventManager* event_manager_ = nullptr;
  ui::RenderWindow* window_ = nullptr;
};

}  // namespace pixf::input