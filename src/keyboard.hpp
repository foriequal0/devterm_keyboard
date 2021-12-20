#ifndef KEYBOARD_HPP
#define KEYBOARD_HPP

#include "lib/debouncer.hpp"

#include "device.hpp"
#include "state.hpp"

void setupKeyboard();
void tickKeyboard(millis_t tick, Device& device, State& state);

#endif