#pragma once

#include "rook.h"
#include "bishop.h"

#include "../Codes.h"


namespace chess::attack {
  constexpr BitBoard queen(const BitBoard& all, const BitBoard& pos) {
    return rook(all, pos) | bishop(all, pos);
  }

  constexpr BitBoard byCode(
    const BitBoard& all,
    const BitBoard& pos,
    std::integral_constant<int, char2Code('Q')>
  ) {
    return queen(all, pos); 
  }

  constexpr BitBoard byCode(
    const BitBoard& all,
    const BitBoard& pos,
    std::integral_constant<int, char2Code('q')>
  ) {
    return queen(all, pos); 
  }
} // namespace chess::attack