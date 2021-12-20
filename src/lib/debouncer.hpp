#ifndef DEBOUNCER_HPP
#define DEBOUNCER_HPP

#include <cstdint>

typedef uint8_t millis_t;

/**
   @brief Asymmetric debouncer
*/
template <millis_t MS> class Debouncer {
public:
  Debouncer() : timeout(0) {}

  void updateTime(millis_t delta) {
    if (timeout > delta) {
      timeout -= delta;
    } else {
      timeout = 0;
    }
  }

  bool sample(bool value) {
    if (value || timeout == 0) {
      timeout = MS;
      return true;
    }

    return false;
  }

private:
  millis_t timeout;
};

template <typename T, T millis> class Timeout {
public:
  Timeout() { timeout = 0; }

  void updateTime(millis_t delta) {
    if (timeout > delta) {
      timeout -= delta;
    } else {
      timeout = 0;
    }
  }

  void expire() { timeout = 0; }

  bool get() const { return timeout == 0; }

  void reset() { timeout = millis; }

private:
  uint16_t timeout;
};

#endif
