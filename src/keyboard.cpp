#include "keyboard.hpp"

#include "lib/joystickstack.hpp"
#include "lib/key.hpp"
#include "lib/keyset.hpp"
#include "lib/rollover.hpp"

#include "keymap.hpp"

#include <Arduino.h>

static const uint8_t ROWS[MatrixKey::ROW_NUM] = {PA0, PA1, PA2, PA3,
                                                 PA4, PA5, PA6, PA7};
static const uint8_t COLS[MatrixKey::COL_NUM] = {PC0, PC1, PC2, PC3,
                                                 PC4, PC5, PC6, PC7};

static const uint8_t KEY_PINS[SpecialKey::NUM] = {
    PB0,  // Joystick up
    PB1,  // Joystick down
    PB2,  // Joystick left
    PB3,  // Joystick right
    PB4,  // Joystick A
    PB5,  // Joystick B
    PB6,  // Joystick X
    PB7,  // Joystick Y
    PB8,  // Left shift
    PB9,  // Fn
    PB10, // Left ctrl
    PB11, // CMD
    PB12, // Left alt
    PB13, // Mouse left
    PB14, // Mouse middle
    PB15, // Mouse right
    PC12, // Trackball click
};

struct KeySets {
  MatrixKeySet matrix;
  SpecialKeySet special;
};

static Debouncer debouncer;
static KeySets keys;
static KeySets snapshot;

static void updateKey(MatrixKey key, bool value) {
  if (keys.matrix.get(key) != value && debouncer.sample(value)) {
    keys.matrix.update(key, value);
  }
}

static void updateKey(SpecialKey key, bool value) {
  if (keys.special.get(key) != value && debouncer.sample(value)) {
    keys.special.update(key, value);
  }
}

static const size_t FN_KEY_ROLLOVER = 4;
static Rollover<FN_KEY_ROLLOVER> pressedWithFn;

static const size_t MIDDLE_CLICK_TIMEOUT_MS = 500;
static Timeout<uint16_t, MIDDLE_CLICK_TIMEOUT_MS> middleClickTimeout;

void setupKeyboard() {
  for (const auto row : ROWS) {
    pinMode(row, OUTPUT_OPEN_DRAIN);
    digitalWrite(row, HIGH);
  }

  for (const auto col : COLS) {
    pinMode(col, INPUT_PULLUP);
  }

  for (const auto pin : KEY_PINS) {
    pinMode(pin, INPUT_PULLUP);
  }
}

static void scan() {
  for (uint8_t row = 0; row < MatrixKey::ROW_NUM; row++) {
    digitalWrite(ROWS[row], LOW);
    delay(1); // wait for settle
    for (uint8_t col = 0; col < MatrixKey::COL_NUM; col++) {
      MatrixKey key{row, col};
      const auto value = !digitalRead(COLS[col]);
      updateKey(key, value);
    }
    digitalWrite(ROWS[row], HIGH);
  }

  for (uint8_t i = 0; i < SpecialKey::NUM; i++) {
    SpecialKey key{i};
    const auto value = !digitalRead(KEY_PINS[i]);
    updateKey(key, value);
  }
}

static JoystickStack joystickX, joystickY;

static void handleTransition(Device &device, State &state, Key key,
                             bool pressed) {
  // Fn key is pressed, or the key was registered with the fn key
  const auto fn = state.fn || pressedWithFn.get(key);
  const auto keyCodeResult = getKeyCode(key, fn);
  const auto type = keyCodeResult.keyCode.type;
  const auto value = keyCodeResult.keyCode.value;
  const auto fromFn = keyCodeResult.fromFn;

  if (fromFn) {
    pressedWithFn.set(key, pressed);
  }

  switch (type) {
  case KeyCode::Type::Key:
    device.setKey(value, pressed);
    return;
  case KeyCode::Type::Consumer:
    device.setConsumerButton(value, pressed);
    return;
  case KeyCode::Type::JoystickButton:
    device.setJoystickButton(value, pressed);
    return;
  case KeyCode::Type::MouseButton:
    device.setMouseButton(value, pressed);
    return;
  case KeyCode::Type::Custom:
    break; // see below
  default:
    return;
  }

  switch ((CustomKeyCode)value) {
  case CustomKeyCode::Fn:
    state.fn = pressed;
    break;
  case CustomKeyCode::JoystickUp: {
    device.setJoystickY(joystickY.a(pressed));
    break;
  }
  case CustomKeyCode::JoystickDown: {
    device.setJoystickY(joystickY.b(pressed));
    break;
  }
  case CustomKeyCode::JoystickLeft: {
    device.setJoystickX(joystickX.a(pressed));
    break;
  }
  case CustomKeyCode::JoystickRight: {
    device.setJoystickX(joystickX.b(pressed));
    break;
  }
  case CustomKeyCode::MiddleClick:
    if (pressed) {
      state.pressMiddleClick();
    } else if (state.releaseMiddleClick()) {
      device.clickMouseButton(MOUSE_MIDDLE);
    }
    break;
  }
}

void tickKeyboard(millis_t delta, Device &device, State &state) {
  debouncer.updateTime(delta);

  snapshot = keys;
  scan();

  // Handle key release first
  for (auto key : snapshot.matrix) {
    if (!keys.matrix.get(key)) {
      handleTransition(device, state, key, false);
    }
  }
  for (auto key : snapshot.special) {
    if (!keys.special.get(key)) {
      handleTransition(device, state, key, false);
    }
  }

  // Handle key press later
  for (auto key : keys.matrix) {
    if (!snapshot.matrix.get(key)) {
      handleTransition(device, state, key, true);
    }
  }
  for (auto key : keys.special) {
    if (!snapshot.special.get(key)) {
      handleTransition(device, state, key, true);
    }
  }
}
