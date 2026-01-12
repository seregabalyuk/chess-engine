#pragma once

#include "BitBoard.h"

namespace chess {
  struct UnwrapBoard {
    struct AllAndBlack {};
    BitBoard all;
    BitBoard notWhite;
    BitBoard notBlack;
    UnwrapBoard(AllAndBlack, 
      const BitBoard& all, 
      const BitBoard& black
    ):all(all), 
      notBlack(~black),
      notWhite(~(all ^ black)) 
    {}

    constexpr BitBoard getAll() const { return all; }

    constexpr BitBoard getNotAll() const { return ~all; }

    constexpr BitBoard getNotColor(std::integral_constant<int, 0>) const {
      return notWhite;
    }

    constexpr BitBoard getNotColor(std::integral_constant<int, 1>) const {
      return notBlack;
    }

    constexpr BitBoard getOtherColor(std::integral_constant<int, 0>) const {
      return ~notBlack;
    }

    constexpr BitBoard getOtherColor(std::integral_constant<int, 1>) const {
      return ~notWhite;
    }
  };
} // namespace chess