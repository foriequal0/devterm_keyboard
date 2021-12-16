#ifndef TRACKBALL_HPP
#define TRACKBALL_HPP

#include "device.hpp"
#include "state.hpp"
#include "debouncer.hpp"

void setupTrackball();
void tickTrackball(millis_t delta, Device& device, State& state);

#endif