#pragma once

#include <cstdint>
#include <cassert>


namespace bit {
  constexpr uint64_t tab64[64] = {
      63,  0, 58,  1, 59, 47, 53,  2,
      60, 39, 48, 27, 54, 33, 42,  3,
      61, 51, 37, 40, 49, 18, 28, 20,
      55, 30, 34, 11, 43, 14, 22,  4,
      62, 57, 46, 52, 38, 26, 32, 41,
      50, 36, 17, 19, 29, 10, 13, 21,
      56, 45, 25, 31, 35, 16,  9, 12,
      44, 24, 15,  8, 23,  7,  6,  5
  };

  constexpr uint64_t log2 (uint64_t value) {
    value |= value >> 1;
    value |= value >> 2;
    value |= value >> 4;
    value |= value >> 8;
    value |= value >> 16;
    value |= value >> 32;
    return tab64[((value - (value >> 1)) * 0x07EDD5E59A4E28C2) >> 58];
  }

  constexpr uint64_t specLog2 (uint64_t value) {
    if (value == 0) { 
      assert(("you were trying siuka blyat" , false));
    }
    return 63 - __builtin_clzll(value);
    //return tab64[value * 0x07EDD5E59A4E28C2 >> 58];
  }
} // namespace bit

static_assert(16ULL == (1ULL << bit::log2(16ULL)), "not correct bit::number work");
static_assert(1ULL == (1ULL << bit::log2(1ULL)), "not correct bit::number work");
static_assert(0x0100000000000000 == (1ULL << bit::log2(0x0100000000000000)), "not correct bit::number work");
static_assert(0x0100000000000000 == (1ULL << bit::log2(0x0190000000000000)), "not correct bit::number work");

static_assert(16ULL == (1ULL << bit::specLog2(16ULL)), "not correct bit::number work");
static_assert(1ULL == (1ULL << bit::specLog2(1ULL)), "not correct bit::number work");
static_assert(0x0100000000000000 == (1ULL << bit::specLog2(0x0100000000000000)), "not correct bit::number work");
