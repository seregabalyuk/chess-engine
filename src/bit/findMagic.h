#pragma once

#include "count.h"

#include <vector>
#include <stdint.h>
#include <random>


namespace bit {
  uint64_t findMagic(uint64_t num, auto& gen) {
    uint64_t countBit = bit::count(num);
    size_t obrCount = 64 - countBit;
    std::vector<char> buffer(1 << countBit);
    std::uniform_int_distribution<uint64_t> dist;
    
    while (1) {
      uint64_t maybeMagic = dist(gen) & dist(gen) & dist(gen);
      bool ok = 1;
      for (auto& x: buffer) x = 0;
      buffer[0] = 1;

      for (
        uint64_t mask1 = num; 
        mask1 != 0; 
        mask1 = (mask1 - 1) & num) 
      {
        uint64_t out = mask1 * maybeMagic >> obrCount;
        if (buffer[out] > 0) {
          ok = 0;
          break;
        }
        ++ buffer[out];
      }
      bool oks = 1;
      for (auto& x: buffer) if (x == 0) { 
        oks = 0; break; 
      }
      if (oks) {
        return maybeMagic;
      }
    }
  }
} // namespace bit