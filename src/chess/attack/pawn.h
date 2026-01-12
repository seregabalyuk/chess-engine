#pragma once

#include "../../bit/log2.h"
#include "../BitBoard.h"

#include "../Codes.h"


namespace chess::attack {
  constexpr BitBoard whitePawn(const BitBoard& all, const BitBoard& pos) {
    uint64_t nA = 0xFeFeFeFeFeFeFeFe;
    uint64_t nH = 0x7f7f7f7f7f7f7f7f;
    return nA & (pos >> 7) |  
           nH & (pos >> 9);
  }

  constexpr BitBoard blackPawn(const BitBoard& all, const BitBoard& pos) {
    uint64_t nA = 0xFeFeFeFeFeFeFeFe;
    uint64_t nH = 0x7f7f7f7f7f7f7f7f;
    return nH & (pos << 7) |  
           nA & (pos << 9);
  }

  constexpr BitBoard pawn(
    const BitBoard& all,
    const BitBoard& pos, 
    std::integral_constant<int, 0>
  ) {
    return whitePawn(all, pos);
  }

  constexpr BitBoard pawn(
    const BitBoard& all,
    const BitBoard& pos, 
    std::integral_constant<int, 1>
  ) {
    return blackPawn(all, pos);
  }

  constexpr BitBoard byCode(
    const BitBoard& all,
    const BitBoard& pos,
    std::integral_constant<int, char2Code('P')>
  ) {
    return whitePawn(all, pos); 
  }

  constexpr BitBoard byCode(
    const BitBoard& all,
    const BitBoard& pos,
    std::integral_constant<int, char2Code('p')>
  ) {
    return blackPawn(all, pos); 
  }
} // namespace chess::attack