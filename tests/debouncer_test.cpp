#include <gtest/gtest.h>

#include <debouncer.hpp>

// Demonstrate some basic assertions.
TEST(Debouncer, ShouldSampleTrueImmediatly) {
  Debouncer<10> debouncer;

  // prepare not expired debouncer
  debouncer.sample(true);
  debouncer.updateTime(5);

  EXPECT_TRUE(debouncer.sample(true));
}

TEST(Debouncer, ShouldDebounceSampleFalse) {
  Debouncer<10> debouncer;

  // prepare not expired debouncer
  debouncer.sample(true);
  debouncer.updateTime(5);

  EXPECT_FALSE(debouncer.sample(false));
}

TEST(Debouncer, ShouldSampleFalse_WhenExpired) {
  Debouncer<10> debouncer;

  // prepare expired debouncer
  debouncer.sample(true);
  debouncer.updateTime(10);

  EXPECT_TRUE(debouncer.sample(false));
}
