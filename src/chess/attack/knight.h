#pragma once

#include "../../bit/log2.h"
#include "../BitBoard.h"

#include "../Codes.h"


namespace chess::attack {
  constexpr BitBoard knight(const BitBoard& all, const BitBoard& pos) {
    uint64_t nA  = 0xFeFeFeFeFeFeFeFe;
    uint64_t nAB = 0xFcFcFcFcFcFcFcFc;
    uint64_t  nH = 0x7f7f7f7f7f7f7f7f;
    uint64_t nGH = 0x3f3f3f3f3f3f3f3f;
    return nGH & (pos <<  6 | pos >> 10) |  
            nH & (pos << 15 | pos >> 17) | 
            nA & (pos << 17 | pos >> 15) | 
           nAB & (pos << 10 | pos >>  6);
  }

  constexpr BitBoard byCode(
    const BitBoard& all,
    const BitBoard& pos,
    std::integral_constant<int, char2Code('N')>
  ) {
    return knight(all, pos); 
  }

  constexpr BitBoard byCode(
    const BitBoard& all,
    const BitBoard& pos,
    std::integral_constant<int, char2Code('n')>
  ) {
    return knight(all, pos); 
  }
} // namespace chess::attack

