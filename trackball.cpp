#include <Arduino.h>

#include <cstdint>
#include <cmath>
#include <limits>
#include <array>

#include "trackball.hpp"
#include "ratemeter.hpp"
#include "glider.hpp"
#include "math.hpp"

#define LEFT PC8
#define UP PC9
#define RIGHT PC10
#define DOWN PC11

enum Axis: uint8_t {
  AXIS_X,
  AXIS_Y,
  AXIS_NUM,
};

static TrackballMode lastMode;
static int8_t distances[AXIS_NUM];
static RateMeter rateMeter[AXIS_NUM];
static Glider glider[AXIS_NUM];

static const int8_t WHEEL_DENOM = 2;
static int8_t wheelBuffer;

static float rateToVelocityCurve(float input) {
  return std::pow(std::abs(input) / 50, 1.5);
}

template<Axis AXIS, int8_t Direction>
static void interrupt() {
  distances[AXIS] += Direction;
  rateMeter[AXIS].onInterrupt();
  glider[AXIS].setDirection(Direction);

  const auto rx = rateMeter[AXIS_X].rate();
  const auto ry = rateMeter[AXIS_Y].rate();

  const auto rate = std::sqrt(rx * rx + ry * ry);
  const auto ratio = rateToVelocityCurve(rate) / rate;

  const auto vx = rx * ratio;
  const auto vy = ry * ratio;

  if (AXIS == AXIS_X) {
    glider[AXIS_X].update(vx, std::sqrt(rateMeter[AXIS_X].delta()));
    glider[AXIS_Y].updateSpeed(vy);
  } else {
    glider[AXIS_X].updateSpeed(vx);
    glider[AXIS_Y].update(vy, std::sqrt(rateMeter[AXIS_Y].delta()));
  }
}

void setupTrackball() {
  pinMode(LEFT, INPUT);
  pinMode(UP, INPUT);
  pinMode(RIGHT, INPUT);
  pinMode(DOWN, INPUT);

  attachInterrupt(LEFT, &interrupt<AXIS_X, -1>, ExtIntTriggerMode::CHANGE);
  attachInterrupt(RIGHT, &interrupt<AXIS_X, 1>, ExtIntTriggerMode::CHANGE);
  attachInterrupt(UP, &interrupt<AXIS_Y, -1>, ExtIntTriggerMode::CHANGE);
  attachInterrupt(DOWN, &interrupt<AXIS_Y, 1>, ExtIntTriggerMode::CHANGE);
}

void tickTrackball(millis_t delta, Device& device, State& state) {
  int8_t x = 0, y = 0, w = 0;
  noInterrupts();
  const auto mode = state.moveTrackball();
  if (lastMode != mode) {
    rateMeter[AXIS_X].expire();
    rateMeter[AXIS_Y].expire();
    wheelBuffer = 0;
  }
  else {
    rateMeter[AXIS_X].tick(delta);
    rateMeter[AXIS_Y].tick(delta);
  }
  lastMode = mode;

  switch (mode) {
    case TrackballMode::Mouse: {
      const auto rX = glider[AXIS_X].glide(delta);
      const auto rY = glider[AXIS_Y].glide(delta);
      x = rX.value;
      y = rY.value;
      if (rX.stopped) {
        glider[AXIS_Y].stop();
      }
      if (rY.stopped) {
        glider[AXIS_Y].stop();
      }

      break;
    }
    case TrackballMode::Wheel: {
      wheelBuffer += distances[AXIS_Y];
      w = wheelBuffer / WHEEL_DENOM;
      wheelBuffer -= w * WHEEL_DENOM;
      break;
    }
  }
  distances[AXIS_X] = 0;
  distances[AXIS_Y] = 0;
  interrupts();

  device.moveMouse(x, y, -w);
}
