#ifndef ROLLOVER_HPP
#define ROLLOVER_HPP

#include <array>
#include <bitset>

#include "key.hpp"

template <size_t N> class Rollover {
public:
  Rollover() { bitmap.reset(); }

  void set(Key key, bool value) {
    // Find exact slot, update value
    for (size_t i = 0; i < N; i++) {
      if (!bitmap[i] || keys[i] != key) {
        continue;
      }

      bitmap[i] = value;
      return;
    }

    if (!value) {
      return;
    }

    // Find the empty slot and insert
    for (size_t i = 0; i < N; i++) {
      if (bitmap[i]) {
        continue;
      }

      bitmap[i] = true;
      keys[i] = key;
      return;
    }
  }

  bool get(Key key) const {
    for (size_t i = 0; i < N; i++) {
      if (bitmap[i] && keys[i] == key) {
        return true;
      }
    }

    return false;
  }

private:
  std::bitset<N> bitmap;
  std::array<Key, N> keys;
};

#endif
