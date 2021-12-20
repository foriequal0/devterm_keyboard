#include "keymap.hpp"

#include <cstddef>

#include <USBComposite.h>

struct KeyMap {
  KeyCode matrix[MatrixKey::ROW_NUM][MatrixKey::COL_NUM];
  KeyCode special[(size_t)SpecialKey::NUM];
};

// Naming rule
// - alphanumeric keys: _{code},
// - key that has a name: use its name.
// - other keys: {row:A, B, C, D, ...}{column: 1,2,3,...}
#define LAYOUT( \
  JOYSTICK_UP, JOYSTICK_DOWN, JOYSTICK_LEFT, JOYSTICK_RIGHT, \
  JOYSTICK_Y, JOYSTICK_X, JOYSTICK_B, JOYSTICK_A, \
  A1, A2, A3, A4, BALL, A5, A6, A7, A8, \
  _1, _2, _3, _4, _5, _6, _7, _8, _9, _0, \
  ESC, _Q, _W, _E, _R, _T, _Y, _U, _I, _O, _P, C12, BACKSP, \
  TAB,  _A, _S, _D, _F, _G, _H, _J, _K, _L, D11, D12, RETURN, \
  LSHIFT, _Z, _X, _C, _V, _B, _N, _M, E9, E10, E11, UP, RSHIFT, \
  FN, LCTRL, CMD, LALT, SPACE, RALT, RCTRL, LEFT, DOWN, RIGHT, \
  MOUSE_L, MOUSE_M, MOUSE_R \
) (KeyMap { \
  .matrix = { \
    {  A1,  A2,  A3,  A4,  A5,  A6,  A7,  A8 }, \
    {  _1,  _2,  _3,  _4,  _5,  _6,  _7,  _8 }, \
    {  _9,  _0, ESC, TAB,  UP, DOWN, LEFT, RIGHT }, \
    {  _Q,  _W,  _E,  _R,  _T,  _Y,  _U,  _I }, \
    {  _O,  _P,  _A,  _S,  _D,  _F,  _G,  _H }, \
    {  _J,  _K,  _L,  _Z,  _X,  _C,  _V,  _B }, \
    {  _N,  _M,  E9, E10, E11, C12, D11, D12 }, \
    { BACKSP, RETURN, RALT, RCTRL, RSHIFT, SPACE, _, _}, \
  }, \
  .special = { \
    JOYSTICK_UP, \
    JOYSTICK_DOWN, \
    JOYSTICK_LEFT, \
    JOYSTICK_RIGHT, \
    JOYSTICK_A, \
    JOYSTICK_B, \
    JOYSTICK_X, \
    JOYSTICK_Y, \
    LSHIFT, \
    FN, \
    LCTRL, \
    CMD, \
    LALT, \
    MOUSE_L, \
    MOUSE_M, \
    MOUSE_R, \
    BALL, \
  }, \
})

static const KeyCode _ { KeyCode::Type::NoOp, 0 };

static KeyCode key(uint8_t code) {
  return { KeyCode::Type::Key, code, };
}

static KeyCode consumer(uint8_t code) {
  return { KeyCode::Type::Consumer, code, };
}

static KeyCode custom(CustomKeyCode action) {
  return { KeyCode::Type::Custom, (uint8_t)action, };
}

static KeyCode joystick(uint8_t code) {
  return { KeyCode::Type::JoystickButton, code, };
}

static KeyCode mouse(uint8_t code) {
  return { KeyCode::Type::MouseButton, code, };
}

static const uint8_t KEY_PRNT_SCRN = 0xCE; //Print screen - 0x88 == usb hut1_12v2.pdf keyboard code
static const uint8_t KEY_PAUSE = 0xd0; // - 0x88 == usb hut1_12v2.pdf keyboard code

static const KeyMap defaultKeyMap = 
LAYOUT(
  custom(CustomKeyCode::JoystickUp), custom(CustomKeyCode::JoystickDown), custom(CustomKeyCode::JoystickLeft), custom(CustomKeyCode::JoystickRight),
  joystick(3), joystick(4), joystick(1), joystick(2),
  joystick(9), joystick(10), consumer(HIDConsumer::VOLUME_DOWN), key('`'), mouse(MOUSE_LEFT), key('['), key(']'), key('-'), key('='),
  key('1'), key('2'), key('3'), key('4'), key('5'), key('6'), key('7'), key('8'), key('9'), key('0'),
  key(KEY_ESC), key('q'), key('w'), key('e'), key('r'), key('t'), key('y'), key('u'), key('i'), key('o'), key('p'), key('\\'), key(KEY_BACKSPACE),
  key(KEY_TAB), key('a'), key('s'), key('d'), key('f'), key('g'), key('h'), key('j'), key('k'), key('l'), key(';'), key('\''), key(KEY_RETURN),
  key(KEY_LEFT_SHIFT), key('z'), key('x'), key('c'), key('v'), key('b'), key('n'), key('m'), key(','), key('.'), key('/'), key(KEY_UP_ARROW), key(KEY_RIGHT_SHIFT),
  custom(CustomKeyCode::Fn), key(KEY_LEFT_CTRL), key(KEY_LEFT_GUI), key(KEY_LEFT_ALT), key(' '), key(KEY_RIGHT_ALT), key(KEY_RIGHT_CTRL), key(KEY_LEFT_ARROW), key(KEY_DOWN_ARROW), key(KEY_RIGHT_ARROW),
  mouse(MOUSE_LEFT), custom(CustomKeyCode::MiddleClick), mouse(MOUSE_RIGHT)
);

static const KeyMap fnKeyMap = 
LAYOUT(
  _, _, _, _,
  _, _, _, _,
  key(KEY_PRNT_SCRN), key(KEY_PAUSE), consumer(HIDConsumer::VOLUME_UP), _, _, _, _, key(KEY_F11), key(KEY_F12),
  key(KEY_F1), key(KEY_F2), key(KEY_F3), key(KEY_F4), key(KEY_F5), key(KEY_F6), key(KEY_F7), key(KEY_F8), key(KEY_F9), key(KEY_F10),
  _, _, _, _, _, _, _, _, key(KEY_INSERT), _, _, _, key(KEY_DELETE),
  key(KEY_CAPS_LOCK), _, _, _, _, _, _, _, _, _, _, _, _,
  _, _, _, _, _, _, _, _, consumer(HIDConsumer::BRIGHTNESS_DOWN), consumer(HIDConsumer::BRIGHTNESS_UP), _, key(KEY_PAGE_UP), _,
  _, _, _, _, _, _, _, key(KEY_HOME), key(KEY_PAGE_DOWN), key(KEY_END),
  _, _, _
);

static KeyCode getKeyCode(const KeyMap& keymap, Key key) {
  if (key.tag() == Key::Tag::Matrix) {
    const auto matrix = key.matrix();
    return keymap.matrix[matrix.row][matrix.col];
  } else {
    const auto special = key.special();
    return keymap.special[special.value];
  }
}

KeyCodeResult getKeyCode(Key key, bool fn) {
  if (fn) {
    const auto keyCode = getKeyCode(fnKeyMap, key);
    if (keyCode.type != KeyCode::Type::NoOp) {
      return KeyCodeResult {
        .keyCode = keyCode,
        .fromFn = true,
      };
    }
  }

  return KeyCodeResult {
    .keyCode = getKeyCode(defaultKeyMap, key),
    .fromFn = false,
  };
}
