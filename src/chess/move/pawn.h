#pragma once

#include "../attack/all.h"
#include "../InfoBoard.h"


namespace chess::move {
  template<int Color>
  constexpr BitBoard pawn(
    const auto& info,
    const BitBoard& pos,
    std::integral_constant<int, Color>
  ) {
    using ColorType = std::integral_constant<int, Color>;
    auto attack = 
      chess::attack::pawn(info.getAll(), pos, ColorType()) & 
      info.getOtherColor(ColorType());
    auto simpleMove = Color == 0 ? (pos >> 8) : (pos << 8);
    return attack | simpleMove & info.getNotAll();
  }

  constexpr BitBoard byCode(
    const auto& info,
    const BitBoard& pos,
    std::integral_constant<int, char2Code('P')>
  ) {
    return pawn(info, pos, 
      std::integral_constant<int, 0>());
  }

  constexpr BitBoard byCode(
    const auto& info,
    const BitBoard& pos,
    std::integral_constant<int, char2Code('p')>
  ) {
    return pawn(info, pos, 
      std::integral_constant<int, 1>());
  }
}