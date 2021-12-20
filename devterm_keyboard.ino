#include <USBComposite.h>

#include "src/device.hpp"
#include "src/keyboard.hpp"
#include "src/state.hpp"
#include "src/tickwaiter.hpp"
#include "src/trackball.hpp"

static Device device;
static State state;

void setup() {
  device.registerComponents();

  USBComposite.setManufacturerString("ClockworkPI");
  USBComposite.setProductString("DevTerm Keyboard");
  USBComposite.begin();

  while (!USBComposite)
    ; // wait until usb port been plugged in to PC

  setupKeyboard();
  setupTrackball();

  delay(500);
}

static const uint32_t LOOP_INTERVAL_MS = 10;
static TickWaiter<LOOP_INTERVAL_MS> waiter;

void loop() {
  const auto delta = waiter.waitForNextTick();
  state.tick(delta);
  tickKeyboard(delta, device, state);
  tickTrackball(delta, device, state);
}
