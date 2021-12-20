#ifndef STATE_HPP
#define STATE_HPP

#include <cstdint>

#include "lib/debouncer.hpp"

enum class TrackballMode : uint8_t {
  Wheel,
  Mouse,
};

class State
{
  public:
    static const uint16_t MIDDLE_CLICK_TIMEOUT_MS = 500;

    State();

    void tick(millis_t delta);

    bool fn;

    void pressMiddleClick();
    bool releaseMiddleClick();
    TrackballMode moveTrackball();
  private:
    bool middleClick;
    Timeout<uint16_t, MIDDLE_CLICK_TIMEOUT_MS> middleClickTimeout;
};

#endif
