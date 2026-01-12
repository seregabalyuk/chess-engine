#pragma once

#include <cstddef>
#include <cstdint>


namespace bit {
  template<class T>
  constexpr T dublicate(T num, size_t size, size_t count2) {
    T ret = num;
    while (count2 --) {
      T half = ret;
      ret <<= size;
      ret |= half;
      size <<= 1;
    }
    return ret;
  }
}