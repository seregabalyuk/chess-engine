#pragma once

#include "dublicate.h"


namespace bit {
  template<size_t size, size_t count, class T>
  constexpr T baseCount(T in) {
    constexpr uint64_t mask =
      dublicate<T>(
        (1ULL << size) - 1ULL, 
        size << 1, 
        count
      );
    return (in & mask) + ((in >> size) & mask);
  }

  constexpr uint64_t count(uint64_t in) {
    in = baseCount<1, 5>(in);
    in = baseCount<2, 4>(in);
    in = baseCount<4, 3>(in);
    in = baseCount<8, 2>(in);
    in = baseCount<16, 1>(in);
    in = baseCount<32, 0>(in);
    return in;
  }
} // namespace bit