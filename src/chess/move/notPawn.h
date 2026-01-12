#pragma once

#include "../attack/all.h"
#include "../UnwrapBoard.h"


namespace chess::move {
  template<int Code>
  constexpr BitBoard byCode(
    const auto& info,
    const BitBoard& pos,
    std::integral_constant<int, Code>
  ) {
    using CodeType = std::integral_constant<int, Code>;
    using ColorType = std::integral_constant<int, (Code & (1 << 3)) ? 1 : 0>;
    auto attack = chess::attack::byCode(info.getAll(), pos, CodeType());
    return 
      attack & 
      info.getNotColor(ColorType());
  }
} // namespace::move