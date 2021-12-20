#include "device.hpp"

Device::Device()
  : keyboard(hid),
    mouse(hid),
    joystick(hid),
    consumer(hid) {
  keyboard.setAdjustForHostCapsLock(false);
}

void Device::registerComponents() {
  hid.registerComponent();
  serial.registerComponent();
}

void Device::setKey(uint8_t key, bool value) {
  if (value) {
    keyboard.press(key);
  } else {
    keyboard.release(key);
  }
  if (key == KEY_CAPS_LOCK) {
    keyboard.setAdjustForHostCapsLock(value);
  }
}

void Device::moveMouse(int8_t x, int8_t y, int8_t w) {
  if (x != 0 || y != 0 || w != 0) {
    mouse.move(x, y, w);
  }
}

void Device::setMouseButton(uint8_t button, bool value) {
  if (value) {
    mouse.press(button);
  } else {
    mouse.release(button);
  }
}

void Device::clickMouseButton(uint8_t button) {
  mouse.click(button);
}

void Device::setJoystickButton(uint8_t button, bool value) {
  joystick.button(button, value);
}

void Device::setJoystickX(uint16_t value) {
  joystick.X(value);
}

void Device::setJoystickY(uint16_t value) {
  joystick.Y(value);
}

void Device::setConsumerButton(uint8_t button, bool value) {
  if (value) {
    consumer.press(button);
  } else {
    consumer.release();
  }
}
