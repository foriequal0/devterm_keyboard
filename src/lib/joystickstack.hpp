#ifndef JOYSTICKSTACK_HPP
#define JOYSTICKSTACK_HPP

#include <cstdint>

class JoystickStack {
public:
  JoystickStack();
  uint16_t a(bool pressed);
  uint16_t b(bool pressed);

private:
  uint8_t stack;
};

#endif