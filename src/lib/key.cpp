#include "key.hpp"

#include <cassert>

MatrixKey::MatrixKey(uint8_t row, uint8_t col)
  : row(row),
    col(col) {
  assert(row < ROW_NUM);
  assert(col < COL_NUM);
}

SpecialKey::SpecialKey(uint8_t value): value(value) {
  assert(value < NUM);
}

static const size_t TAG_OFFSET = 6;

Key::Key()
{
  packed = 255;
}

Key::Key(MatrixKey matrix)
{
  assert(matrix.row < MatrixKey::ROW_NUM);
  assert(matrix.col < MatrixKey::COL_NUM);
  packed = (((uint8_t)Tag::Matrix) << TAG_OFFSET) | (matrix.row << 3) | matrix.col;
}

Key::Key(SpecialKey special)
{
  packed = ((uint8_t)Tag::Special << TAG_OFFSET) | special.value;
}

Key::Tag Key::tag() const
{
  assert(packed != 255);
  return (Tag)((packed >> TAG_OFFSET) & 0x01);
}

MatrixKey Key::matrix() const
{
  assert(packed != 255);
  assert(tag() == Tag::Matrix);
  const auto row = (uint8_t)((packed >> 3) & 0b111);
  const auto col = (uint8_t)(packed & 0b111);
  return MatrixKey { row, col };
}

SpecialKey Key::special() const
{
  assert(packed != 255);
  assert(tag() == Tag::Special);
  uint8_t masked = packed & (uint8_t)0b111111;
  return SpecialKey { masked };
}

bool Key::operator==(Key other) const
{
  assert(packed != 255);
  return packed == other.packed;
}

bool Key::operator!=(Key other) const
{
  assert(packed != 255);
  return packed != other.packed;
}
