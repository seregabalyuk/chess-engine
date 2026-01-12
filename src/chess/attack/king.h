#pragma once

#include "../../bit/log2.h"
#include "../BitBoard.h"

#include "../Codes.h"


namespace chess::attack {
  constexpr BitBoard king(const BitBoard& all, const BitBoard& pos) {
    uint64_t nA = 0xFeFeFeFeFeFeFeFe;
    uint64_t nH = 0x7f7f7f7f7f7f7f7f;
    return  
      nH & (pos << 7 | pos >> 9 | pos >> 1) |
      nA & (pos << 9 | pos >> 7 | pos << 1) |
      pos >> 8 | pos << 8;
  }

  constexpr BitBoard byCode(
    const BitBoard& all,
    const BitBoard& pos,
    std::integral_constant<int, char2Code('K')>
  ) {
    return king(all, pos); 
  }

  constexpr BitBoard byCode(
    const BitBoard& all,
    const BitBoard& pos,
    std::integral_constant<int, char2Code('k')>
  ) {
    return king(all, pos); 
  }
} // namespace chess::attack

