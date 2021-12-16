#ifndef DEVICE_HPP
#define DEVICE_HPP

#include <USBComposite.h>

class Device {
  public:
    USBCompositeSerial serial;

    Device();

    void registerComponents();

    void setKey(uint8_t key, bool value);

    void moveMouse(int8_t x, int8_t y, int8_t w);
    void setMouseButton(uint8_t button, bool value);
    void clickMouseButton(uint8_t buton);

    void setJoystickButton(uint8_t button, bool value);
    void setJoystickX(uint16_t value);
    void setJoystickY(uint16_t value);

    void setConsumerButton(uint8_t buton, bool value);

  private:
    USBHID hid;
    HIDKeyboard keyboard;
    HIDMouse mouse;
    HIDJoystick joystick;
    HIDConsumer consumer;
};

#endif
