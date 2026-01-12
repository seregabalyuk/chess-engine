#pragma once 

#include "Codes.h"

#include <type_traits>


namespace chess {
  template<int Color>
  using Rook = std::integral_constant<int, Color ? char2Code('r') : char2Code('R')>;
  template<int Color>
  using Bishop = std::integral_constant<int, Color ? char2Code('b') : char2Code('B')>;
  template<int Color>
  using King = std::integral_constant<int, Color ? char2Code('k') : char2Code('K')>;
  template<int Color>
  using Knight = std::integral_constant<int, Color ? char2Code('n') : char2Code('N')>;
  template<int Color>
  using Pawn = std::integral_constant<int, Color ? char2Code('p') : char2Code('P')>;
  template<int Color>
  using Queen = std::integral_constant<int, Color ? char2Code('q') : char2Code('Q')>;
  
} // namespace chess