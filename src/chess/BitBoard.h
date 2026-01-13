#pragma once

#include <stdint.h>
#include <string>
#include <type_traits>


namespace chess {
  using BitBoard = uint64_t;

  int getBB(const BitBoard& bitboard, int i) {
    return (bitboard & (1ULL << i)) ? 1 : 0;
  }

  void setBB(BitBoard& bitboard, int i, bool x) {
    if (x) {
      bitboard |= (1ULL << i);
    } else {
      bitboard &= ~(1ULL << i);
    }
  }
  
  void fastSetBB(
    BitBoard& bitboard,
    const BitBoard& i, 
    std::bool_constant<0>
  ) {
    bitboard &= ~i;
  }

  void fastSetBB(
    BitBoard& bitboard, 
    const BitBoard& i, 
    std::bool_constant<1>
  ) {
    bitboard |= i;
  }

  BitBoard getPosBB(
    const BitBoard& bitboard,
    std::bool_constant<1>
  ) {
    return bitboard;
  }

  BitBoard getPosBB(
    const BitBoard& bitboard,
    std::bool_constant<0>
  ) {
    return ~bitboard;
  }

  std::string toString(const BitBoard& bitboard) {
    std::string ret;
    for (int i = 0; i < 64; ++ i) {
      ret.push_back(getBB(bitboard, i) ? 'X' : '.');
      if (i % 8 == 7) {
        ret.push_back('\n');
      }
    }
    return ret;
  }

  int countBB(const BitBoard& bitboard) {
    return __builtin_popcountll(bitboard);
  }
} // namespace chess