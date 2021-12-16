#ifndef GLIDER_HPP
#define GLIDER_HPP

#include <cstdint>
#include "debouncer.hpp"

class Glider {
public:
  Glider();
  void setDirection(int8_t);
  void update(float velocity, uint16_t sustain);
  void updateSpeed(float velocity);
  void stop();

  struct GlideResult {
    int8_t value;
    bool stopped;
  };
  GlideResult glide(millis_t delta);

public:
  int8_t direction;
  float speed;
  uint16_t sustain;
  uint16_t release;
  float error;
};

#endif