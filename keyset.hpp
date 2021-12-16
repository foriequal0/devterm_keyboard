#ifndef KEYSET_HPP
#define KEYSET_HPP

#include <cstddef>
#include <bitset>

#include "key.hpp"

template<size_t N>
static size_t nextSetIndex(const std::bitset<N>& bits, size_t i) {
  while (i < N && !bits[i]) {
    i++;
  }

  return i;
}

template<typename Indexer>
class KeySetIterator {
  public:
    KeySetIterator(const std::bitset<Indexer::END>& bits, size_t i)
      : bits(bits),
        idx(i)
    {
    }

    KeySetIterator& operator++() {
      idx = nextSetIndex(bits, idx+1);
      return *this;
    }

    KeySetIterator operator++(int) {
      KeySetIterator tmp = *this;
      idx = nextSetIndex(bits, idx+1);
      return tmp;
    }

    typename Indexer::key_type operator*() const {
      return Indexer::fromIndex(idx);
    }

    bool operator==(KeySetIterator other) const {
      return idx == other.idx;
    }

    bool operator!=(KeySetIterator other) const {
      return idx != other.idx;
    }
  private:
    const std::bitset<Indexer::END>& bits;
    size_t idx;
};

template<typename Indexer>
class KeySet {
  public:
    KeySet() {
      bits.reset();
    }

    void update(typename Indexer::key_type key, bool value) {
      const auto index = Indexer::toIndex(key);
      bits[index] = value;
    }

    bool get(typename Indexer::key_type key) const {
      const auto index = Indexer::toIndex(key);
      return bits[index];
    }

    typedef KeySetIterator<Indexer> iterator;
    iterator begin() const {
      return iterator(bits, nextSetIndex(bits, 0));
    }
    iterator end() const {
      return iterator(bits, bits.size());
    }
  private:
    std::bitset<Indexer::END> bits;
};

struct MatrixKeysetIndexer {
  typedef MatrixKey key_type;

  static const size_t END = MatrixKey::COL_NUM * MatrixKey::ROW_NUM;

  inline static key_type fromIndex(size_t index) {
    uint8_t row = (index >> 3) & 0b111;
    uint8_t col = index & 0b111;
    return MatrixKey { row, col };
  }

  inline static size_t toIndex(MatrixKey key) {
    return key.row << 3 | key.col;
  }
};

class MatrixKeySet: public KeySet<MatrixKeysetIndexer>
{
};

struct SpecialKeysetIndexer {
  typedef SpecialKey key_type;

  static const size_t END = (size_t)SpecialKey::NUM;

  inline static key_type fromIndex(size_t index) {
    return SpecialKey(index);
  }

  inline static size_t toIndex(key_type key) {
    return key.value;
  }
};

class SpecialKeySet: public KeySet<SpecialKeysetIndexer>
{
};

#endif
