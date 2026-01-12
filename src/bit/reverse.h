#pragma once

#include "dublicate.h"


namespace bit {
  template<size_t size, size_t count, class T>
  constexpr T baseReverse(T in) {
    constexpr uint64_t mask =
      dublicate<T>(
        (1ULL << size) - 1ULL, 
        size << 1, 
        count
      );
    return ((in & mask) << size) | (in >> size) & mask;
  }

  constexpr uint64_t reverse(uint64_t in) {
    in = baseReverse<1, 5>(in);
    in = baseReverse<2, 4>(in);
    in = baseReverse<4, 3>(in);
    in = baseReverse<8, 2>(in);
    in = baseReverse<16, 1>(in);
    in = baseReverse<32, 0>(in);
    return in;
  }
} // namespace bit