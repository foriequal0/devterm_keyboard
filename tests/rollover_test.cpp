#include <gtest/gtest.h>

#include <rollover.hpp>

static const Key k0{SpecialKey{0}}, k1{SpecialKey{1}}, k2{SpecialKey{2}};

TEST(RolloverTests, CanRegisterUpToDesignatedSize) {
  Rollover<2> rollover;

  rollover.set(k0, true);
  rollover.set(k1, true);

  rollover.set(k2, true);

  ASSERT_TRUE(rollover.get(k0));
  ASSERT_TRUE(rollover.get(k1));
  ASSERT_FALSE(rollover.get(k2));
}

TEST(RolloverTests, CanRegisterAfterRelease) {
  Rollover<2> rollover;

  rollover.set(k0, true);
  rollover.set(k1, true);
  rollover.set(k0, false);

  rollover.set(k2, true);

  ASSERT_FALSE(rollover.get(k0));
  ASSERT_TRUE(rollover.get(k1));
  ASSERT_TRUE(rollover.get(k2));
}