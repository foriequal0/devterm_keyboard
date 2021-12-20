#include <cstdint>

#include <gtest/gtest.h>

#include <key.hpp>
#include <keyset.hpp>

TEST(KeyTests, ShouldRoundTripMatrixKey) {
  for (uint8_t row = 0; row < MatrixKey::ROW_NUM; row++) {
    for (uint8_t col = 0; col < MatrixKey::COL_NUM; col++) {
      MatrixKey matrixKey{row, col};
      Key key{matrixKey};

      ASSERT_TRUE(key.tag() == Key::Tag::Matrix);
      ASSERT_TRUE(key.matrix() == matrixKey);
    }
  }
}

TEST(KeyTests, ShouldRoundTripSpecialKey) {
  for (uint8_t value = 0; value < SpecialKey::NUM; value++) {
    SpecialKey specialKey{value};
    Key key{specialKey};

    ASSERT_TRUE(key.tag() == Key::Tag::Special);
    ASSERT_TRUE(key.special() == specialKey);
  }
}

TEST(MatrixKeySetTests, SmokeTest) {
  MatrixKeySet keyset;
  for (uint8_t row = 0; row < MatrixKey::ROW_NUM; row++) {
    for (uint8_t col = 0; col < MatrixKey::COL_NUM; col++) {
      MatrixKey key{row, col};

      ASSERT_FALSE(keyset.get(key));
      keyset.update(key, true);
      ASSERT_TRUE(keyset.get(key));
    }
  }
}

TEST(SpecialKeySetTests, SmokeTest) {
  SpecialKeySet keyset;
  for (uint8_t value = 0; value < SpecialKey::NUM; value++) {
    SpecialKey key{value};

    ASSERT_FALSE(keyset.get(key));
    keyset.update(key, true);
    ASSERT_TRUE(keyset.get(key));
  }
}