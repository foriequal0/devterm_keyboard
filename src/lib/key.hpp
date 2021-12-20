#ifndef KEY_HPP
#define KEY_HPP

#include <cstddef>
#include <cstdint>

struct MatrixKey {
  static const uint8_t ROW_NUM = 8;
  static const uint8_t COL_NUM = 8;

  MatrixKey(uint8_t row, uint8_t col);
  bool operator==(MatrixKey other) const;
  const uint8_t row;
  const uint8_t col;
};

struct SpecialKey {
  static const size_t NUM = 17;

  explicit SpecialKey(uint8_t value);
  bool operator==(SpecialKey other) const;
  const uint8_t value;
};

class Key {
public:
  enum class Tag { Matrix, Special };

  Key();
  Key(MatrixKey matrix);
  Key(SpecialKey special);

  Tag tag() const;
  MatrixKey matrix() const;
  SpecialKey special() const;

  bool operator==(Key other) const;
  bool operator!=(Key other) const;

private:
  uint8_t packed;
};

#endif
