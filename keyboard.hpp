#ifndef KEYBOARD_HPP
#define KEYBOARD_HPP

#include "device.hpp"
#include "state.hpp"
#include "debouncer.hpp"

void setupKeyboard();
void tickKeyboard(millis_t tick, Device& device, State& state);

#endif