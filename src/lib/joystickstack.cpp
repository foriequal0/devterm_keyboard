#include "joystickstack.hpp"

#include <cassert>

static const uint8_t IDLE = 0b00;
static const uint8_t A = 0b01;
static const uint8_t B = 0b10;
static const uint8_t AB = A << 2 | B;
static const uint8_t BA = B << 2 | A;

static uint16_t value(uint8_t state) {
  switch (state) {
  case A:
    return 0;
  case B:
    return 1023;
  default:
    return 512;
  }
}

JoystickStack::JoystickStack() : stack(IDLE) {}

uint16_t JoystickStack::a(bool pressed) {
  if (pressed) {
    assert(stack == IDLE || stack == B);
    stack = stack << 2 | A;
    return value(A);
  } else {
    switch (stack) {
    case BA:
    case AB:
      stack = B;
      return value(B);
    default:
      stack = IDLE;
      return value(IDLE);
    }
  }
}

uint16_t JoystickStack::b(bool pressed) {
  if (pressed) {
    assert(stack == IDLE || stack == A);
    stack = stack << 2 | B;
    return value(A);
  } else {
    switch (stack) {
    case AB:
    case BA:
      stack = A;
      return value(A);
    default:
      stack = IDLE;
      return value(IDLE);
    }
  }
}
