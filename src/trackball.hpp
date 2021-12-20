#ifndef TRACKBALL_HPP
#define TRACKBALL_HPP

#include "lib/debouncer.hpp"

#include "device.hpp"
#include "state.hpp"

void setupTrackball();
void tickTrackball(millis_t delta, Device& device, State& state);

#endif