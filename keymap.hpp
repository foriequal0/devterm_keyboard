#ifndef KEYMAP_HPP
#define KEYMAP_HPP

#include <cstdint>

#include "key.hpp"

struct KeyCode {
  enum class Type : uint8_t {
    NoOp,

    Key,
    Consumer,
    JoystickButton,
    MouseButton,

    Custom,
  };

  Type type;
  uint8_t value;
};

enum class CustomKeyCode : uint8_t {
  Fn,
  JoystickUp,
  JoystickDown,
  JoystickLeft,
  JoystickRight,
  MiddleClick,
};

struct KeyCodeResult {
  KeyCode keyCode;
  bool fromFn;
};

KeyCodeResult getKeyCode(Key key, bool fn);

#endif
