#include <limits>

#include <gtest/gtest.h>

#include <math.hpp>

TEST(MathTests, getDeltaShouldHandleOverwrap) {
  uint32_t before = std::numeric_limits<uint32_t>::max();
  uint32_t after = 0;

  auto delta = getDelta(before, after);
  ASSERT_EQ(delta, 1);
}